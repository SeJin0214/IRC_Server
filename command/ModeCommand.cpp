/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:37:54 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/12 17:17:40 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include "ModeCommand.hpp"
#include <stdlib.h>

MessageBetch ModeCommand::getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const
{
	std::cout << "mode : " << buffer << std::endl;
	assert(buffer != NULL);
	MessageBetch msg;
	// CommonCommand commoncommand;

	// MODE #channel + - 
	// std::cout << "클라->서버 : " << buffer << std::endl;
	User user = server.findUser(clientSocket).getValue();
	std::string nickname = user.getNickname();

	std::stringstream ss (buffer);
	std::string temp, channelName;
	ss >> temp >> channelName;
	if (channelName[0] != '#')
	{
		std::string errMsg = "Invalid format.";
		msg.addMessage(clientSocket, errMsg);
		return msg;
	}
	channelName.erase(0, 1);
	Channel *channel = server.findChannelOrNull(channelName);
	if (channel == NULL)
	{
		std::string errMsg = "Invalid format.";
		msg.addMessage(clientSocket, errMsg);
		return msg;
	}
	if (ss.eof())
	{
		std::stringstream ret;
		ret << ":irc.local 324 " << nickname << " #" << channelName << " :" << channel->getActiveMode() << "\r\n";
		ret << ":irc.local 329 " << nickname << " #" << channelName << " :1743734234";//시간체크
		msg.addMessage(clientSocket, ret.str());
		return (msg);
	} 

	std::string bMode;
	ss >> bMode;
	std::stringstream clientMsg;
	if (bMode.compare("b") == 0)
	{
		clientMsg << ":irc.local 368 " << nickname << " " << channelName << " :End of channel ban list.";
		msg.addMessage(clientSocket, clientMsg.str());
        return (msg);
	}
	if (bMode.size() != 2)
	{
		for (size_t i = 0; i < bMode.size(); ++i)
		{
			clientMsg << ":irc.local 472 " << nickname << " " << bMode[i] << " :is not a recognised channel mode.";
			if (bMode.size() - 1 > i)
			{
				clientMsg << "\r\n";
			}
		}
		msg.addMessage(clientSocket, clientMsg.str());
		return (msg);
	}

	char sign = bMode[0];
	std::vector<int> userSockets = channel->getFdSet();
	
	if (sign != '-' && sign != '+' && bMode[1] != 'i' && bMode[1] != 'k' && bMode[1] != 'l' && bMode[1] != 'o' && bMode[1] != 't')
	{
		for (size_t i = 0; i < bMode.size(); ++i)
		{
			clientMsg << ":irc.local 472 " << nickname << " " << bMode[i] << " :is not a recognised channel mode.";
			if (bMode.size() - 1 > i)
			{
                clientMsg << "\r\n";
            }
		}
		msg.addMessage(clientSocket, clientMsg.str());
		return (msg);
	}

	std::stringstream successMsg;
	std::stringstream errorMsg;

	if (channel->isOperator(clientSocket) == false)
	{
		errorMsg << server.getServerName() << " 482 " << nickname << " #" << channelName << " :You must be a channel op or higher to mode a more privileged user.";
		msg.addMessage(clientSocket, errorMsg.str());
		return (msg);
	}

	if (bMode[1] == 'i')
	{
		if (channel->isOperator(clientSocket) == false)
		{
			errorMsg << server.getServerName() << " 482 " << nickname << " #" << channelName << " :You must be a channel op or higher to set channel mode i (inviteonly).";
			msg.addMessage(clientSocket, errorMsg.str());
			return (msg);
		}
	}
	else if (bMode[1] == 'k')
	{
		if (channel->isOperator(clientSocket) == false)
		{
			errorMsg << server.getServerName() << " 482 " << nickname << " #" << channelName << " :You must be a channel op or higher to unset channel mode k (key).";
			msg.addMessage(clientSocket, errorMsg.str());
			return (msg);
		}
		std::string password;
		if (!(ss >> password))
		{
			errorMsg << server.getServerName() << " 696 " << nickname << " #" << channelName << " k * :You must specify a parameter for the key mode. Syntax: <key>.";
            msg.addMessage(clientSocket, errorMsg.str());
            return (msg);
		}
		if (sign == '-' && channel->isModeActive(MODE_KEY_LIMIT))
		{
			if (!channel->isPassword(password))
			{
				errorMsg << server.getServerName() << " 467 " << nickname << " #" << channelName << " :Channel key already set";
				msg.addMessage(clientSocket, errorMsg.str());
				return (msg);
			}
		}
		bMode += " :" + password;
	}
	else if (bMode[1] == 'l')
	{
		unsigned int memberCount;
		if (!(ss >> memberCount))
		{
			errorMsg << server.getServerName() << " 696 " << nickname << " #" << channelName << " l * :You must specify a parameter for the limit mode. Syntax: <limit>.";
            msg.addMessage(clientSocket, errorMsg.str());
            return (msg);
		}
		if (channel->isOperator(clientSocket) == false)
		{
			errorMsg << server.getServerName() << " 482 " << nickname << " #" << channelName << " :You must be a channel op or higher to set channel mode l (limit).";
			msg.addMessage(clientSocket, errorMsg.str());
			return (msg);
		}
		std::stringstream num;
		num << memberCount;
		bMode += " :" + num.str();
	}
	else if (bMode[1] == 'o')
	{
		std::string username;
		if (!(ss >> username))
		{
			errorMsg << server.getServerName() << " 696 " << username << " #" << channelName << " " << "o * :You must specify a parameter for the op mode. Syntax: <nick>.";
            msg.addMessage(clientSocket, errorMsg.str());
            return (msg);
		}
		if (channel->isOperator(clientSocket) == false)
		{
			errorMsg << server.getServerName() << " 482 " << nickname << " #" << channelName << " :You must be a channel op or higher to set channel mode o (op).";
			msg.addMessage(clientSocket, errorMsg.str());
			return (msg);
		}
		if (channel->isAddUserAsAdmin(username) == false) 
		{
			errorMsg << server.getServerName() << " 401 " << nickname << " " << username << " :No such nick.";
            msg.addMessage(clientSocket, errorMsg.str());
            return (msg);
		}
		bMode += " :" + username;
	}
	else if (bMode[1] == 't')
	{
		if (channel->isOperator(clientSocket) == false)
		{
			errorMsg << server.getServerName() << " 482 " << nickname << " #" << channelName << " :You must be a channel op or higher to set channel mode t (topiclock).";
			msg.addMessage(clientSocket, errorMsg.str());
			return (msg);
		}
	}
	successMsg << server.getServerName() << " MODE #" << channelName << " " << bMode;
	msg.addMessage(clientSocket, successMsg.str());
	for (size_t i = 0; i < userSockets.size(); ++i)
	{
		if (userSockets[i] != clientSocket)
		{
			msg.addMessage(userSockets[i], successMsg.str());
		}
	}
	return msg;
}

