/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:00:57 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/05 13:37:47 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "IOutgoingMessageProvider.hpp"

class WhoCommand : public IOutgoingMessageProvider
{
public:
 
    MessageBetch getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const;
};