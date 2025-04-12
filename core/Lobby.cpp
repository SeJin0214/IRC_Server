/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lobby.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 10:48:49 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/08 20:33:37 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <cstring>
#include <string>
#include "Lobby.hpp"
#include "ErrorCommand.hpp"

Lobby::~Lobby()
{
	
}

IOutgoingMessageProvider* Lobby::getOutgoingMessageProvider(const char* buffer) const
{
	assert(buffer != NULL);
	IOutgoingMessageProvider* provider = Space::getOutgoingMessageProvider(buffer);
	if (provider != NULL)
	{
		return provider;
	}
	return new ErrorCommand();
}

IExecutable* Lobby::getExecutor(const char* buffer) const
{
	assert(buffer != NULL);
	IExecutable* executor = Space::getExecutor(buffer);
	if (executor != NULL)
	{
		return executor;
	}
	return NULL;

}