/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Result.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 23:42:19 by sejjeong          #+#    #+#             */
/*   Updated: 2025/03/26 23:53:37 by sejjeong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

template <typename T>
class Result
{

public:
	Result(T value, bool bSucceed);
	T getValue();
	bool isSucceed();

private:
	T mValue;
	bool mbSucceed;

};

template <typename T>
Result<T>::Result(T value, bool bSucceed)
: mValue(value)
, mbSucceed(bSucceed)
{
	
}

template <typename T>
T Result<T>::getValue()
{
	return mValue;
}

template <typename T>
bool Result<T>::isSucceed()
{
	return mbSucceed;
}