#include "msocket.h"
#include <QDebug>

MSocket::MSocket(qintptr handle, QObject *parent) :
    QObject(parent), m_socket(0)
{
    m_socket = new QSslSocket(this);
    Q_CHECK_PTR(m_socket);
    connect(m_socket, SIGNAL(sslErrors(QList<QSslError>)), SLOT(socketError(QList<QSslError>)));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)));
    connect(m_socket, &QSslSocket::encrypted, this, &MSocket::encrypted);
    connect(m_socket, &QSslSocket::disconnected, this, &MSocket::sig_delete);
    connect(m_socket, &QSslSocket::readyRead, this, &MSocket::inData);    

    qDebug()<<"New incoming connection";
    if(!m_socket->setSocketDescriptor(handle))
    {
        delete m_socket;
        m_socket = 0;
        qCritical()<<"Can't set socket descriptor";
    }
    else
    {
        m_socket->setPrivateKey("../keys/server.pem");
        m_socket->setLocalCertificate("../keys/server.pem");
        m_socket->addCaCertificates("../keys/rootCA.pem");

        m_socket->startServerEncryption();
    }
}

void MSocket::encrypted()
{
    qDebug()<<"Connection encrypted";
}

void MSocket::inData()
{
    Q_CHECK_PTR(m_socket);
    QDataStream in(m_socket);
    while(m_socket->bytesAvailable())
    {
        if(!m_dataSize)
        {
            if(m_socket->bytesAvailable() < (qint64)sizeof(m_dataSize))
                continue;
            in>>m_dataSize;
            m_inBuffer.clear();
        }
        qint64 byteToRead = qMin<qint64>(m_socket->bytesAvailable(), m_dataSize);

        QByteArray buffer;
        buffer.resize(byteToRead);
        if(in.readRawData(buffer.data(), byteToRead)!=byteToRead)
        {
            qCritical()<<"Can't read all bytes.";
            m_socket->disconnectFromHost();
            return;
        }
        m_inBuffer.append(buffer);
        m_dataSize -= byteToRead;
        if(!m_dataSize)
            emit sig_cmd(m_inBuffer);
    }
}

void MSocket::socketError(const QList<QSslError> &list)
{
    foreach(QSslError err, list)
        qDebug()<<"SSL error:"<<err.errorString();

    Q_CHECK_PTR(m_socket);
    m_socket->disconnectFromHost();
}

void MSocket::socketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    Q_CHECK_PTR(m_socket);
    qDebug()<<"Socket error:"<<m_socket->errorString();
}

MSocket::~MSocket()
{
    delete m_socket;
}
