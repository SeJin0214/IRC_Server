/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:20:19 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/27 15:21:34 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "Channel.hpp"
#include "Util.hpp"
#include "Result.hpp"

Channel::Channel(const std::string& title, const char* password) 
: mTitle(title)
, mPassword(Util::generateHash65599(password))
{
	
}

Channel::~Channel()
{

}

bool Channel::operator<(const Channel& rhs)
{
	return mTitle < rhs.mTitle;
}

/* getter */
std::string Channel::getTitle() const
{
	return mTitle;
}

unsigned int Channel::getPassword() const
{
	return mPassword;
}

std::vector<std::string> Channel::getNicknames() const
{
	std::vector<std::string> result;
	std::map<int, User>::const_iterator it = mUsers.begin();
	while (it != mUsers.end())
	{
		result.push_back(it->second.getNickname());
		++it;
	}
	return result;
}
	
std::vector<std::string> Channel::getUsernames() const
{
	std::vector<std::string> result;
	std::map<int, User>::const_iterator it = mUsers.begin();
	while (it != mUsers.end())
	{
		result.push_back(it->second.getUsername());
		++it;
	}
	return result;
}

std::vector<int> Channel::getClientSockets() const
{
	std::vector<int> result;
	std::map<int, User>::const_iterator it = mUsers.begin();
	while (it != mUsers.end())
	{
		result.push_back(it->first);
		++it;
	}
	return result;
}

std::string Channel::getCommandList() const
{
	return "";
}

/* setter */
bool Channel::setTitle(const int clientSocket, std::string& title)
{
	if (isOperator(clientSocket) == false)
	{
		return false;
	}
	mTitle = title;
	return true;
}

bool Channel::toggleMode(User& user, const eMode mode)
{
	if (isOperator(user) == false)
	{
		return false;
	}
	const unsigned char bitFlag = 1 << mode;
	mModeFlag ^= bitFlag;
	return true;
}

bool Channel::isOperator(const User& user) const
{
	for (std::map<int, User>::const_iterator it = mUsers.begin(); it != mUsers.end(); ++it)
	{
		if (it->second == user)
		{
			return true;
		}
	}
	return false;
}

bool Channel::isOperator(const int userSocket) const
{
	for (std::map<int, User>::const_iterator it = mUsers.begin(); it != mUsers.end(); ++it)
	{
		if (it->first == userSocket)
		{
			return true;
		}
	}
	return false;
}

Result<User> Channel::findUser(const int clientSocket) const
{
	std::map<int, User>::const_iterator it = mUsers.find(clientSocket);
	if (it != mUsers.end())
	{
		Result<User> result(it->second, true);
		return result;
	}
	else
	{
		Result<User> result(it->second, false);
		return result;	
	}
}