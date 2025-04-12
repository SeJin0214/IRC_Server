/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:52:10 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/05 13:37:51 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "IOutgoingMessageProvider.hpp"

class ErrorCommand : public IOutgoingMessageProvider
{

public:
    MessageBetch getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const;

};