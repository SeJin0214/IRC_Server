/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBetch.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donkim3 <donkim3@student.42.fr>            #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-06 08:30:57 by donkim3           #+#    #+#             */
/*   Updated: 2025-04-06 08:30:57 by donkim3          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <vector>
#include <iostream>

class MessageBetch
{
public :
    std::vector<std::pair<int, std::string> > getMessage() const;
    void addMessage(int socket, std::string msg);
private :
    std::vector <std::pair<int, std::string> > mMsg;
};