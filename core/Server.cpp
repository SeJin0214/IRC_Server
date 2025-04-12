/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:40:43 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/12 17:21:25 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <arpa/inet.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <netinet/in.h>
#include <set>
#include "Server.hpp"
#include "Space.hpp"
#include "Util.hpp"
#include "Result.hpp"
#include "IOutgoingMessageProvider.hpp"
#include "MessageBetch.hpp"
#include "JoinCommand.hpp"
#include "Parser.hpp"
#define SYSCALL_FAIL (-1)

Server::Server(const char* port, const char* password)
: mName("irc.local")
, mbRunning(false)
, mPort(std::atoi(port))
, mPassword(Util::generateHash65599(password))
{
	if (Parser::isInvalidPortNumber(port))
	{
		std::cerr << "옳지 않은 port 번호, 1024~65534 사이로 넣어야 합니다." << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (Parser::isInvalidPasswordFormatted(password))
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
	std::set<int> clientSockets;
	std::vector<Space*> spaces = getSpaces();
	for (size_t i = 0; i < spaces.size(); ++i)
	{
		std::vector<int> fdSet = spaces[i]->getFdSet();
		for (size_t i = 0; i < fdSet.size(); ++i)
		{
			clientSockets.insert(fdSet[i]);
		}
	}

	for (std::set<int>::iterator it = clientSockets.begin(); it != clientSockets.end(); ++it)
	{
		leaveServer(*it);
	}

	close(mServerSocket);
	mbRunning = false;
}

std::vector<Space*> Server::getSpaces()
{
	std::vector<Space*> spaces;
	for (size_t i = 0; i < mChannels.size(); ++i)
	{
		spaces.push_back(mChannels[i]);
	}
	spaces.push_back(&mLobby);
	spaces.push_back(&mLoggedInSpace);
	return spaces;
}

std::vector<const Space *> Server::getSpaces() const
{
	std::vector<const Space*> spaces;
	for (size_t i = 0; i < mChannels.size(); ++i)
	{
		spaces.push_back(mChannels[i]);
	}
	spaces.push_back(&mLobby);
	spaces.push_back(&mLoggedInSpace);
	return spaces;
}

/* getter */
std::string Server::getServerName() const
{
	return mName;
}

unsigned int Server::getPassword() const
{
	return mPassword;
}

fd_set Server::getFdSet() const
{
	fd_set master;
	
	FD_ZERO(&master);
	
	FD_SET(mServerSocket, &master);
	FD_SET(STDIN_FILENO, &master);

	std::vector<const Space*> spaces = getSpaces();
	for (size_t i = 0; i < spaces.size(); ++i)
	{
		std::vector<int> fdSet = spaces[i]->getFdSet();
		for (size_t i = 0; i < fdSet.size(); ++i)
		{
			FD_SET(fdSet[i], &master);
		}
	}
	return master;
}

int Server::getMaxFd() const
{
	int maxFd = mServerSocket;

	std::vector<const Space*> spaces = getSpaces();
	for (size_t i = 0; i < spaces.size(); ++i)
	{
		std::vector<int> fdSet = spaces[i]->getFdSet();
		for (size_t i = 0; i < fdSet.size(); ++i)
		{
			if (fdSet[i] > maxFd)
			{
				maxFd = fdSet[i];
			}
		}
	}
	return maxFd;
}

const Space* Server::findSpace(const int clientSocket) const
{
	std::vector<const Space*> spaces = getSpaces();
	for (size_t i = 0; i < spaces.size(); ++i)
	{
		const User* user = spaces[i]->findUserOrNull(clientSocket);
		if (user != NULL)
		{
			return spaces[i];
		}
	}
	assert(false);
	return NULL;
}

User* Server::findUserInAllSpace(const int clientSocket) const
{
	std::vector<const Space*> spaces = getSpaces();
	for (size_t i = 0; i < spaces.size(); ++i)
	{
		User* user = spaces[i]->findUserOrNull(clientSocket);
		if (user != NULL)
		{
			return user;
		}
	}
	assert(false);
	return NULL;
}

Channel* Server::createChannel(const std::string& title)
{
	assert(findChannelOrNull(title) == NULL 
	|| findChannelOrNull(title)->getTitle() != title);
	Channel* newChannel = new Channel(title, "");
	mChannels.push_back(newChannel);
	return newChannel;
}

Result<User> Server::findUser(const int clientSocket) const
{
	for (size_t i = 0; i < mChannels.size(); ++i)
	{
		User* user = mChannels[i]->findUserOrNull(clientSocket);
		if (user != NULL)
		{
			Result<User> result(*user, true);
			return result;
		}
	}
	User* user = mLobby.findUserOrNull(clientSocket);
	if (user != NULL)
	{
		Result<User> result(*user, true);
		return result;
	}
	Result<User> emptyUser(User(), false);
	return emptyUser;
}

// return <socket, User>
Result<std::pair<int, User> > Server::findUser(const std::string& nickname) const
{
	for (size_t i = 0; i < mChannels.size(); ++i)
	{
		Result<std::pair<int, User*> > userToFind = mChannels[i]->findUser(nickname);
		if (userToFind.hasSucceeded())
		{
			std::pair<int, User*> pointerPair(userToFind.getValue());
			std::pair<int, User> objPair(pointerPair.first, *pointerPair.second);
			Result<std::pair<int, User> > result(objPair, true);
			return result;
		}
	}
	Result<std::pair<int, User*> > userToFind = mLobby.findUser(nickname);
	if (userToFind.hasSucceeded())
	{
		std::pair<int, User *> pointerPair(userToFind.getValue());
		std::pair<int, User> objPair(pointerPair.first, *pointerPair.second);
		Result<std::pair<int, User> > result(objPair, true);
		return result;
	}
	std::pair<int, User> invalidPair(-1, User());
	Result<std::pair<int, User> > emptyUser(invalidPair, false);
	return emptyUser;
}

Channel* Server::findChannelOrNull(const std::string& title) const
{
	for (size_t i = 0; i < mChannels.size(); ++i)
	{
		Channel* channel = mChannels[i];
		if (channel->getTitle() == title)
		{
			return channel;
		}
	}
	return NULL;
}

Channel* Server::findChannelOrNull(const int clientSocket) const
{
	for (size_t i = 0; i < mChannels.size(); ++i)
	{
		Channel* channel = mChannels[i];
		User* result = channel->findUserOrNull(clientSocket);
		if (result != NULL)
		{
			return channel;
		}
	}
	return NULL;
}

 
bool Server::trySetAuthenticatedInLoggedSpace(const int clientSocket)
{
	return mLoggedInSpace.trySetAuthenticated(clientSocket);
}

bool Server::trySetNicknameInLoggedSpace(const int clientSocket, const std::string& nickname)
{
	return mLoggedInSpace.trySetNickname(clientSocket, nickname);
}

bool Server::trySetUsernameInLoggedSpace(const int clientSocket, const std::string& username)
{
	return mLoggedInSpace.trySetUsername(clientSocket, username);
}

void Server::loginToServer(const int clientSocket, User* user)
{
	assert(user != NULL);
	std::string message = ":";
	message += mName + " 001 " + user->getNickname() + " :Welcome to the Internet Relay Chat Network!\r\n";
	std::cout << message << std::endl;
	sendToClient(clientSocket, message.c_str());
	enterUserInLobby(clientSocket, user);
}

bool Server::enterUserInLobby(const int clientSocket, User* user)
{
	assert(user != NULL);
	return mLobby.enterUser(clientSocket, user);
}

User* Server::exitUserInLobbyOrNull(const int clientSocket)
{
	return mLobby.exitUserOrNull(clientSocket);
}

bool Server::enterUserInChannel(const int clientSocket, const std::string& title)
{
	Channel* channel = findChannelOrNull(title);
	if (channel == NULL)
	{
		channel = createChannel(title);
		assert(channel != NULL);
	}
	User* user = findUserInAllSpace(clientSocket);
	exitUserInLobbyOrNull(clientSocket);
	return channel->enterUser(clientSocket, user);
}

User* Server::exitUserInChannel(const int clientSocket, const std::string& title)
{
	Channel* channel = findChannelOrNull(title);
	if (channel == NULL)
	{
		return NULL;
	}
	User* user = channel->exitUserOrNull(clientSocket);
	if (user == NULL)
	{
		return NULL;
	}
	if (channel->getUserCount() == 0)
	{
		std::vector<Channel *>::iterator it = mChannels.begin();
		while (it != mChannels.end())
		{
			if ((*it)->getTitle() == channel->getTitle())
			{
				mChannels.erase(it);
				break;
			}
			++it;
		}
		delete channel;
	}
	if (user->getJoinedChannelCount() == 0)
	{
		mLobby.enterUser(clientSocket, user);
	}
	return user;
}


bool Server::run()
{
	mbRunning = true;
	
	while (mbRunning)
	{
		fd_set temp_fds = getFdSet();
		int maxFd = getMaxFd();
		
		struct timeval tv;
		tv.tv_sec = 0;
    	tv.tv_usec = 100000;
		if (select(maxFd + 1, &temp_fds, NULL, NULL, &tv) == SYSCALL_FAIL)
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
				char buffer[MAX_BUFFER] = { 0, };
				const int readLength = read(STDIN_FILENO, buffer, MAX_BUFFER - 1);
				if (readLength == 0)
				{
					return false;
				}
				assert(buffer[readLength - 1] == '\n');
 				buffer[readLength - 1] = '\0';
				if (std::strncmp("quit", buffer, readLength) == 0)
				{
					mbRunning = false;
				}
			}
			else
			{
				handleClientMessage(i);
			}
		}
		
		mLoggedInSpace.admitOrExile(*this);
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
	mLoggedInSpace.enterUser(clientSocket, new User());
}

