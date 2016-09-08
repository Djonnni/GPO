#include "receiver.h"
#include "serialport.h"
#include <QCoreApplication>
#include "QtSerialPort/QSerialPort"
#include <QDebug>

const int buffer_size = 12;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Receiver *receiver;
    receiver = new Receiver();
    SerialPort serial_port(buffer_size);             //Затащить в конструктор
    qDebug()<<"RECEIVER!\n";                //Сделать возможность задать порт с самого начала
    char port_name[10];
    qDebug()<<"Input name of port: ";
    gets(port_name);
    if (serial_port.connect(port_name))        //Векторы
    {
      serial_port.readData(receiver);
      serial_port.disconnect();
    } else
        qDebug()<<"Port not found";
    return a.exec();
}

