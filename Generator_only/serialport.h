#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <generator.h>
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>


class SerialPort
{

public:
    SerialPort(int, QString, Generator *);
    //~SerialPort();
    void put_packet(QByteArray packet, Generator *generator);
    void put_packet(QByteArray packet);
    //bool get_Status();
    bool connect(QString port);
    void disconnect();
    bool get_status();
   // void setSizeBuffer(int value);
   // int getSizeBuffer();
signals:

private:
    QSerialPort *serial;
    bool status;
    int sizeBuffer;
    QString port_name;
public slots:
};

#endif // SERIALPORT_H
