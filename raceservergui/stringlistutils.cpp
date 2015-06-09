#include "stringlistutils.h"


QStringList toQStringList(std::list<std::string>  l){
	QStringList ql;
	for (auto& x:l){
		ql.push_back(QString::fromStdString(x));
	}
	return ql;
}

std::list <std::string> toStdStringList(QStringList ql){
	std::list<std::string> l;
	for (auto& x:ql){
		l.push_back(x.toStdString());
	}
	return l;
}

QStringList toQStringList(std::list<Better> b){
	QStringList ql;
	for (auto& x:b){
		ql.push_back(QString::fromStdString(x.getName()));
	}
	return ql;
}