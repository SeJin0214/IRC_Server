/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:01:19 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/12 15:04:23 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "IOutgoingMessageProvider.hpp"

class ListCommand : public IOutgoingMessageProvider
{

public:
	MessageBetch getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const;

};