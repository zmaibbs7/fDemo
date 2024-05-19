#include <iostream>
#include <QBreakpadHandler.h>
#include <QDir>
#include "AAA.h"

int main(int argc, char *argv[])
{
    QBreakpadInstance.setDumpPath(QDir::currentPath() + "/crashes");

	A a;
	a.test();
 
	return 0;
}