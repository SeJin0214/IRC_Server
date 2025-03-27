/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Util.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:54:10 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/26 15:23:35 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Util.hpp"

unsigned int Util::generateHash65599(const char* str)
{
	unsigned int	hash = 0;

	for (size_t i = 0; str[i] != '\0'; ++i)
	{
		hash = 65599 * hash + str[i];
	}
	return (hash ^ (hash >> 16));
}
