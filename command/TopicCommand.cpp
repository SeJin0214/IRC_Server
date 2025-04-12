/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:40:36 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/11 13:34:39 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include "TopicCommand.hpp"
#include <sstream>

MessageBetch TopicCommand::getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const
{
	assert(buffer != NULL);
	MessageBetch msg;
	std::string temp;
	std::string channelName;
	std::string topic;

	std::stringstream ss(buffer);
	std::stringstream ret;
	ss >> temp >> channelName >> topic;
	if (channelName[0] != '#')
	{
		std::string errMsg = "Invalid format.";
		msg.addMessage(clientSocket, errMsg);
		return msg;
	}
	channelName.erase(0, 1);
	Channel* channel = server.findChannelOrNull(channelName);
	std::string clientNickname = server.findUser (clientSocket).getValue().getNickname();

	if (channel == NULL) // channel, lobby 공용;
	{
			// 유효하지 않은 채널입력했을 경우
		//  TOPIC #invalidchannel
		// :irc.local 403 donkim3 #invalidchannel :No such channel
		ret << server.getServerName() << " 403 " << clientNickname << " #" << channelName << " :No such channel";
		msg.addMessage(clientSocket, ret.str());
	}
	else if (channel->findUserOrNull(clientSocket) != NULL) // channel 만
	{
		if (channel->isModeActive(MODE_TOPIC_LOCK) == true && channel->isOperator(clientSocket) == false)
		{
			
			// 권한이 없는 경우
			// client -> donjeong
			// TOPIC #a :s
			// :irc.local 482 donjeong #a :You must be a channel op or higher to change the topic.
			ret << server.getServerName() << " 482 " << clientNickname << " #" << channelName << " :You must be a channel op or higher to change the topic.";
			msg.addMessage(clientSocket, ret.str());
		}
		else
		{
			//  <<방장 : donkim3 >>
			// TOPIC #a :helloworld

			// :donkim3!root@127.0.0.1 TOPIC #a :helloworld  /// server -> donkim3 ///
			// :donkim3!root@127.0.0.1 TOPIC #a :helloworld  /// server -> donjeong ///
			// :donkim3!root@127.0.0.1 TOPIC #a :helloworld  /// server -> sejjeong ///
			std::vector<int> namesInChannel = channel->getFdSet();
			for (size_t i = 0; i < namesInChannel.size(); ++i)
			{
				ret << server.getServerName() << " " << buffer;
				msg.addMessage(namesInChannel[i], ret.str());
			}
		}
	}
	else // 로비만
	{
		if (channelName.empty() == true)
		{
		// lobby /topic만 쳤을 경우
			// TOPIC 
			// :irc.local 461 sejjeong TOPIC :Not enough parameters.
			ret << server.getServerName() << " 461 " << clientNickname << " TOPIC :Not enough parameters.";
			msg.addMessage(clientSocket, ret.str());
		}
		else if (topic.empty() == false)
		{
			 // channel 존재 ->  topic이 있는 경우
				// TOPIC #a :asdf
				// :irc.local 442 sejjeong #a :You're not on that channel!
			ret << server.getServerName() << " 442 " << clientNickname << " #" << channelName << " :You're not on that channel!";
			msg.addMessage(clientSocket, ret.str());
		}
			// TOPIC #a
			// :irc.local 332 sejjeong #a :hello
			// :irc.local 333 sejjeong #a donkim3!root@127.0.0.1 :1744009620
		if (channel->getTopic().empty())
		{
			ret << server.getServerName() << " 331 " << clientNickname << " #" << channelName << " :No topic is set.";
		}
		else
		{
			ret << server.getServerName() << " 332 " << clientNickname << " #" << channelName << " :" << channel->getTopic() << "\r\n" \
			<< server.getServerName() << " 333 " << clientNickname << " #" << channelName << " " << CommonCommand::getPrefixMessage(server.findUser(clientSocket).getValue(), clientSocket) << " :1744009620";
		}
		msg.addMessage(clientSocket, ret.str());
	}
	return (msg);
}

// /topic #aaa -> 토픽 확인
// /topic #aaa 

// 채널 topic 에 입장해서 /topic   -> topic -->> 패킷없음
// irssi(채널) -> /topic
// TOPIC #채널
// 

void TopicCommand::execute(Server& server, const int clientSocket, const char* buffer)
{
	assert(buffer != NULL);
	std::string temp;
	std::string channelName;
	std::string topic;
	std::stringstream ss(buffer);

	ss >> temp >> channelName >> topic;
	if (channelName[0] != '#')
	{
		return ;
	}
	channelName.erase(0, 1);
	Channel* channel = server.findChannelOrNull(channelName);
	if (channel == NULL)
	{
		return ;
	}
	User user = server.findUser(clientSocket).getValue();
	if (user.getLastJoinedChannel().getValue() != channelName
		|| (channel->isModeActive(MODE_TOPIC_LOCK) == true && channel->isOperator(clientSocket) == false))
	{
		return ;
	}
	channel->setTopic(clientSocket, topic);
}