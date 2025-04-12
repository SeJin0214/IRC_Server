/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:08:38 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/05 13:37:50 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "User.hpp"
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

class CommonCommand
{
public:
	static std::string getHostIP(int sockfd);
	static std::string getPrefixMessage(const User& user, const int clientSocket);
};
