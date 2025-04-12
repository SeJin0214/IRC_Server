/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendChannelMessageCommand.hpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:23:53 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/05 13:37:44 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "IOutgoingMessageProvider.hpp"

class SendChannelMessageCommand : public IOutgoingMessageProvider
{

public:
    MessageBetch getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const;
};