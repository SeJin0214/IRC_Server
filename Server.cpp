/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:40:43 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/27 16:12:01 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <arpa/inet.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "Server.hpp"
#include "Space.hpp"
#include "Util.hpp"
#include "Result.hpp"
#define SYSCALL_FAIL (-1)

Server::Server(const char* port, const char* password)
: mbRunning(false)
, mPort(std::atoi(port))
, mPassword(Util::generateHash65599(password))
{
	if (isInvalidPortNumber(port))
	{
		std::cerr << "옳지 않은 port 번호, 6660~6670 사이로 넣어야 합니다." << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (isInvalidPasswordFormatted(password))
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
	std::map<std::string, Channel *>::const_iterator it = mChannels.begin();
	while (it != mChannels.end())
	{
		std::vector<int> fdSet = it->second->getFdSet();
		for (size_t i = 0; i < fdSet.size(); ++i)
		{
			FD_SET(fdSet[i], &master);
		}
		++it;
	}
	std::vector<int> fdSet = mLobby.getFdSet();
	for (size_t i = 0; i < fdSet.size(); ++i)
	{
		FD_SET(fdSet[i], &master);
	}
	return master;
}

int Server::getMaxFd() const
{
	int maxFd = mServerSocket;

	std::map<std::string, Channel *>::const_iterator it = mChannels.begin();
	while (it != mChannels.end())
	{
		std::vector<int> fdSet = it->second->getFdSet();
		for (size_t i = 0; i < fdSet.size(); ++i)
		{
			if (fdSet[i] > maxFd)
			{
				maxFd = fdSet[i];
			}
		}
		++it;
	}
	std::vector<int> fdSet = mLobby.getFdSet();
	for (size_t i = 0; i < fdSet.size(); ++i)
	{
		if (fdSet[i] > maxFd)
		{
			maxFd = fdSet[i];
		}
	}
	return maxFd;
}

std::map<std::string, Channel *> Server::getChannels() const
{
	return mChannels;
}

Channel* Server::findChannel(const int clientSocket)
{
	std::map<std::string, Channel *>::iterator it = mChannels.begin();
	while (it != mChannels.end())
	{
		Result<User> result = it->second->findUser(clientSocket);
		if (result.isSucceed())
		{
			return it->second;
		}
		++it;
	}
	return NULL;
}

// TODO: i == STDIN_FILENO 일 때, 서버 콘솔에서 입력 처리, 서버 운영자 명령어 처리
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
		
		for (int i = 0; i <= maxFd; ++i)
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
			
			}
			else
			{
				handleClientMessage(i);
			}
		}
	}
	return (true);
}

