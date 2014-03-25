#ifndef MMAININSTANCE_H
#define MMAININSTANCE_H

#include <QObject>
#include <QHostAddress>
#include <QTcpServer>
#include <QThread>
#include "msocket.h"

#define mINTERFACE QHostAddress::Any
#define mPORT 32345

class MMainInstance : public QTcpServer
{
    Q_OBJECT
public:
    explicit MMainInstance(int argc, char *argv[], QObject *parent = 0);
    ~MMainInstance();

private:
    struct Options_s
    {
        QHostAddress interface;
        quint16 port;

        Options_s() : interface(mINTERFACE), port(mPORT) {}
    };

    Options_s m_options;

    void parseArgs(int argc, char *argv[]);
    void initSocket(qintptr handle);
    void deleteSocket();

protected:
    void incomingConnection(qintptr handle);

signals:

};

#endif // MMAININSTANCE_H
