/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:01:53 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/11 13:34:37 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include "WhoCommand.hpp"

MessageBetch WhoCommand::getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const
{
	assert(buffer != NULL);

	MessageBetch msg;

	std::string buf(buffer);
	User user = server.findUser(clientSocket).getValue();
	std::string nickname = user.getNickname();
	int idxOfGuestNick = buf.find_first_of(" ", 4);
      // who 닉네임 %tna,745 / who 채널 %tcuhnfdar,743
	if (buf.find ("%tna,745"))
	{
		std::string guestNick = buf.substr(4, idxOfGuestNick);
		msg.addMessage(clientSocket, ":" + server.getServerName() + " 354 " + nickname + " 745 " + guestNick + " :0");
		msg.addMessage(clientSocket, ":" + server.getServerName() + " 315 " + nickname + " " + guestNick + " :End of /WHO list.");
		return msg;
	}
	else if (buf.find("%tcuhnfdar"))
	{
		assert(user.getLastJoinedChannel().hasSucceeded());
		Channel* currentChannel = server.findChannelOrNull(user.getLastJoinedChannel().getValue());
		std::vector<std::string> nick = currentChannel->getNicknames();
		
		std::string channelName = buf.substr(4, idxOfGuestNick);
		std::stringstream ret;
		for (size_t i = 0; i < nick.size(); ++i)
		{
			const std::pair<int, User*> pair = currentChannel->findUser(nick[i]).getValue();
			User user = *(pair.second);
			int usersocket = pair.first;

			ret << ":" << server.getServerName() << " 354 " << nickname << " 743 " << channelName << user.getUsername() \
			<< " " << CommonCommand::getPrefixMessage(user, usersocket) << " " << nick[i] << " " << "H";
			if (currentChannel->isOperator(server.findUser(nick[i]).getValue().first))
			{
				ret << "@";
			}
			ret << " 0 0 :" << user.getUsername();
			if (nick.size() - 1 != i)
			{
				ret << "\r\n";
			}
		}
		ret << ":" << server.getServerName() << " 315 " << nickname << " " << channelName << " :End of /WHO list.";
		msg.addMessage (clientSocket, ret.str());
	}
	return msg;
}