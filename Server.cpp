/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:40:43 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/24 19:56:37 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "Server.hpp"
#include "Parser.hpp"
#include "Util.hpp"
#define SYSCALL_FAIL (-1)

Server::Server(const char* port, const char* password)
: mPort(std::atoi(port))
, mPassword(Util::generateHash65599(password))
, mbRunning(false)
{
	if (Parser::isInvalidPortNumber(port))
	{
		std::cerr << "옳지 않은 port 번호, 6660~6670 사이로 넣어야 합니다." << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (Parser::isInvalidPassword(password))
	{
		std::cerr << "비밀번호는 8~16자리, 대소문자, 숫자로만 이루어져야합니다." << std::endl;
		exit(EXIT_FAILURE);
	}

	mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mServerSocket == SYSCALL_FAIL)
	{
		std::cerr << "fail socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	int on = 1;
	if (setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) == -1)
	{
		std::cerr << "fail setsocketopt" << std::endl;
		exit(EXIT_FAILURE);
	}

	sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(sockaddr_in));
	serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(mPort);
	if (bind(mServerSocket, (const sockaddr *)&serverAddr, sizeof(sockaddr_in)) == SYSCALL_FAIL)
	{
		std::cerr << "fail bind" << std::endl;
		exit(EXIT_FAILURE);
	}

	const size_t MAX_PENDING_CONNECTIONS = 100;
	if (listen(mServerSocket, MAX_PENDING_CONNECTIONS) == SYSCALL_FAIL)
	{
		std::cerr << "fail listen" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	// non blocking 을 다른 걸로 쓰기

	std::cout << "서버가 " << mPort << " 포트에서 실행 중입니다..." << std::endl;
}

Server::~Server()
{
	close(mServerSocket);
	mbRunning = false;
}

/* getter */
fd_set Server::getFdSet() const
{
	fd_set master;
	
	FD_ZERO(&master);
	
	FD_SET(mServerSocket, &master);
	FD_SET(STDIN_FILENO, &master);
	std::map<std::string, Channel>::const_iterator it = mChannels.begin();
	while (it != mChannels.end())
	{
		std::vector<int> fdSet = it->second.getFdSet();
		for (size_t i = 0; i < fdSet.size(); ++i)
		{
			FD_SET(fdSet[i], &master);
		}
		++it;
	}
	return master;
}

int Server::getMaxFd() const
{
	int maxFd = 0;

	std::map<std::string, Channel>::const_iterator it = mChannels.begin();
	while (it != mChannels.end())
	{
		std::vector<int> fdSet = it->second.getFdSet();
		for (size_t i = 0; i < fdSet.size(); ++i)
		{
			if (fdSet[i] > maxFd)
			{
				maxFd = fdSet[i];
			}
		}
		++it;
	}
	return maxFd;
}

/**/

bool Server::run()
{
	mbRunning = true;
	
	while (mbRunning)
	{
		fd_set temp_fds = getFdSet();
		int maxFd = getMaxFd();	
		if (select(maxFd + 1, &temp_fds, NULL, NULL, NULL) == SYSCALL_FAIL)
		{
			assert(false);
		}
		
		for (size_t i = 0; i < maxFd; ++i)
		{
			if (FD_ISSET(i, &temp_fds) == false)
			{
				continue;
			}
			else if (i == mServerSocket)
			{
				acceptClient();
			}
			else if (i == STDIN_FILENO)
			{
				// 서버 콘솔에서 입력 처리
				// 서버 운영자 명령어 
			}
			else
			{
				// 클라 메시지
				// 운영자일 경우 뭐어쩌구저쩌구
				// 
			}
			
			// 클라이언트랑 수신
			// 채널 참여
			// 이름 찾아서 추가 해주면 되고

			// 채널 생성
			
			// 메시지 보내고
		}
	}
	return (true);
}

void Server::acceptClient()
{
	struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
	int clientSocket = accept(mServerSocket, (struct sockaddr *)&clientAddr, &clientLen);
	if (clientSocket < 0)
	{
		std::cerr << "fail accept" << std::endl;
		return;
	}

	const char* message = "password 를 입력해주세요.\r\n";
	send(clientSocket, message, strlen(message), 0);
	
	char buffer[MAX_BUFFER] = { 0, };
	int len = recv(mServerSocket, buffer, sizeof(buffer), 0);
	buffer[MAX_BUFFER - 1] = '\0';
	
	
}

void Server::stop()
{
	mbRunning = false;
}
