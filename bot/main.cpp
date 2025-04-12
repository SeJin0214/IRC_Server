/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:04:18 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/12 16:27:45 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>
#include "args.hpp"
#include "Bot.hpp"
#include "Parser.hpp"

int main(int argc, char** argv)
{
    if (argc != 5)
    {
        std::cerr << argv[0] << " <port> <password> <nickname> <username>" << std::endl;
        return EXIT_FAILURE;
    }
    
    if (Parser::isInvalidPortNumber(argv[ARGS_PORT]))
    {
        std::cerr << "옳지 않은 port 번호, 1024~65534 사이로 넣어야 합니다." << std::endl;
        return EXIT_FAILURE;
    }
    else if (Parser::isInvalidPasswordFormatted(argv[ARGS_PASSWORD]))
    {
        std::cerr << "비밀번호는 8~16자리, 대소문자, 숫자로만 이루어져야합니다." << std::endl;
        return EXIT_FAILURE;    
    }
    else if (Parser::isInvalidNameFormatted(argv[ARGS_NICKNAME]) 
    || Parser::isInvalidNameFormatted(argv[ARGS_USERNAME]))
    {
        std::cerr << "닉네임과 유저이름은 30자 이내의 대소문자, 숫자로만 이루어져야합니다." << std::endl;
        return EXIT_FAILURE;
    }

    Bot bot(argv[ARGS_PORT], argv[ARGS_NICKNAME], argv[ARGS_USERNAME]);
    if (bot.tryLogin(argv[ARGS_PASSWORD]) == false)
    {
        std::cerr << "닉네임이 중복이거나 password가 올바르지 않습니다." << std::endl;
        return EXIT_FAILURE;
    }
    
    bot.run();
    
    return EXIT_SUCCESS;
}