/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 12:27:42 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/12 15:50:27 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <vector>

class Bot
{

public:
    Bot(const char* port, const char* nickname, const char* username);
    ~Bot();
    bool tryLogin(const char* password);
    void run();
private:
    std::string mNickname;
    std::string mUsername;
    std::string mServerName;
    std::vector<std::string> mJoinedChannels;
    bool mbIsRunning;
    int mConnectedSocket;
    void handleCommand();
    void handleQuit();
    void executeList();
    void executePart(const std::string& channelName);
    void executeJoin(const std::string& channelName);
    void sendToServer(const char* buffer);

};