#include "serialport.h"
#include "QtSerialPort/QSerialPort"
#include "QDebug"
#include "conio.h"
#include "receiver.h"

SerialPort::SerialPort(int buffer_size)
{
    sizeBuffer = buffer_size;
    serial = new QSerialPort();
}

void SerialPort::readData(Receiver* receiver)
{
    QByteArray data;
    //qDebug()<<"For starting put anything on keyboard";
    //_getch();

    //for (;;)
    //{

        if (serial->waitForReadyRead(-1))
        {
            while (!serial->atEnd())
            {
                data = serial->read(12);//data.append(serial->read(&data,12));
                //serial->clear();
                receiver->packet_parse(data);
                receiver->showInfo();
                receiver->add_data(receiver->get_id(), receiver->get_meas_type(), receiver->get_value(), receiver->get_time());
                //receiver->showData();
                qDebug()<<"Packet(Hex): "<<data.toHex()<<" Size: "<<data.size();
                if (this->sizeBuffer != data.size())
                    qDebug()<<"Error!"; else qDebug()<<"Packet is received!";
            }
        }
        readData(receiver);
     //}
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
            qDebug()<<"Connect to "<<port<<" is complete!";
            this->status = true;
            return true;

        }
    else
        {
            qDebug()<<"Connection error!!";
            this->status = false;
            return false;
    }
}

void SerialPort::disconnect()
{
    serial->close();
    qDebug()<<"Port closed";
}

