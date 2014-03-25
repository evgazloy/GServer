#ifndef MSOCKET_H
#define MSOCKET_H

#include <QObject>
#include <QTimer>
#include <QSslSocket>
#include <QFile>

class MSocket : public QObject
{
    Q_OBJECT
public:
    explicit MSocket(quint8 type, qintptr handle, QObject *parent = 0);
    ~MSocket();

    enum
    {
        SERVER_TYPE,
        CLIENT_TYPE
    };

private:
    QSslSocket *m_socket;
    quint8 m_type;

    void encrypted();

signals:
    void sig_delete();

public slots:
    void socketError(const QList<QSslError> &list);
    void socketError(QAbstractSocket::SocketError error);
};

#endif // MSOCKET_H