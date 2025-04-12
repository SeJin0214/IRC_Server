/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:40:16 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/08 19:24:44 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "IExecutable.hpp"
#include "IOutgoingMessageProvider.hpp"

class TopicCommand : public IOutgoingMessageProvider, public IExecutable
{
public:
    MessageBetch getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const;
    void execute(Server& server, const int clientSocket, const char* buffer);
    
};