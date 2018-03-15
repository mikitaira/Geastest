#ifndef __UTILITY_H__
#define __UTILITY_H__


#include <string>
#include <direct.h>
#include <sys/stat.h>

bool MakeDirectory(std::string lpPathName);
void myTrim(char* str);

#endif //__UTILITY_H__