/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:49:15 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/19 17:51:55 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Parser
{
public:
	static bool IsInvalidPortNumber(const char* port);
	static bool IsInvalidPassword(const char* password);
};