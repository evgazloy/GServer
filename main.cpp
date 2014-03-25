#include <QCoreApplication>
#include "mmaininstance.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MMainInstance m_inst(argc, argv);

    return a.exec();
}
