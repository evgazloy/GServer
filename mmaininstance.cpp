#include "mmaininstance.h"
#include <QDebug>

MMainInstance::MMainInstance(int argc, char *argv[], QObject *parent) :
    QTcpServer(parent)
{
    this->parseArgs(argc, argv);
    if(this->listen(m_options.interface, m_options.port))
        qDebug()<<"Server started on"<<m_options.interface.toString()<<":"<<m_options.port;
    else
        qCritical()<<"Server can't started."<<this->errorString();
}

void MMainInstance::parseArgs(int argc, char *argv[])
{
    for(int i=1;i<argc;i++)
    {
        QString arg(argv[i]);
        if(arg.size()>1 && arg.at(0) == '-')
        {
            char cmd = arg.at(1).toLatin1();
            QString param(arg.mid(2));

            switch(cmd)
            {
            case 'i':
            {
                QHostAddress interface(param);
                if(!interface.isNull())
                    m_options.interface = interface;
                else
                    qWarning()<<"Invalid interface";
                break;
            }

            case 'p':
            {
                bool status = false;
                quint16 port = param.toInt(&status, 10);
                if(status)
                    m_options.port = port;
                else
                    qWarning()<<"Invalid port";
                break;
            }
            default:
                qWarning()<<"Unknown argument -"<<cmd;
            }
        }
        else
            qWarning()<<"Unknown argument"<<arg;
    }
}

void MMainInstance::initSocket(qintptr handle)
{
    QThread *cThread = QThread::currentThread();
    Q_CHECK_PTR(cThread);

    MSocket *socket = new MSocket(MSocket::SERVER_TYPE, handle);
    Q_CHECK_PTR(socket);

    connect(socket, &MSocket::sig_delete, this, &MMainInstance::deleteSocket, Qt::QueuedConnection);
}

void MMainInstance::deleteSocket()
{
    MSocket *cSocket = (MSocket*)QObject::sender();
    Q_CHECK_PTR(cSocket);

    QThread *cThread = cSocket->thread();
    Q_CHECK_PTR(cThread);

    delete cSocket;
    cThread->quit();
}

void MMainInstance::incomingConnection(qintptr handle)
{
    QThread *thread = new QThread(this);
    Q_CHECK_PTR(thread);

    connect(thread, &QThread::started, [this, handle](){
        this->initSocket(handle);
    });

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}

MMainInstance::~MMainInstance()
{

}
