/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:53:02 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/12 17:17:17 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <cstring>
#include "ErrorCommand.hpp"

MessageBetch ErrorCommand::getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const
{
	assert(buffer != NULL);
	char temp[MAX_BUFFER] = { 0, };
	std::strcpy(temp, buffer);
	const char* command = std::strtok(temp, " ");
	std::stringstream message;
	
	User user = server.findUser(clientSocket).getValue();
	assert(server.findUser(clientSocket).hasSucceeded());
	
	message << ":" << server.getServerName() << " 421 " << user.getNickname() << " " << command 
	<< " :Unknown command";
	
	MessageBetch messageBetch;
	messageBetch.addMessage(clientSocket, message.str());
	return messageBetch;
}
