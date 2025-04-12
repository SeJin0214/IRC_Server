/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoginInfo.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:09:17 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/10 12:04:06 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LoginInfo.hpp"

LoginInfo::LoginInfo()
: mUsername("")
, mNickname("")
, mbAuthenticated(false)
{
	gettimeofday(&mStartTime, NULL);
}

std::string LoginInfo::getUsername() const
{ 
	return mUsername;
}

std::string LoginInfo::getNickname() const
{ 
	return mNickname;
}

bool LoginInfo::hasNickname() const
{
	return mNickname != "";
}

bool LoginInfo::hasUsername() const
{
	return mUsername != "";
}

bool LoginInfo::isPassed() const
{
	return mbAuthenticated;
}

bool LoginInfo::isValidInfo() const
{
	return hasNickname() && hasUsername() && isPassed();
}

bool LoginInfo::isTimeout() const
{
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	double sec = (currentTime.tv_sec - mStartTime.tv_sec) + (currentTime.tv_usec - mStartTime.tv_usec) / 1000000.0;
	return sec > MAX_TIME;
}

void LoginInfo::setNickname(const std::string& nickname)
{
	mNickname = nickname;
}

void LoginInfo::setUsername(const std::string& username)
{
	mUsername = username;
}

void LoginInfo::setAthenticated()
{
	mbAuthenticated = true;
}
