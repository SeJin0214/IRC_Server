/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:35:34 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/24 19:00:46 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>
#include <unistd.h>
#include "Parser.hpp"
#include "Server.hpp"

// IPv4, TCP 
int main(int argc, char** argv)
{
	(void) argv;
	if (argc != 3)
	{
		std::cerr << "사용법 : " << argv[0] << " <port number> <password>" << std::endl;
		return 0;
	}

	std::cout << "연결 대기중... " << std::endl;
	Server server(argv[1], argv[2]);
	
	server.run();

	// bool bRunning = false;
	// size_t i = 0;
	// while (bRunning == false && i < 10)
	// {
		
	// 	++i;
	// }
	
	
	return 0;
}
