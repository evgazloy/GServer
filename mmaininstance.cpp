#include "mmaininstance.h"
#include <QDebug>
#include <QCommandLineParser>

MMainInstance::MMainInstance(QObject *parent) :
    QTcpServer(parent)
{
    this->parseArgs();
    if(this->listen(m_options.interface, m_options.port))
        qDebug()<<"Server started on"<<m_options.interface.toString()<<":"<<m_options.port;
    else
        qCritical()<<"Server can't started."<<this->errorString();
}

void MMainInstance::parseArgs()
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Grow server");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption interface(QStringList()<<"i"<<"interface", "Interface to listen", "ip", "0.0.0.0");
    parser.addOption(interface);

    QCommandLineOption port(QStringList()<<"p"<<"port", "Port to listen", "number", "32345");
    parser.addOption(port);

    parser.process(QCoreApplication::arguments());

    m_options.interface = QHostAddress(parser.value(interface));
    m_options.port = parser.value(port).toInt();
    if(m_options.interface.isNull())
    {
        qDebug()<<"Invalid interface";
        m_options.interface = QHostAddress::Any;
    }
}

void MMainInstance::initSocket(qintptr handle)
{
    QThread *cThread = QThread::currentThread();
    Q_CHECK_PTR(cThread);

    MSocket *socket = new MSocket(handle);
    Q_CHECK_PTR(socket);

    connect(socket, &MSocket::sig_delete, this, &MMainInstance::deleteSocket, Qt::QueuedConnection);
    connect(socket, &MSocket::sig_cmd, &m_control, &MControl::inCmd, Qt::QueuedConnection);
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
