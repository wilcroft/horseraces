#ifndef __STRINGLISTUTILS_H__
#define __STRINGLISTUTILS_H__

#include <string>
#include <list>
#include <qstringlist.h>
#include "../horseraces/better.h"

QStringList toQStringList(std::list<std::string>  l);
std::list <std::string> toStdStringList(QStringList ql);
QStringList toQStringList(std::list<Better> b);

#endif