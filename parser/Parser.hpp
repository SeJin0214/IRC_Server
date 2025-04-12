/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:16:36 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/11 13:35:53 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Parser
{

public:
    static bool isInvalidPortNumber(const char* port);
    static bool isInvalidPasswordFormatted(const char* password);
    static bool isInvalidNameFormatted(const char* name);
private:

};