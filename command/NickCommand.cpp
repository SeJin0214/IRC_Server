/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:36:00 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/12 17:14:34 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <cstddef>
#include <cstring>
#include "NickCommand.hpp"
#include "Parser.hpp"

MessageBetch NickCommand::getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const
{
	assert(buffer != NULL);
	assert(std::strncmp(buffer, "NICK ", std::strlen("NICK ")) == 0);
	
	std::stringstream message;
	MessageBetch messageBetch;
	
	const char* nickname = buffer + std::strlen("NICK ");
	size_t nicknameLength = std::strlen(nickname);
	if (nicknameLength == 0)
	{
		message << ":" << server.getServerName() << " 431 * " << nickname << " :No nickname given";
		messageBetch.addMessage(clientSocket, message.str());
	}
	else if (Parser::isInvalidNameFormatted(nickname))
	{
		message << ":" << server.getServerName() << " 432 * " << nickname << " :Erroneous nickname";
		messageBetch.addMessage(clientSocket, message.str());
	}
	else if (server.isDuplicatedNickname(nickname))
	{
		message << ":" << server.getServerName() << " 433 * " << nickname << " :Nickname is already in use.";
		messageBetch.addMessage(clientSocket, message.str());
	}
	return messageBetch;
}

void NickCommand::execute(Server& server, const int clientSocket, const char* buffer)
{
	assert(buffer != NULL);
	assert(std::strncmp(buffer, "NICK ", std::strlen("NICK ")) == 0);

	const char* nickname = buffer + std::strlen("NICK ");
	
	size_t nicknameLength = std::strlen(nickname);
	if (nicknameLength == 0 || Parser::isInvalidNameFormatted(nickname) || server.isDuplicatedNickname(nickname))
	{
		return;	
	}
	server.trySetNicknameInLoggedSpace(clientSocket, nickname);
}
