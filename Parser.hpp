/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:49:15 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/24 18:38:10 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Parser
{
public:
	static bool isInvalidPortNumber(const char* port);
	static bool isInvalidPassword(const char* password);
};