/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:36:09 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/27 15:45:14 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <map>
#include "Channel.hpp"
#include "Lobby.hpp"

class Server
{
	
public:
	Server(const char* port, const char* password);
	~Server();
	bool run();
	std::map<std::string, Channel *> getChannels() const;
private:
	enum { MAX_BUFFER = 512 };
	Lobby mLobby;
	std::map<std::string, Channel *> mChannels;
	int mServerSocket;
	bool mbRunning;
	const int mPort;
	const unsigned int mPassword;
	
	fd_set getFdSet() const;
	int getMaxFd() const;
	void stop();
	Channel* findChannel(const int clientSocket);
	void acceptClient();
	void clearStream(const int socket);
	bool sendToClient(const int clientSocket, const char* message);
	bool acceptUser(const int clientSocket);
	void handleClientMessage(const int clientSocket);
	bool isDuplicatedUsername(const char* buffer) const;
	bool isDuplicatedNickname(const char* buffer) const;
	bool isInvalidPortNumber(const char* port) const;
	bool isInvalidPasswordFormatted(const char* password) const;
	bool isInvalidPassword(const char* password) const;
	bool attemptReceiveValidData(const int clientSocket, char *buffer, \
		bool (Server::*isInvalid)(const char *) const, const char *message, const int maxCount);


};