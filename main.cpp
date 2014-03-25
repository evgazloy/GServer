#include <QCoreApplication>
#include "mmaininstance.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("GServer");
    QCoreApplication::setApplicationVersion("0.1");
    MMainInstance m_inst;

    return a.exec();
}
