#ifndef MSOCKET_H
#define MSOCKET_H

#include <QObject>
#include <QTimer>
#include <QSslSocket>
#include <QFile>
#include <QByteArray>
#include "tcpcommands.h"

class MSocket : public QObject
{
    Q_OBJECT
public:
    explicit MSocket(qintptr handle, QObject *parent = 0);
    ~MSocket();

private:
    QSslSocket *m_socket;
    quint32 m_dataSize = 0;
    QByteArray m_inBuffer;

    void encrypted();
    void inData();

signals:
    void sig_delete();
    void sig_cmd(QByteArray data);

public slots:
    void socketError(const QList<QSslError> &list);
    void socketError(QAbstractSocket::SocketError error);
};

#endif // MSOCKET_H