void ModeCommand::execute(Server& server, const int clientSocket, const char* buffer)
{
	assert(buffer != NULL);
	//:irc.local MODE #a +i
	std::stringstream ss(buffer);
	std::string temp;
	std::string channelName;
	std::string mode;
	ss >> temp >> channelName >> mode;
	channelName.erase(0, 1);
	Channel *channel = server.findChannelOrNull(channelName);
	User user = server.findUser(clientSocket).getValue();

	if (mode.size() != 2 || channel == NULL)
	{
		return ;
	}
	else if (user.getLastJoinedChannel().getValue() != channelName)
	{
		return ;
	}
	else if (channel->isOperator(clientSocket) == false)
	{
		return ;
	}

	if (mode[0] == '+')
	{
		if (mode[1] == 'i')
		{
			channel->onMode(clientSocket, MODE_INVITE_ONLY);
		}
		else if (mode[1] == 'k')
		{
			std::string password;
			ss >> password;
			channel->onMode(clientSocket, MODE_KEY_LIMIT);
			channel->setPassword(password);
		}
		else if (mode[1] == 'l')
		{
			size_t maxMemberCounnt;
			ss >> maxMemberCounnt;
			channel->onMode(clientSocket, MODE_LIMIT_USER);
			channel->setMaxMemberCount(maxMemberCounnt);
		}
		else if (mode[1] == 'o')
		{
			std::string nickname;
			ss >> nickname;
			channel->isAddUserAsAdmin(nickname);
		}
		else if (mode[1] == 't')
		{
			std::string topic;
			ss >> topic;
			channel->onMode(clientSocket, MODE_TOPIC_LOCK);
			channel->setTopic(clientSocket, topic);
		}
	}
	else if (mode[0] == '-')
	{
		if (mode[1] == 'i')
		{
			channel->offMode(clientSocket, MODE_INVITE_ONLY);
		}
		else if (mode[1] == 'k')
		{
			std::string password;
			ss >> password;
			if (channel->isPassword(password))
			{
				channel->offMode(clientSocket, MODE_KEY_LIMIT);
			}
		}
		else if (mode[1] == 'l')
		{
			channel->offMode(clientSocket, MODE_LIMIT_USER);
		}
		else if (mode[1] == 'o')
		{
			std::string nickname;
			ss >> nickname;
			channel->removeOperatorNicknames(nickname);
		}
		else if (mode[1] == 't')
		{
			channel->offMode(clientSocket, MODE_TOPIC_LOCK);
		}
	}
}