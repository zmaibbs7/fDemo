#include <iostream>
#include <QBreakpadHandler.h>
#include <QDir>
#include "AAA.h"

int main(int argc, char *argv[])
{
    QBreakpadInstance.setDumpPath(QDir::currentPath() + "/crashes");
	// 启动时检查并发送未发送的转储文件
    QBreakpadInstance.sendDumps();

	A a;
	a.test();
 
	return 0;
}