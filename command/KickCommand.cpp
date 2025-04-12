/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:09:01 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/31 17:09:01 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <cstring>
#include <sstream>
#include "KickCommand.hpp"

MessageBetch KickCommand::getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const
{
	assert(buffer != NULL);
	MessageBetch msg;

	Result<User> resultUser = server.findUser(clientSocket);
	User clientUser = resultUser.getValue();
	std::string temp;
	std::string channelName;
	std::string kickedName;
	std::string kickMsg;
	std::stringstream ss(buffer);
	std::stringstream ret;
	ss >> temp >> channelName;
	if (channelName[0] != '#')
	{
		std::string errMsg = "Invalid format.";
		msg.addMessage(clientSocket, errMsg);
		return msg;
	}
	if (!(ss >> kickedName))
	{
		std::string errMsg = "Invalid format.";
		msg.addMessage(clientSocket, errMsg);
		return msg;
	}
	channelName.erase(0, 1);
	getline(ss, kickMsg);
	Channel* channel = server.findChannelOrNull(channelName);
	assert(channel != NULL);
	std::cout << "name :" << kickedName << std::endl;
	if (server.findUser(kickedName).hasSucceeded() == false)
	{
		//존재하지 않는 유저
		//  KICK #a donf :
		// :irc.local 401 donkim3 donf :No such nick
		ret << server.getServerName() << " 401 " << clientUser.getNickname() << " " << kickedName << " :No such nick";
		msg.addMessage(clientSocket, ret.str());
	}
	else if (channel->isOperator(clientSocket) == false)
	{
		//권한 (donjeong -> donkim3(방장))
		// KICK #a donkim3 :
		// :irc.local 482 sejjeong #a :You must be a channel op or higher to kick a more privileged user.
		ret << server.getServerName() << " 482 " << clientUser.getNickname() << " #" << channelName << " :You must be a channel op or higher to kick a more privileged user.";
		msg.addMessage(clientSocket, ret.str());
	}
	else if (channel->findUser(kickedName).hasSucceeded() == false)
	{
		// 유저 전체에는 존재하나 -> 현재 채널에는 없는 경우
		// KICK #a sejjeong :#a hello
		// :irc.local 441 donkim3 sejjeong #a :They are not on that channel
		ret << server.getServerName() << " 441 " << clientUser.getNickname() << " " << kickedName << " #" << channelName << " :They are not on that channel";
		msg.addMessage(clientSocket, ret.str());
	}
	else
	{
	////성공 //// 자기 자신 강퇴 가능 서버 ->클라 : :donkim3!donkim3@127.0.0.1a KICK #a sejjeong :
	// 방장 <donkim3>
	// KICK #a donjeong :sejjeong       | << donjeong만 강퇴
// :donkim3!root@127.0.0.1 KICK #a donjeong :sejjeong
// :donkim3!root@127.0.0.1 KICK #a donjeong :sejjeong
// :donkim3!root@127.0.0.1 KICK #a donjeong :sejjeong
	std::string leaveMessage = CommonCommand::getPrefixMessage(clientUser, clientSocket) + " KICK #" + channelName + " " + kickedName;
	std::vector<int> namesInChannel = channel->getFdSet();
	for (size_t i = 0; i < namesInChannel.size(); ++i)
	{
		ret << leaveMessage << kickMsg;
		if (i + 1 < namesInChannel.size())
			ret << "\r\n";
		msg.addMessage(namesInChannel[i], ret.str());
	}
	}
	return msg;
}

void KickCommand::execute(Server& server, const int clientSocket, const char* buffer)
{
	assert(buffer != NULL);

	std::string temp;
	std::string channelName;
	std::string kickedName;   // 차이는 유저인데
	std::stringstream ss(buffer);
	ss >> temp >> channelName >> kickedName;
	channelName.erase(0, 1);

	Channel* channel = server.findChannelOrNull(channelName);
	if (channel == NULL)
	{
		return ;
	}
	Result<std::pair<int, User*> > result = channel->findUser(kickedName);
	std::pair<int, User *> kickedUserPack = result.getValue();	
	if (result.hasSucceeded() == false 
	|| channel->isOperator(clientSocket) == false)
	{
		return ;
	}
	
	size_t channelIndexInKickedUser = kickedUserPack.second->getIndexOfJoinedChannel(channelName).getValue();
	// User가 가지고있는 채널들에 추방 당한 channelName 인덱스 확인

	for (size_t i = 0; i <= channelIndexInKickedUser; ++i)
	{
		std::string exitChannelName = kickedUserPack.second->getJoinedChannelName(i);
		server.exitUserInChannel(kickedUserPack.first, exitChannelName);
		kickedUserPack.second->removeJoinedChannel(exitChannelName);
	}
}
