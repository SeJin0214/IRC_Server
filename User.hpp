/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:13:00 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/26 22:06:40 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

class User
{
    
public:
    User(const std::string& username, const std::string& nickname);
    std::string getUsername() const;
    std::string getNickname() const;
    void setNickname(std::string& nickname);
    bool operator==(const User& rhs) const;
    ~User();

private:
    std::string mUsername;
    std::string mNickname;

};