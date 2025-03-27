/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:01:36 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/26 15:29:42 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

enum eMode
{
	MODE_INVITE_ONLY,
	MODE_TITLE_LOCK,
	MODE_PASSWORD_PROTECTED,
	MODE_OPERATOR_ONLY,
	MODE_USER_LIMIT
};