void Server::handleClientMessage(const int clientSocket)
{
	char buffer[MAX_BUFFER] = { 0, };
	
	const int readLength = recv(clientSocket, buffer, MAX_BUFFER - 1, 0);
	if (readLength == 0)
	{
		return;
	}
	else if (readLength == MAX_BUFFER - 1)
	{
		while (recv(clientSocket, buffer, MAX_BUFFER - 1, 0) == MAX_BUFFER - 1)
		{
			continue;
		}
		sendToClient(clientSocket, "ERROR :Line too long");
		return;
	}

	User *user = findUserInAllSpace(clientSocket);
	std::string remain = user->flushBuffer();
	
	std::stringstream ss;
	ss << remain << buffer;
	
	std::string line;
	while (true)
	{
		std::getline(ss, line);
		if (ss.fail())
		{
			break;
		}

		if (ss.eof())
		{
			user->appendStringInBuffer(line.c_str());
			continue;
		}

		if (line[line.size() - 1] == '\r')
		{
			line[line.size() - 1] = '\0';
		}
		ExecuteCommandByProtocol(clientSocket, line.c_str());
	}
}

// TODO: remove log
void Server::ExecuteCommandByProtocol(const int clientSocket, const char* buffer)
{
	const Space* space = findSpace(clientSocket);

	std::cout << "클라 -> 서버: |" << buffer << std::endl;
	
	IOutgoingMessageProvider* outgoingMessageProvider = space->getOutgoingMessageProvider(buffer);
	if (outgoingMessageProvider != NULL)
	{
		MessageBetch messageBetch = outgoingMessageProvider->getMessageBetch(*this, clientSocket, buffer);
		std::vector<std::pair<int, std::string> > socketAndMessages = messageBetch.getMessage();
		for (size_t i = 0; i < socketAndMessages.size(); ++i)
		{
			std::pair<int, std::string> socketAndMessage = socketAndMessages[i];
			std::cout << "서버 -> 클라: |" << socketAndMessage.second << std::endl;
			sendToClient(socketAndMessage.first, socketAndMessage.second.c_str());
		}
	}

	delete outgoingMessageProvider;

	IExecutable *executor = space->getExecutor(buffer);
	if (executor != NULL)
	{
		executor->execute(*this, clientSocket, buffer);
	}
	delete executor;
}

