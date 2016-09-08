#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <conio.h>

#include "serialport.h"
#include "generator.h"

const bool settings = true;



SerialPort::SerialPort(int buffer_size, QString load_port_name, Generator* generator)
{
    sizeBuffer = buffer_size;
    port_name = load_port_name;
    this->serial = new QSerialPort();
    if (this->connect(load_port_name))
    {
      //this->put_packet(generator->get_packet(),generator);
      //serial_port.disconnect();
    } else
        qDebug()<<"Port is not found!";
}

void SerialPort::put_packet(QByteArray packet, Generator* generator)
{
    qDebug("To send packet put anything on keyboard!");
    _getch();
    serial->write(packet);
    serial->waitForBytesWritten(50);
    generator->showInfo();
    generator->generate_random_packet(settings);
}

void SerialPort::put_packet(QByteArray packet)
{
    serial->write(packet);
    serial->waitForBytesWritten(50);
}

bool SerialPort::connect(QString port)
{
    serial->setPortName(port);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setStopBits(QSerialPort::OneStop);
    if (serial->open(QIODevice::ReadWrite))
    {
            qDebug()<<"Conection to "<<port<<" is complete!";
            this->status = true;
            return true;

        }
    else
        {
            qDebug()<<"Conection error!!!";
            this->status = false;
            return false;
    }
}

void SerialPort::disconnect()
{
    serial->close();
    qDebug()<<"Port closed";
}

bool SerialPort::get_status()
{
    return status;
}
