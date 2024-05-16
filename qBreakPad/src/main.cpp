#include <iostream>
#include <QBreakpadHandler.h>
#include <QDir>
int main()
{
    QBreakpadInstance.setDumpPath(QDir::currentPath() + "/crashes");

    int* p = NULL; *p = 10;
    return 0;
}