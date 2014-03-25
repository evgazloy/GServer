#include "msocket.h"
#include <QDebug>

MSocket::MSocket(quint8 type, qintptr handle, QObject *parent) :
    QObject(parent), m_socket(0), m_type(type)
{
    m_socket = new QSslSocket(this);
    Q_CHECK_PTR(m_socket);
    connect(m_socket, SIGNAL(sslErrors(QList<QSslError>)), SLOT(socketError(QList<QSslError>)));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(socketError(QAbstractSocket::SocketError)));
    connect(m_socket, &QSslSocket::encrypted, this, &MSocket::encrypted);
    connect(m_socket, &QSslSocket::disconnected, this, &MSocket::sig_delete);

    if(m_type == SERVER_TYPE)
    {
        qDebug()<<"New incoming connection";
        if(!m_socket->setSocketDescriptor(handle))
        {
            delete m_socket;
            m_socket = 0;
            qCritical()<<"Can't set socket descriptor";
        }
        else
        {
            m_socket->setPrivateKey("keys/server.pem");
            m_socket->setLocalCertificate("keys/server.pem");
            m_socket->addCaCertificates("keys/server.pem");

            m_socket->startServerEncryption();
        }
    }
}

void MSocket::encrypted()
{
    qDebug()<<"Connection encrypted";
}

void MSocket::socketError(const QList<QSslError> &list)
{
    foreach(QSslError err, list)
        qDebug()<<"SSL error:"<<err.errorString();

    Q_CHECK_PTR(m_socket);
    //if(list.contains(QSslError::SelfSignedCertificate))
    m_socket->disconnectFromHost();
}

void MSocket::socketError(QAbstractSocket::SocketError error)
{
    Q_CHECK_PTR(m_socket);
    qDebug()<<"Socket error:"<<m_socket->errorString();
}

MSocket::~MSocket()
{
    delete m_socket;
    qDebug("~sock");
}
