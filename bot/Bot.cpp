/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 12:27:36 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/12 18:34:26 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "Bot.hpp"
#include "Server.hpp"
#include <ctime>
#define SYSCALL_FAIL (-1)
#define MAX_WAIT (11)

Bot::Bot(const char* port, const char* nickname, const char* username)
: mNickname(nickname)
, mUsername(username)
, mServerName("")
, mbIsRunning(false)
{
    mConnectedSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(std::atoi(port));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (connect(mConnectedSocket, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == SYSCALL_FAIL)
    {
        std::cerr << "connect fail" << std::endl;
        close(mConnectedSocket);
        exit(EXIT_FAILURE);
    }
}

bool Bot::tryLogin(const char* password)
{
    std::stringstream ss;
    ss << "PASS " << password << "\r\n"
    << "USER " << mUsername << " * * :Bot\r\n"
    << "NICK " << mNickname << "\r\n";
    std::string loginMessage = ss.str(); 
    
    send(mConnectedSocket, loginMessage.c_str(), loginMessage.size(), 0);
    
    fd_set set;

    FD_ZERO(&set);
    FD_SET(mConnectedSocket, &set);

    struct timeval start;
    gettimeofday(&start, NULL);
    struct timeval end;
    do
    {
        fd_set temp = set;
        
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;

        select(mConnectedSocket + 1, &temp, NULL, NULL, &tv);
        gettimeofday(&end, NULL);
        if (FD_ISSET(mConnectedSocket, &temp) == false)
        {
            continue;
        }
        
        char buffer[MAX_BUFFER] = { 0, };
        recv(mConnectedSocket, buffer, MAX_BUFFER, 0);
        
        const char* token = std::strtok(buffer, " ");
        assert(token != NULL);
        if (std::strcmp(token, "ERROR") == 0)
        {
            std::cout << "ERROR!" << std::endl;
            return false;
        }
        const char* code = std::strtok(NULL, " ");
        assert(code != NULL);
        if (std::strcmp(code, "001") == 0)
        {
            mServerName = token;
            return true;
        }
        
    } while (end.tv_sec - start.tv_sec < MAX_WAIT);
    return false;
}

void Bot::run()
{
    fd_set set;

    FD_ZERO(&set);
    
    FD_SET(mConnectedSocket, &set);
    FD_SET(STDIN_FILENO, &set);
    
    mbIsRunning = true;
    struct timeval timer;
    gettimeofday(&timer, NULL);
    while (mbIsRunning)
    {
        fd_set temp = set;
        
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        select(mConnectedSocket + 1, &temp, NULL, NULL, &tv);
        if (FD_ISSET(mConnectedSocket, &temp))
        {
            handleCommand();
        }
        else if (FD_ISSET(STDIN_FILENO, &temp))
        {
            handleQuit();
        }
        struct timeval current;
        gettimeofday(&current, NULL);
        if (current.tv_sec - timer.tv_sec > 1)
        {
            timer = current;
            executeList();
        }
    }
}

void Bot::handleCommand()
{
    char buffer[MAX_BUFFER] = { 0, };
    recv(mConnectedSocket, buffer, MAX_BUFFER, 0);

    std::stringstream ss;
    ss << buffer;
    std::cout << ss.str();
    std::string command;
    std::getline(ss, command, ' ');
    std::getline(ss, command, ' ');
    if (command == "PRIVMSG")
    {
        std::string name;
        std::getline(ss, name, ' ');
        std::string message;
        std::getline(ss, message);

        assert(message[message.size() - 1] == '\r');
        message.erase(message.size() - 1, 1);
       
        if (message == ":!help")
        {
            std::stringstream messageToSend;
            messageToSend
            << command << " " << name << " :Available Commands:\n"
            << command << " " << name << " :-------------------\n"
            << command << " " << name << " :!help        - Show this help message.\n"
            << command << " " << name << " :!coinflip    - Simulates a coin toss, randomly returning 'Heads' or 'Tails'.\n"
            << command << " " << name << " :!dice        - Rolls a die; returns a random number between 1 and 6\n\n"
            << command << " " << name << " :Bot created by [sejjeong]] on [2025/04/12].";
            sendToServer(messageToSend.str().c_str());
        }
        else if (message == ":!coinflip")
        {
            std::stringstream messageToSend;
            messageToSend
            << command << " " << name;

            std::srand(static_cast<unsigned int>(std::time(0)));
            int randomValue = std::rand() % 2;
            if (randomValue == 0)
            {
                messageToSend << " :Heads!";
            }
            else
            {
                messageToSend << " :Tails!";
            }
            sendToServer(messageToSend.str().c_str());
        }
        else if (message == ":!dice")
        {
            std::cout << message << std::endl;

            std::stringstream messageToSend;
            messageToSend
            << command << " " << name << " :result! {";

            std::srand(static_cast<unsigned int>(std::time(0)));
            int randomValue = std::rand() % 6;
            messageToSend << (randomValue + 1) << "}";
            sendToServer(messageToSend.str().c_str());
        }
    }
    else if (command == "321")
    {
        std::string line;
        std::getline(ss, line);
        while (true)
        {
            std::getline(ss, line);
            std::stringstream lineStream(line);

            std::string token;
            std::getline(lineStream, token, ' ');
            assert(token == mServerName);
            
            std::getline(lineStream, token, ' ');
            if (token == "323")
            {
                break;
            }
            
            assert(token == "322");
            
            std::getline(lineStream, token, ' ');
            assert(token == mNickname);
            
            std::getline(lineStream, token, ' ');
            assert(token[0] == '#');
            std::string channelName = token.erase(0, 1);
            
            std::getline(lineStream, token, ' ');
            int userCount = strtol(token.c_str(), NULL, 10);

            bool bIsJoinedChannel = false;
            for (size_t i = 0; i < mJoinedChannels.size(); ++i)
            {
                if (mJoinedChannels[i] == channelName)
                {
                    bIsJoinedChannel = true;
                    break;
                }
            }

            if (bIsJoinedChannel == false)
            {
                executeJoin(channelName);
            }
            else if (bIsJoinedChannel && userCount == 1)
            {
                executePart(channelName);
            }
        }
    }
}

void Bot::handleQuit()
{
    char buffer[MAX_BUFFER] = { 0, };
    if (read(STDIN_FILENO, buffer, MAX_BUFFER) == 0)
    {
        return;
    }

    std::stringstream ss;
    ss << buffer;
    std::string command;
    std::getline(ss, command);
    if (command == "quit")
    {
        mbIsRunning = false;
    }
}

void Bot::executeList()
{
    sendToServer("LIST");
}

void Bot::executePart(const std::string& channelName)
{
    std::string partMessage = "PART #" + channelName;
    std::vector<std::string>::iterator it = mJoinedChannels.begin();
    while (it != mJoinedChannels.end())
    {
        if (*it == channelName)
        {
            mJoinedChannels.erase(it);
            break;
        }
        ++it;
    }
    sendToServer(partMessage.c_str());
}

void Bot::executeJoin(const std::string& channelName)
{
    std::string joinMessage = "JOIN #" + channelName;
    mJoinedChannels.push_back(channelName);
    sendToServer(joinMessage.c_str());
}

void Bot::sendToServer(const char* buffer)
{
    char temp[MAX_BUFFER] = { 0, };
    sprintf(temp, "%s\r\n", buffer);
    send(mConnectedSocket, temp, std::strlen(temp), 0);
}

Bot::~Bot()
{
    for (size_t i = 0; i < mJoinedChannels.size(); ++i)
    {
        executePart(mJoinedChannels[i]);
    }
    close(mConnectedSocket);
}