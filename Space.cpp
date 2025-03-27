/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Space.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:52:43 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/27 16:07:03 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Space.hpp"

Space::~Space()
{

}

std::vector<int> Space::getFdSet() const
{
	std::vector<int> result;
	std::map<int, User>::const_iterator it = mUsers.begin();
	while (it != mUsers.end())
	{
		result.push_back(it->first);
		++it;
	}
	return result;
}

bool Space::enterUser(int clientSocket, User& user)
{
	mUsers.insert(std::pair<int, User>(clientSocket, user));
	return true;
}

std::string Space::getHelpMessage() const
{
	std::string message = "COMMAND 라고 입력하여 명령어를 확인하세요!\r\n";
	
	return message;
}

std::string Space::getCommonCommandList() const
{
	// 공통 : 
	// 귓속말을 하고 싶다면 무슨 입력을 하세요. 
	// 유저 목록 확인
	// 전체 유저, 채널 유저
	return "";
}

void Space::exitUser(int clientSocket)
{
	mUsers.erase(clientSocket);
	//  user 반환하기? pair로 반환하기?
}