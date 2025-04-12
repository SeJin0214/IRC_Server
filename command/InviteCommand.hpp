/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:31:36 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/01 18:31:36 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "IOutgoingMessageProvider.hpp"
#include "IExecutable.hpp"

class InviteCommand : public IOutgoingMessageProvider, public IExecutable
{

public:
    MessageBetch getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const;
    void execute(Server& server, const int clientSocket, const char* buffer);
};