void Server::acceptClient()
{
	struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
	const int clientSocket = accept(mServerSocket, (struct sockaddr *)&clientAddr, &clientLen);
	if (clientSocket < 0)
	{
		std::cerr << "클라이언트 연결 실패" << std::endl;
		assert(false);
		return;
	}

	const char* message = "비밀번호를 입력해주세요.\r\n";
	sendToClient(clientSocket, message);

	char buffer[MAX_BUFFER] = { 0, };
	
	const char* passwordFailedMessage = "비밀번호가 일치하지 않습니다. 다시 시도해 주세요\r\n";

	if (attemptReceiveValidData(clientSocket, buffer, &Server::isInvalidPassword, passwordFailedMessage, 3))
    {
		char clientIP[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
		std::cout << "클라이언트 연결: " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;

		const char* welcomMessage = "IRC 서버에 오신 것을 환영합니다!\r\n";
		sendToClient(clientSocket, welcomMessage);

		if (acceptUser(clientSocket))
		{
			return;
		}
	}
	
	const char *refusalMessage = "비밀번호가 3회 일치 하지 않습니다. 다시 접속해 주세요.\r\n";
	sendToClient(clientSocket, refusalMessage);
	close(clientSocket);
}

bool Server::acceptUser(const int clientSocket)
{
	const char* requestMessage = "유저이름을 입력해주세요.\r\n";
	sendToClient(clientSocket, requestMessage);
	char buffer[MAX_BUFFER] = {	0, };
	
	const char* duplicatedUsernameMessage = "유저이름이 이미 존재합니다. 다시 시도해주세요\r\n";
	if (attemptReceiveValidData(clientSocket, buffer, &Server::isDuplicatedUsername, duplicatedUsernameMessage, __INT_MAX__) == false)
	{
		sendToClient(clientSocket, "유저이름 생성 시도가 실패하였습니다. 다시 접속해 주세요.\r\n");
		return false;
	}

	const char* requestNicknameMessage = "닉네임을 입력해주세요.\r\n";
	sendToClient(clientSocket, requestNicknameMessage);
	
	std::string username = buffer;
	const char* duplicatedNicknameMessage = "닉네임이 이미 존재합니다. 다시 시도해주세요\r\n";
	if (attemptReceiveValidData(clientSocket, buffer, &Server::isDuplicatedNickname, duplicatedNicknameMessage, __INT_MAX__) == false)
	{
		sendToClient(clientSocket, "닉네임 생성 시도가 실패하였습니다. 다시 접속해 주세요.\r\n");
		return false;
	}

	std::string nickname = buffer;

	User user(username, nickname);
	mLobby.enterUser(clientSocket, user);
	
	sendToClient(clientSocket, mLobby.getHelpMessage().c_str());
	return true;
}

/**
 *  recv max 512 byte
*/
bool Server::attemptReceiveValidData(const int clientSocket, char *buffer, bool (Server::*isInvalid)(const char *) \
const, const char *message, const int maxCount)
{
	int i = 0;
	for (i = 0; i < maxCount; ++i)
	{
		const int readLength = recv(clientSocket, buffer, MAX_BUFFER, 0);
		buffer[readLength - 1] = '\0';

		if (readLength == MAX_BUFFER)
		{
			clearStream(clientSocket);
		}

		if (readLength < 0)
		{
			std::cerr << "데이터 수신 실패" << std::endl;
			assert(false);
			return false;
		}
		if ((this->*isInvalid)(buffer) == false)
		{
			break;
		}
		sendToClient(clientSocket, message);
	}
	if (i == maxCount)
	{
		return false;
	}
	return true;
}

void Server::clearStream(const int socket)
{
	char buffer[MAX_BUFFER] = { 0, };
	while (recv(socket, buffer, sizeof(buffer), 0) > MAX_BUFFER - 1)
	{
		continue;
	}
}

// TODO: 명령어 실행 구현
void Server::handleClientMessage(const int clientSocket)
{
	char buffer[MAX_BUFFER] = { 0, };
	
	const int readLength = recv(clientSocket, buffer, MAX_BUFFER, 0);
	buffer[readLength - 1] = '\0';
	clearStream(clientSocket);
	if (readLength > 500)
	{
		const char* over = "메시지는 500자 이내로 보내야 합니다.\r\n";
		sendToClient(clientSocket, over);
		return;
	}
 
	Channel* channel = findChannel(clientSocket);
	if (channel == NULL)
	{
		// 명령어인지 확인하는 불리언 반환 함수
		
		// 로비 명령어 실행
		const char* test = "test~~~~~~~~~~~~~~~~~~~~~\r\n";
		sendToClient(clientSocket, test);
		return;
	}
	
	Result<User> result = channel->findUser(clientSocket);
	std::vector<int> sockets = channel->getClientSockets();
	for (size_t i = 0; i < sockets.size(); ++i)
	{
		if (static_cast<int>(sockets[i]) == clientSocket)
		{
			continue;
		}
		std::string message = result.getValue().getNickname() + " : " + buffer;
		sendToClient(sockets[i], message.c_str());
	}
}

void Server::stop()
{
	mbRunning = false;
}

bool Server::isDuplicatedUsername(const char* buffer) const
{
	(void) buffer;
	return (false);
}

bool Server::isDuplicatedNickname(const char* buffer) const
{
	(void) buffer;
	return (false);
}

// TODO: 에러처리 구현
bool Server::sendToClient(const int clientSocket, const char* message)
{
    send(clientSocket, message, std::strlen(message), 0);
	// 에러 처리
	return (true);
}


bool Server::isInvalidPortNumber(const char* port) const
{
	(void) port;
	return (false);
}

bool Server::isInvalidPassword(const char* password) const
{
	bool isSucceed;
	if (isInvalidPasswordFormatted(password))
	{
		return true;
	}

	isSucceed = mPassword != Util::generateHash65599(password);
	return isSucceed;
}

// 8 ~ 16  min Uppercase, lowercase, digit
bool Server::isInvalidPasswordFormatted(const char* password) const
{
    const int length = std::strlen(password);
    if (length < 8  || length > 16)
    {
        return true;
    }
    for (int i = 0; i < length; ++i)
    {
        if (!(std::isalnum(password[i]) || std::isupper(password[i])))
        {
            return true;
        }
    }
    return false;
}
