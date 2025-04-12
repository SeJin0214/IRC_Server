/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Space.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 10:51:10 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/10 13:19:25 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <map>
#include <vector>
#include "IExecutable.hpp"
#include "User.hpp"
#include "Result.hpp"

class IIncomingMessageProvider;
class IOutgoingMessageProvider;

class Space
{
public:
	virtual ~Space();
	/* getter */
	virtual IOutgoingMessageProvider* getOutgoingMessageProvider(const char* buffer) const;
	virtual IExecutable* getExecutor(const char* buffer) const;
	virtual bool enterUser(const int clientSocket, User* user);
	virtual User* exitUserOrNull(const int clientSocket);
	std::vector<int> getFdSet() const;
	size_t getUserCount() const;
	std::vector<std::string> getNicknames() const;
	std::vector<std::string> getUsernames() const;
	User* findUserOrNull(const int clientSocket) const;
	

	// 이 둘은 channel과 lobby에만 있어야 함, 이동할 지 생각하기, 
	Result<std::pair<int, User*> > findUser(const std::string& nickname) const;

protected:
	Space() {};
	std::map<int, User*> mUsers;
  	std::string getCommandSection(const char* buffer) const;

 };
