/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectMessageCommand.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:22:22 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/08 16:37:50 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <cstring>
#include "DirectMessageCommand.hpp"
#include "CommonCommand.hpp"
#include <iostream>
#include <sstream>
#include "User.hpp"

MessageBetch DirectMessageCommand::getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const
{
	assert(buffer != NULL);
	assert(std::strcmp(buffer, "") != 0);
	std::map<int, std::string> socketAndMessage;
	MessageBetch retMsg;

	// 127.000.000.001.58178-127.000.000.001.06667: PRIVMSG #aaa :hello@######@@world
	// 127.000.000.001.06667-127.000.000.001.58162: :sejjeong!root@127.0.0.1 PRIVMSG #aaa :hello@######@@world
    //        aa,bb,cc,dd

	std::stringstream si(buffer);

	std::string pri;
	std::string usernames;
	si >> pri >> usernames;
	std::string msg;
	getline(si, msg);
	
	std::cout << pri << " " << usernames << " " << std::endl;	
	std::stringstream ss(usernames);
	std::string temp;
	std::stringstream result;
	while (true)
	{
		std::getline(ss, temp, ',');

		Result<std::pair<int, User> > user = server.findUser(temp);
		if(user.hasSucceeded() == true)
		{
			User host = server.findUser(clientSocket).getValue();
			result << CommonCommand::getPrefixMessage(host, clientSocket) << " PRIVMSG " << temp << msg;
			retMsg.addMessage(user.getValue().first, result.str());
		}
		else
		{
			result << ":server 401 " << temp << " :No such nick/channel";
			retMsg.addMessage(clientSocket, result.str());
		}
		if (ss.eof())
		{
			break;
		}
	}
	return retMsg;
}