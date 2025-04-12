/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Util.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:54:10 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/05 13:38:03 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include "Util.hpp"

unsigned int Util::generateHash65599(const char* str)
{
	assert(str != NULL);
	unsigned int	hash = 0;

	for (size_t i = 0; str[i] != '\0'; ++i)
	{
		hash = 65599 * hash + str[i];
	}
	return (hash ^ (hash >> 16));
}

std::string Util::getLowercaseString(const std::string& str)
{
	std::string result(str);
	for (size_t i = 0; i < result.size(); ++i)
	{
		if (std::isupper(result[i]))
		{
			result[i] -= 32;
		}
	}
	return result;
}
