#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <receiver.h>


class SerialPort
{

public:
    SerialPort(int);
    //~SerialPort();
    //void put_packet(QByteArray packet);
   // bool get_Status();
    bool connect(QString port);
    void disconnect();
    void readData(Receiver *receiver);
signals:

private:
    QSerialPort *serial;
    bool status;
    int sizeBuffer;

public slots:
};

#endif // SERIALPORT_H
