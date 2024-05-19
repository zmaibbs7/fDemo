#include <iostream>
#include <QCoreApplication>
#include <QBreakpadHandler.h>
#include <QDir>
#include "AAA.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
    QBreakpadInstance.setDumpPath(QDir::currentPath() + "/crashes");

	A a;
	a.test();
 
	return 0;
}