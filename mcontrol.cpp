#include "mcontrol.h"
#include <QDebug>

MControl::MControl(QObject *parent) :
    QObject(parent)
{
}

void MControl::inCmd(QByteArray data)
{
    QDataStream in(data);
    quint8 cmd;
    in>>cmd;

    switch(cmd)
    {
    case cmdRegistration:
    {
        cmdRegistration_s data;
        in>>data;
        if(in.status()!=QDataStream::Ok)
            qWarning()<<"Can't read registration stream";
        else
            this->registration(data, (qintptr)QObject::sender());
        break;
    }
    }
}

void MControl::registration(const cmdRegistration_s data, qintptr id)
{
    if(data.type == typeControl)
        qDebug()<<"Control registered";
    if(data.type == typeDevice)
        qDebug()<<"Device registered";
    if((data.type!=typeDevice) || (!m_clients.key(id, 0)))
        m_clients.insert(id, data.type);
}

MControl::~MControl()
{

}
