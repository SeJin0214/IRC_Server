/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:19:57 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/27 15:25:14 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Space.hpp"
#include "Mode.hpp"
#include "Result.hpp"

class Channel : public Space
{

public:
	virtual ~Channel();
	std::string getCommandList() const;
	Channel(const std::string& title, const char* password);
	bool operator<(const Channel& rhs);
	std::string getTitle() const;
	bool setTitle(int clientSocket, std::string& title);
	unsigned int getPassword() const;
	std::vector<std::string> getNicknames() const;
	std::vector<std::string> getUsernames() const;
	std::vector<int> getClientSockets() const;
	Result<User> findUser(const int clientSocket) const;
	bool toggleMode(User& user, const eMode mode);
	// 유저의 이름을 바꾸는 setter
	
private:
	std::vector<std::string> mOperators;
	unsigned char mModeFlag;
	std::string mTitle;
	unsigned int mPassword;
	
	bool isOperator(const User& user) const;
	bool isOperator(const int userSocket) const;
	
};
