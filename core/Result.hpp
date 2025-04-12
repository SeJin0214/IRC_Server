/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Result.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 23:42:19 by sejjeong          #+#    #+#             */
/*   Updated: 2025/04/10 12:47:21 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

template <typename T>
class Result
{

public:
	Result(T value, bool bIsSucceed);
	T getValue();
	bool hasSucceeded();

private:
	T mValue;
	bool mbIsSucceed;
};

template <typename T>
Result<T>::Result(T value, bool bIsSucceed)
: mValue(value)
, mbIsSucceed(bIsSucceed)
{
	
}

template <typename T>
T Result<T>::getValue()
{
	return mValue;
}

template <typename T>
bool Result<T>::hasSucceeded()
{
	return mbIsSucceed;
}