bool Server::isDuplicatedNickname(const char* buffer) const
{
	std::vector<const Space *> spaces = getSpaces();
	for (size_t i = 0; i < spaces.size(); ++i)
	{
		std::vector<std::string> nicknames = spaces[i]->getNicknames();
		for (size_t i = 0; i < nicknames.size(); i++)
		{
			if (nicknames[i] == buffer)
			{
				return true;
			}
		}
	}
	return false;
}

bool Server::sendToClient(const int clientSocket, const char* message)
{
	char buffer[MAX_BUFFER + 2] = { 0, };
	sprintf(buffer, "%s\r\n", message);
    send(clientSocket, buffer, std::strlen(buffer), 0);
	return (true);
}

std::vector<const Channel*> Server::loadChannels() const
{
	std::vector<const Channel*> channels;
	for (size_t i = 0; i < mChannels.size(); ++i)
	{
		channels.push_back(mChannels[i]);
	}
	return channels;
}

bool Server::isInvalidPassword(const char* password) const
{
	bool bIsSucceeded;
	if (Parser::isInvalidPasswordFormatted(password))
	{
		return true;
	}

	bIsSucceeded = mPassword != Util::generateHash65599(password);
	return bIsSucceeded;
}

void Server::leaveServer(const int clientSocket)
{
	const char* quitProtocol = "QUIT :leaving";
	ExecuteCommandByProtocol(clientSocket, quitProtocol);
}

void Server::exitAllSpaces(const int clientSocket)
{
	User* user;
	std::vector<Space *> spaces = getSpaces();
	for (size_t i = 0; i < spaces.size(); ++i)
	{
		User* temp = spaces[i]->exitUserOrNull(clientSocket);
		if (temp != NULL)
		{
			user = temp;
		}
	}

	std::vector<Channel *>::iterator it = mChannels.begin();
	while (it != mChannels.end())
	{
		if ((*it)->getUserCount() == 0)
		{
			Channel* channel = *it;
			it = mChannels.erase(it);
			delete channel;
		}
		else
		{
			++it;
		}
	}
	close(clientSocket);
	delete user;
}