/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:08:44 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/31 17:08:44 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "IOutgoingMessageProvider.hpp"
#include "IExecutable.hpp"

class KickCommand : public IOutgoingMessageProvider, public IExecutable
{
public:
	MessageBetch getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const;
    void execute(Server& server, const int clientSocket, const char* buffer);
};