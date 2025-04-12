/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:43:06 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/05 13:37:54 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "IExecutable.hpp"
#include "IOutgoingMessageProvider.hpp"

class JoinCommand : public IOutgoingMessageProvider, public IExecutable
{
public:
 
    MessageBetch getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const;
     void execute(Server& server, const int clientSocket, const char* buffer);
};