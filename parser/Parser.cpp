/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:22:49 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/11 13:42:02 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstdlib>
#include <cstring>
#include "Parser.hpp"

bool Parser::isInvalidPortNumber(const char* port)
{
    size_t length = std::strlen(port);
    if ((length == 4 || length == 5) == false)
	{
        return true;
	}
	for (size_t i = 0; i < length; ++i)
    {
        if (std::isdigit(port[i]) == false)
        {
            return true;
        }
    }
    int value = std::atoi(port);
    return (value < 1024 || value == 65535);
}

// 8 ~ 16  min Uppercase, lowercase, digit
bool Parser::isInvalidPasswordFormatted(const char* password)
{
    const size_t length = std::strlen(password);
    if (length < 8  || length > 16)
    {
        return true;
    }
    for (size_t i = 0; i < length; ++i)
    {
        if (std::isalnum(password[i]) == false)
        {
            return true;
        }
    }
    return false;
}

bool Parser::isInvalidNameFormatted(const char* name)
{
    const size_t length = std::strlen(name);
    if (length > 30)
    {
        return true;
    }
    for (size_t i = 0; i < length; ++i)
    {
        if (std::isalnum(name[i]) == false)
        {
            return true;
        }
    }
    return false;
}