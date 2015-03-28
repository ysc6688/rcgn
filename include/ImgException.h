/*
 * author : Shucheng Yin
 * stu_id : SA14011016
 * e-mail : ysc6688@mail.ustc.edu.cn
 *  date  : 2014-11-22
 *  file  : ImgException.h
 *  desc  : The declaration of Exception handling mechanism
 */
#ifndef __IMGEXCEPTION_H__
#define __IMGEXCEPTION_H__

#include <exception>
#include <string>

class LoadException : public std::exception {
public:
	LoadException(const char *pMsg);
	~LoadException() throw(){}
	const char* what() const throw();
private:
	std::string m_msg;
};

class RemoveException : public std::exception {
public:
	RemoveException(const char *pMsg);
	~RemoveException() throw(){}
	const char* what() const throw();
private:
	std::string m_msg;
};

#endif		// __IMGEXCEPTION_H__
