/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoggedInSpace.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:13:46 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/10 14:18:21 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <cstring>
#include "ErrorCommand.hpp"
#include "LoggedInSpace.hpp"
#include "NickCommand.hpp"
#include "UserCommand.hpp"
#include "PassCommand.hpp"
#include "QuitCommand.hpp"

IOutgoingMessageProvider* LoggedInSpace::getOutgoingMessageProvider(const char *buffer) const
{
	assert(buffer != NULL);

	std::string command = getCommandSection(buffer);
	if (command == "NICK")
	{
		return new NickCommand();
	}
	else if (command == "QUIT")
	{
		return new QuitCommand();
	}
	return NULL;
}

IExecutable* LoggedInSpace::getExecutor(const char *buffer) const
{
	assert(buffer != NULL);

	std::string command = getCommandSection(buffer);
	if (command == "PASS")
	{
		return new PassCommand();
	}
	else if (command == "USER")
	{
		return new UserCommand();
	}
	else if (command == "NICK")
	{
		return new NickCommand();
	}
	else if (command == "QUIT")
	{
		return new QuitCommand();
	}
	return NULL;
}

// before erase, ++  
void LoggedInSpace::admitOrExile(Server& server)
{
	std::map<int, LoginInfo>::iterator it = mInfos.begin();
	while (it != mInfos.end())
	{
		LoginInfo info = it->second;
		if (info.isValidInfo())
		{
			int clientSocket = it->first;
			++it;
			User* user = exitUserOrNull(clientSocket);
			user->setNickname(info.getNickname());
			user->setUsername(info.getUsername());
			server.loginToServer(clientSocket, user);
		}
		else if (info.isTimeout())
		{
			int clientSocket = it->first;
			++it;
			server.leaveServer(clientSocket);
		}
		else
		{
			++it;
		}
	}
}

bool LoggedInSpace::enterUser(const int clientSocket, User* user)
{
	bool bIsSucceed = Space::enterUser(clientSocket, user);
	assert(bIsSucceed);
	LoginInfo info;
	std::pair<int, LoginInfo> pair(clientSocket, info);
	mInfos.insert(pair);
	return true;
}

User* LoggedInSpace::exitUserOrNull(const int clientSocket)
{
	mInfos.erase(clientSocket);
	return Space::exitUserOrNull(clientSocket);
}

bool LoggedInSpace::trySetAuthenticated(const int clientSocket)
{
	std::map<int, LoginInfo>::iterator it = mInfos.find(clientSocket);
	if (it != mInfos.end())
	{
		it->second.setAthenticated();
		return true;
	}
	return false;
}

bool LoggedInSpace::trySetNickname(const int clientSocket, const std::string& nickname)
{
	std::map<int, LoginInfo>::iterator it = mInfos.find(clientSocket);
	if (it != mInfos.end())
	{
		it->second.setNickname(nickname);
		return true;
	}
	return false;
}

bool LoggedInSpace::trySetUsername(const int clientSocket, const std::string& username)
{
	std::map<int, LoginInfo>::iterator it = mInfos.find(clientSocket);
	if (it != mInfos.end())
	{
		it->second.setUsername(username);
		return true;
	}
	return false;
}