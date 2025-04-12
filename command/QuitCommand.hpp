/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 11:05:11 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/05 13:37:43 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "IExecutable.hpp"
#include "IOutgoingMessageProvider.hpp"

class QuitCommand : public IOutgoingMessageProvider, public IExecutable
{
public:
    MessageBetch getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const;
    void execute(Server& server, const int clientSocket, const char* buffer);
    
};