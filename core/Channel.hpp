/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:19:57 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/10 14:19:17 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include "Space.hpp"
#include "Mode.hpp"

class Channel : public Space
{

public:
	virtual ~Channel();
	IOutgoingMessageProvider* getOutgoingMessageProvider(const char* buffer) const;
	IExecutable* getExecutor(const char* buffer) const;
	Channel(const std::string& title, const char* password);
	bool operator<(const Channel& rhs);
	std::string getTitle() const;
	std::string getTopic() const;
	bool setTopic(int clientSocket, std::string& topic);
	unsigned int getPassword() const;

	void setPassword(std::string& password);
	bool isPassword(std::string& password);
	bool isInvited (std::string& invitedUser);

	void enterInvitedList (std::string& invitedUser);
	void exitInvitedList (std::string& invitedUser);
	bool isAddUserAsAdmin(const std::string& userNickname);//서버에서 채널생성할 때 사용 유저 호스트만들기
	void removeOperatorNicknames(const std::string& nickname);
	void removeInvitedLists(const std::string& nickname);
	std::string getActiveMode();
	bool isModeActive(const eMode mode);
	bool onMode(const int userSocket, const eMode mode);
	bool offMode(const int userSocket, const eMode mode);
	bool enterUser(const int clientSocket, User* user);
	User* exitUserOrNull(const int clientSocket);
	bool isOperator(const User& user) const;
	bool isOperator(const int userSocket) const;
	void setMaxMemberCount(unsigned int num);
	size_t getMemberCount() const;
 
private:
	std::vector<std::string> mOperatorNicknames;
	std::vector<std::string> mInvitedList;
	unsigned int mMaxMemberCount;
	unsigned char mModeFlag;
	std::string mTitle;
	std::string mTopic;
	unsigned int mPassword;
};