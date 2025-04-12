/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendChannelMessageCommand.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:25:10 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/12 18:09:33 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <cstring>
#include "SendChannelMessageCommand.hpp"

 
MessageBetch SendChannelMessageCommand::getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const
{
	assert(buffer != NULL);
	assert(std::strcmp(buffer, "") != 0);
	std::map<int, std::string> socketAndMessage;
	MessageBetch retMsg;

	// PRIVMSG #aaa :asdfasdf
	// donkim!root@127.0.0.1 PRIVMSG #aaa :asdfasdf
	std::string str(buffer);
	std::stringstream ss(str);
	std::string temp;
	std::string channelName;
	// ss >> temp >> channelName;
	getline(ss, temp, ' ');
	getline(ss, channelName, ' ');
	std::string msg;
	getline(ss, msg);
	if (channelName[0] != '#')
	{
		std::string errMsg = "Invalid format.";
		retMsg.addMessage(clientSocket, errMsg);
		return retMsg;
	}
	channelName.erase(0, 1);

	std::stringstream ret;
	User clientUser = server.findUser(clientSocket).getValue();
	std::vector<std::string> userChannels = clientUser.getJoinedChannels();
	if (userChannels.size() == 0)
	{
		ret << server.getServerName() << " 404 " << clientUser.getNickname() << " #" << channelName << " :You cannot send external messages to this channel whilst the +n (noextmsg) mode is set.";
		retMsg.addMessage(clientSocket, ret.str());
		return (retMsg);
	}

	// :irc.local 404 donkim3 #a :You cannot send external messages to this channel whilst the +n (noextmsg) mode is set.
	Channel  *channel = server.findChannelOrNull(channelName);
	if (channel == NULL)
	{
		std::string errMsg = "Invalid format.";
		retMsg.addMessage(clientSocket, errMsg);
		return retMsg;
	}
	std::vector<int> userSockets = channel->getFdSet();
	for (size_t i = 0; i < userSockets.size(); ++i)
	{
		if (userSockets[i] == clientSocket)
		{
			continue;
		}
		User user = server.findUser(clientSocket).getValue();
		std::string who = CommonCommand::getPrefixMessage(user, clientSocket);

		std::cout << msg << std::endl;
		
		retMsg.addMessage(userSockets[i], who + " PRIVMSG #" + channelName + " " + msg + "\r\n");
	}
	return retMsg;
}