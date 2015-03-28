/*
 * author : Shucheng Yin
 * stu_id : SA14011016
 * e-mail : ysc6688@mail.ustc.edu.cn
 *  date  : 2014-11-22
 *  file  : ImgException.cpp
 *  desc  : The declaration of Exception handling mechanism
 */

#include "../include/ImgException.h"
using namespace std;

LoadException::LoadException(const char *pMsg) : m_msg(pMsg)
{
	m_msg = "Can't load file "+m_msg+" !";
}

const char* LoadException::what() const throw() 
{
	return m_msg.c_str();
}

RemoveException::RemoveException(const char *pMsg) : m_msg(pMsg)
{
	m_msg = "Remoce file "+m_msg+" error!";
}

const char* RemoveException::what() const throw() 
{
	return m_msg.c_str();
}
