/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 17:36:20 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/11 13:36:06 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cassert>
#include <cstring>
#include <cstddef>
#include "UserCommand.hpp"
#include "Server.hpp"
#include "Parser.hpp"

void UserCommand::execute(Server& server, const int clientSocket, const char* buffer)
{
	assert(buffer != NULL);

	char temp[MAX_BUFFER] = { 0, };
	strcpy(temp, buffer);
	size_t bufferLength = std::strlen(temp);
	
	const char* command = std::strtok(temp, " ");
	assert(std::strncmp(command, "USER", std::strlen("USER")) == 0);

	const char* username = std::strtok(NULL, " ");
	const char* hostname = std::strtok(NULL, " ");
	const char* servername = std::strtok(NULL, " ");
	
	if (servername == NULL || hostname == NULL || username == NULL
	|| Parser::isInvalidNameFormatted(username))
	{
		return;
	}
	
	size_t realnameStartIndex = std::strlen(command) + 1 + std::strlen(username) + 1 
	+ std::strlen(hostname) + 1 + std::strlen(servername) + 1;
	bool isNotExistedRealName = bufferLength <= realnameStartIndex;
	if (isNotExistedRealName)
	{
		return;
	}
	const char* realname = temp + realnameStartIndex;
	if (realname[0] != ':')
	{
		return;
	}
	
	server.trySetUsernameInLoggedSpace(clientSocket, username);
}
