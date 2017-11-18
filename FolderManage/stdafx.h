#ifndef _STDAFX_H
#define _STDAFX_H

#include <windows.h>
#include <string>
#include <map>
#include <list>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

#include <string.h>

#ifdef _UNICODE
typedef std::wstring string_t;
#else
typedef std::string  string_t;
#endif

#endif