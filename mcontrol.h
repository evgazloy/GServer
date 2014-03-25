#ifndef MCONTROL_H
#define MCONTROL_H

#include <QObject>
#include <QMap>
#include "tcpcommands.h"

class MControl : public QObject
{
    Q_OBJECT
public:
    explicit MControl(QObject *parent = 0);
    ~MControl();

private:
    QMap<qintptr, quint8> m_clients;

    void registration(const cmdRegistration_s data, qintptr id);

signals:
    void sig_sendToControl(QByteArray data);
    void sig_sendToViewer(QByteArray data);

public slots:
    void inCmd(QByteArray data);
};

#endif // MCONTROL_H
