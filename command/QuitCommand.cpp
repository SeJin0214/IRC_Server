/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:05:45 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/12 17:16:22 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <cstring>
#include <unistd.h>
#include <set>
#include "QuitCommand.hpp"
#include "CommonCommand.hpp"

MessageBetch QuitCommand::getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const
{
	assert(buffer != NULL);
	assert(std::strncmp(buffer, "QUIT", std::strlen("QUIT")) == 0);

	MessageBetch messageBetch;
	std::stringstream message;
	message << "ERROR :Closing link: (" << CommonCommand::getHostIP(clientSocket) << ") [" << buffer << "]";

	messageBetch.addMessage(clientSocket, message.str());
	
	Result<User> resultUser = server.findUser(clientSocket);
	if (resultUser.hasSucceeded() == false)
	{
		return messageBetch;
	}

	User user = resultUser.getValue();
	std::stringstream leaveMessage(CommonCommand::getPrefixMessage(user, clientSocket));

	std::set<int> socketsToSend;
	std::vector<std::string> joinedChannels = user.getJoinedChannels();
	for (size_t i = 0; i < joinedChannels.size(); ++i)
	{
		Channel* joinedChannel = server.findChannelOrNull(joinedChannels[i]);
		assert(joinedChannel != NULL);
		std::vector<int> clientSockets = joinedChannel->getFdSet();
		for (size_t i = 0; i < clientSockets.size(); ++i)
		{
			socketsToSend.insert(clientSockets[i]);
		}
	}
	for (std::set<int>::iterator it = socketsToSend.begin(); it != socketsToSend.end(); ++it)
	{
		messageBetch.addMessage(*it, leaveMessage.str());
	}
	return messageBetch;
}

void QuitCommand::execute(Server& server, const int clientSocket, const char* buffer)
{
	assert(buffer != NULL);
	server.exitAllSpaces(clientSocket);
}
