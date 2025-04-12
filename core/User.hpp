/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:13:00 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/10 14:02:06 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <vector>
#include "Result.hpp"
#include <sstream>

class User
{
    
public:
    User();
    User(const User& user);
    User(const std::string& nickname, const std::string& username);
    std::string getUsername() const;
    std::string getNickname() const;
    size_t getJoinedChannelCount() const;
    std::vector<std::string> getJoinedChannels() const;
    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username);
    bool isInChannel(const std::string& title);
    void addJoinedChannel(std::string& title);
    std::string flushBuffer();
    void appendStringInBuffer(const char* buffer);
    void removeLastJoinedChannel();


    Result<size_t> getIndexOfJoinedChannel (const std::string& channelName); // 추가 (donkim3)
    void removeJoinedChannel(std::string channelName); // 추가 (donkim3)
    std::string getJoinedChannelName(size_t indexOfJoinedChannels); // 추가 (donkim3)
    

    Result<std::string> getLastJoinedChannel() const;
    ~User();

private:
    std::vector<std::string> mJoinedChannels;
    std::string mUsername;
    std::string mNickname;
    std::stringstream mBuffer;
};