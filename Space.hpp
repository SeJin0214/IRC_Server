/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Space.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:50:56 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/27 15:07:15 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <map>
#include <vector>
#include "User.hpp"

class Space
{
public:
	virtual ~Space();
	virtual std::string getCommandList() const = 0;
	std::string getHelpMessage() const;
	std::string getCommonCommandList() const;
	std::vector<int> getFdSet() const;
	bool enterUser(int clientSocket, User& user);
	void exitUser(int clientSocket);
	
	
protected:
	std::map<int, User> mUsers;
};
