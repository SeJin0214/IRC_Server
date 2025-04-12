/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:43:26 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/10 15:09:14 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <cstddef>
#include <cstring>
#include <sstream>
#include "JoinCommand.hpp"

 
MessageBetch JoinCommand::getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const
{
	// JoinCommand a;
	// a.execute(const_cast<Server&>(server), clientSocket, buffer);
	assert(buffer != NULL);
	assert(std::strncmp(buffer, "JOIN ", std::strlen("JOIN ")) == 0);
		//채널 30자 이내
	MessageBetch msg;
	User user = server.findUser(clientSocket).getValue();
	std::stringstream ss(buffer);
	std::string join;
	std::string channelName;
	ss >> join >> channelName;
	if (channelName[0] != '#')
	{
		std::string errMsg = "Invalid format.";
		msg.addMessage(clientSocket, errMsg);
		return msg;
	}
	channelName.erase(0,1);
	std::string nickname = user.getNickname();

	Channel *channel = server.findChannelOrNull(channelName);
	std::stringstream userlist;
	if (channel != NULL)
	{
		std::stringstream errorMsg;//비초인
		std::string pass;
		// join 하기 전에 mode 확인 // i 면 list에 있는지 확인하고 메시지 출력
		//:irc.local 475 donkim3 #a :Cannot join channel (incorrect channel key)
		ss >> pass;
		if (channel->isModeActive(MODE_KEY_LIMIT) == true)
		{
			if (channel->isPassword(pass) == false)
			{
				if (channel->isInvited(nickname) == false)
				{
					errorMsg << server.getServerName() << " 475 " << nickname << " #" << channelName << " :Cannot join channel (incorrect channel key)";
					msg.addMessage(clientSocket, errorMsg.str());
					return msg;
				}
			}
		}
		if (channel->isModeActive(MODE_INVITE_ONLY) == true && channel->isInvited(nickname) == false)
		{//:irc.local 473 donkim3_ #a :Cannot join channel (invite only)
			errorMsg << server.getServerName() << " 473 " << nickname << " #" << channelName << " :Cannot join channel (invite only)";
			msg.addMessage(clientSocket, errorMsg.str());
			return msg;
		}
		if (channel->isModeActive(MODE_LIMIT_USER) == true)
		{
			if (channel->getUserCount() != channel->getMemberCount())
			{
				errorMsg << server.getServerName() << " 471 " << nickname << " #" << channelName << " :Cannot join channel (channel is full)";
				msg.addMessage(clientSocket, errorMsg.str());
				return msg;
			}
		}
		std::vector<std::string> nicknames = channel->getNicknames();
		std::vector<int> userSockets = channel->getFdSet();
		for (size_t i = 0; i < nicknames.size(); ++i)
		{
			if (channel->isOperator(userSockets[i]))
			{
				userlist << "@";
			}
			userlist << nicknames[i];
			if (i != nicknames.size())
			{
				userlist << ' ';
			}
		}
		for (size_t i = 0; i < nicknames.size(); ++i)
		{
			if (nickname != nicknames[i])
			{
				msg.addMessage(server.findUser(nicknames[i]).getValue().first, CommonCommand::getPrefixMessage(user, clientSocket) + " " + join + " #" + channelName + "\r\n");
			}
		}
	}
	else
	{
		nickname = "@" + nickname;
		userlist << nickname;
	}

	std::stringstream ret;
	ret << CommonCommand::getPrefixMessage(user, clientSocket) << " " << join << " #" << channelName << "\r\n" 
	<< server.getServerName() << " 353 " << nickname << " = #" << channelName << " :" << userlist.str() << "\r\n" 
	<< server.getServerName() << " 366 " << nickname << " #" << channelName << " :End of /NAMES list.";
	
	msg.addMessage(clientSocket, ret.str());

	return msg;
}

void JoinCommand::execute(Server& server, const int clientSocket, const char* buffer)
{
	assert(buffer != NULL);
	// JOIN #channel
	std::stringstream buf(buffer);
	std::string channelName;
	std::string password;
	std::string temp;
	buf >> temp >> channelName >> password;
	channelName.erase(0, 1);
	Channel *channel = server.findChannelOrNull(channelName);
	std::string nickname(server.findUser(clientSocket).getValue().getNickname());

	if (channel && channel->isModeActive(MODE_KEY_LIMIT) == true)
	{
		if (channel->isPassword(password) == false)
		{
			if (channel->isInvited(nickname) == false)
			{
				return ;
			}
		}
	}
	if (channel && channel->isModeActive(MODE_LIMIT_USER) == true)
	{
		if (channel->getMemberCount() <= channel->getUserCount())
		{
			return ;
		}
	}
	if (channel && channel->isModeActive(MODE_INVITE_ONLY) == true)
	{
		if (channel->isInvited(nickname) == false)
		{
			return ;
		}
	}
	server.enterUserInChannel(clientSocket, channelName);
}
