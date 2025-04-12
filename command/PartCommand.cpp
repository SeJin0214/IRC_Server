/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:41:48 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/12 17:32:54 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <cstring>
#include "PartCommand.hpp"

MessageBetch PartCommand::getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const
{
	assert(buffer != NULL);
	assert(std::strncmp(buffer, "PART ", std::strlen("PART ")) == 0);
	
	MessageBetch messageBetch;
	
	assert(server.findUser(clientSocket).hasSucceeded());
	User user = server.findUser(clientSocket).getValue();

	char temp[MAX_BUFFER];
	std::strcpy(temp, buffer);

	const char* command = std::strtok(temp, " ");
	(void) command;
 	const char* channelToken = std::strtok(NULL, " ");
	bool bIsParamMissing = channelToken == NULL;
	if (bIsParamMissing)
	{
		std::string needMoreParam = ":" + server.getServerName() + " 461 " + user.getNickname() 
		+ " PART :Not enough parameters"; 
		messageBetch.addMessage(clientSocket, needMoreParam);
		return messageBetch;
	}
	
	const char* channelName = channelToken + 1;
	Channel* channel = server.findChannelOrNull(channelName);
	bool bIsChannel = channelToken[0] == '#';
	if (bIsChannel == false || channel == NULL)
	{
		std::string noSuchChannel = ":" + server.getServerName() + " 403 " + channelToken 
		+ " :No such channel"; 
		messageBetch.addMessage(clientSocket, noSuchChannel);
		return messageBetch;
	}
	else if (user.isInChannel(channelName) == false)
	{
		std::string notOnChannel = ":" + server.getServerName() + " 442 " + user.getNickname() + " " + channelToken 
		+ " :You're not on that channel";
		messageBetch.addMessage(clientSocket, notOnChannel);
		return messageBetch;
	}

	assert(bIsChannel && channel != NULL && user.isInChannel(channelName));

	std::string leaveMessage = CommonCommand::getPrefixMessage(user, clientSocket) + " PART :#" + channelName;
	messageBetch.addMessage(clientSocket, leaveMessage);

	std::vector<int> clientSockets = channel->getFdSet();
	for (size_t i = 0; i < clientSockets.size(); ++i)
	{
		if (clientSockets[i] == clientSocket)
		{
			continue;
		}
		User* userToFind = channel->findUserOrNull(clientSockets[i]);
		assert(userToFind != NULL);
		
		std::string currentChannel = userToFind->getLastJoinedChannel().getValue();
		assert(userToFind->getLastJoinedChannel().hasSucceeded());

		if (currentChannel == channel->getTitle())
		{
			messageBetch.addMessage(clientSockets[i], leaveMessage);
		}
	}
	return messageBetch;
}

void PartCommand::execute(Server& server, const int clientSocket, const char* buffer)
{
	assert(buffer != NULL);
	assert(std::strncmp(buffer, "PART ", std::strlen("PART ")) == 0);
	
	char temp[MAX_BUFFER];
	std::strcpy(temp, buffer);
	
	const char* command = std::strtok(temp, " ");
	(void) command;
 	const char* channelToken = std::strtok(NULL, " ");
	if (channelToken == NULL || channelToken[0] != '#')
	{
		return;
	}
	
	const char* channelName = channelToken + 1;
	server.exitUserInChannel(clientSocket, channelName);
}
