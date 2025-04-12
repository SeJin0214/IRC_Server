/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOutgoingMessageProvider.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 10:33:05 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/05 13:37:53 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include "MessageBetch.hpp"
#include "CommonCommand.hpp"
#include "Server.hpp"

class IOutgoingMessageProvider
{
public:
	virtual ~IOutgoingMessageProvider() {};	
	virtual MessageBetch getMessageBetch(const Server& server, const int clientSocket, const char* buffer) const = 0;
 };