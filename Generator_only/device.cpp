#include <QTimer>
#include <QDebug>

#include "device.h"
#include "controlcenter.h"

const bool settings = true;



Device::Device(Generator *load_gen, SerialPort *load_serial_port, QObject *parent)
{   
    generator = load_gen;
    serial_port = load_serial_port;
    generator->generate_random_packet();
    id = generator->get_id();
    measure_type = generator->get_meas_type();
    time = generator->get_time();
    value = generator->get_value();
   /* timer  = new QTimer();
    timer->setInterval(500);
    connect(timer,SIGNAL(timeout()),this, SLOT(test_slot()));
    timer->start();*/
}

Device::Device(unsigned long load_id, unsigned char load_meas_type, unsigned long load_value, unsigned long load_time, QObject *parent)
{
    id = load_id;
    measure_type = load_meas_type;
    value = load_value;
    time = load_time;
}

void Device::showInfo()
{
    qDebug()<<id;
    qDebug()<<measure_type;
    qDebug()<<time;
}

unsigned long Device::get_id()
{
    return id;
}

unsigned char Device::get_meas_type()
{
    return measure_type;
}

unsigned int Device::get_time()
{
    return time;
}

unsigned long Device::get_value()
{
    return value;
}

void Device::set_id(unsigned long new_id)
{
    id = new_id;
}


void Device::test_slot()
{
    qDebug()<<"SLOT!!!";

}

void Device::send_packet()
{
    qDebug()<<"SLOT SEND PACKET!!!";
    unsigned long int id = this->get_id();                //4 байта
    unsigned char measure_type = this->get_meas_type();   //1 байт
    generator->generate_random_packet();                  //чтобы одно и то же устройство не менялось
    unsigned int time = generator->get_time();              //2 байта
    unsigned long int value = generator->get_value();       //4 байта
    qDebug()<<id<<measure_type<<time<<value;
    generator->generate_packet(id, measure_type,value,time);
    qDebug()<<generator->get_packet().toHex();
    serial_port->put_packet(generator->get_packet());
}


