/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:35:34 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/19 16:49:56 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <cassert>

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "invalid args" << std::endl; 
		return 0;
	}
	
	// 올바르지 않은 port 번호  1 ~ 65534, short 범위 벗어나는 경우 


	return 0;
}