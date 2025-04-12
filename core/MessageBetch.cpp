/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBetch.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donkim3 <donkim3@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-06 08:30:49 by donkim3           #+#    #+#             */
/*   Updated: 2025-04-06 08:30:49 by donkim3          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageBetch.hpp"

std::vector<std::pair<int, std::string> > MessageBetch::getMessage() const
{
    return (mMsg);
}

void MessageBetch::addMessage(int socket, std::string msg)
{
    std::pair<int, std::string> message(socket, msg);

    mMsg.push_back(message);
}