/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:31:28 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/01 18:31:28 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include "InviteCommand.hpp"
#include "Server.hpp"
#include <sstream>

MessageBetch InviteCommand::getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const
{
	assert(buffer != NULL);

	// INVITE donkim3 #channel
	// INVITE donkim3 donjeong

	std::string guestNick, guestNick2, channelName;
	MessageBetch msg;
	CommonCommand commonCommand;
	std::stringstream ss(buffer);
	std::string temp;

	ss >> temp >> guestNick >> channelName;

	const User hostUser = server.findUser(clientSocket).getValue();
	if (channelName[0] != '#')
	{
		guestNick2 = channelName;
		ss >> channelName;
		msg.addMessage(clientSocket, ":irc.local 403 " + hostUser.getNickname() + " " + guestNick2 + " :No such channel\r\n");
		return msg;
	}

	User user = server.findUser(clientSocket).getValue();
	Channel *channel = server.findChannelOrNull(user.getLastJoinedChannel().getValue());
	
	if (channel == NULL)
	{
		std::string errMsg = "Invalid format.";
		msg.addMessage(clientSocket, errMsg);
		return msg;
	}

	Result<std::pair<int, User *> > guestInChannelPack = channel->findUser(guestNick);
	if (server.findChannelOrNull(user.getLastJoinedChannel().getValue())->isOperator(clientSocket) == false)
	{
		// 권한 없음
	// INVITE sejjeong #channel
		// :irc.local 482 donjeong #channel :You must be a channel op or higher to send an invite.
		msg.addMessage(clientSocket, ":irc.local 482 " + hostUser.getNickname() + " " + channelName + "  :You must be a channel op or higher to send an invite.\r\n");
		return msg;
	}
	else if (server.findUser(guestNick).hasSucceeded() == false)// Nickname이 없음;
	{
		// 127.000.000.001.58398-127.000.000.001.06667: INVITE asdf #channel
		// 127.000.000.001.06667-127.000.000.001.58398: :irc.local 401 donkim3 asdf :No such nick
		msg.addMessage (clientSocket, ":irc.local 401 " + hostUser.getNickname() + " " + guestNick + " :No such nick\r\n");
		return msg;
	}
	else if (guestInChannelPack.hasSucceeded() == true) // 채널가입자에 있으면    -> 채널안에 들어가있으면
	{
		//  채널에 있는 donkim3 초대
	// INVITE donkim3 #channel
	// :irc.local 443 donkim3 donkim3 #channel :is already on channel
		msg.addMessage(clientSocket, ":irc.local 443 " + guestNick + " " + guestNick + " " + channelName + "  :is already on channel\r\n");
		return msg;
	}
	else
	{
 		// INVITE sejjeong #channel
		// :irc.local 341 donkim3 sejjeong :#channel   나 
		// :donkim3!root@127.0.0.1 INVITE sejjeong :#channel   게스트
 
		//성공
		int userSocket = server.findUser(guestNick).getValue().first;
		msg.addMessage(clientSocket, ":irc.local 341 " + hostUser.getNickname() + " " + guestNick + " :" + channelName + "\r\n");
		msg.addMessage(userSocket, commonCommand.getPrefixMessage(hostUser, clientSocket) + " INVITE " + guestNick + " :" + channelName + "\r\n");
		return msg;
	}
	return msg;
}

void InviteCommand::execute(Server& server, const int clientSocket, const char* buffer)
{
	assert(buffer != NULL);
	std::string guest;
	std::string channelName;
	std::string temp;

//  INVITE sejjeong #channel
//	:irc.local 341 donkim3 sejjeong :#channel
//  :donkim3!root@127.0.0.1 INVITE sejjeong :#channel
	std::stringstream ss(buffer);
	ss >> temp >> guest >> channelName;
	if (channelName[0] != '#')
	{
		return ;
	}
	User invitedUser = server.findUser(guest).getValue().second; // 초대받은 유저
	std::string currentChannelName = server.findUser(clientSocket).getValue().getLastJoinedChannel().getValue();   //그 유저(소켓)의 마지막 채널
	Channel *channel = server.findChannelOrNull(currentChannelName);
	if (channel == NULL)
	{
		return ;
	}
	std::string nick(invitedUser.getNickname());
	channel->enterInvitedList(nick);
}