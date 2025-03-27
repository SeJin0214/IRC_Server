/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:13:06 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/26 20:42:40 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(const std::string &username, const std::string &nickname) 
: mUsername(username)
, mNickname(nickname)
{
	
}

bool User::operator==(const User& rhs) const
{
	return mUsername == rhs.mUsername;
}

User::~User()
{

}

std::string User::getUsername() const
{ 
	return mUsername;
}

std::string User::getNickname() const
{ 
	return mNickname;
}

void User::setNickname(std::string& nickname)
{
	mNickname = nickname;
}