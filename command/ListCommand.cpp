/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:01:29 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/12 17:22:52 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <cstring>
#include <sstream>
#include "ListCommand.hpp"

MessageBetch ListCommand::getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const
{
    assert(buffer != NULL);
    assert(std::strncmp(buffer, "LIST", std::strlen("LIST")) == 0);

    std::string serverName = server.getServerName();
    Result<User> user = server.findUser(clientSocket);
    std::string nickname = user.getValue().getNickname();
    std::stringstream ss;
    ss << ":" << serverName << " 321 " << nickname << " Channel :Users Name\n";
    
    std::vector<const Channel*> channels = server.loadChannels();
    
    for (size_t i = 0; i < channels.size(); ++i)
    {
        int userCount = channels[i]->getUserCount();
        std::string title = channels[i]->getTitle();
        ss << ":" << serverName << " 322 " << nickname << " #" << title << " " << userCount << " :[]\n";
    }
    ss << ":" << serverName << " 323 " << nickname << " :End of channel list.";

    MessageBetch messageBetch;
    messageBetch.addMessage(clientSocket, ss.str());
    return messageBetch;
}
