/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 18:44:17 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/10 11:56:09 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <cstring>
#include "Util.hpp"
#include "PassCommand.hpp"

void PassCommand::execute(Server& server, const int clientSocket, const char* buffer)
{
	assert(buffer != NULL);
	assert(std::strncmp(buffer, "PASS ", std::strlen("PASS ")) == 0);

	const char* password = buffer + std::strlen("PASS ");
	
	if (Util::generateHash65599(password) == server.getPassword())
	{
		server.trySetAuthenticatedInLoggedSpace(clientSocket);
	}
}
