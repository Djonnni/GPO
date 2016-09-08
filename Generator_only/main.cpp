#include <QCoreApplication>
#include <QDebug>
#include <stdio.h>
#include <QSettings>
#include <fstream>
#include <iostream>

#include "generator.h"
#include "serialport.h"
#include "device.h"
#include "controlcenter.h"
#include "QtSerialPort/QSerialPort"

const int buffer_size = 12;
const bool settings = true;
//const int device_count = 10;

using namespace std;

void creating_settings()
{
    QSettings *settings_new = new QSettings("settings.ini",QSettings::IniFormat);
    settings_new->setValue("Id/StaringValue",2);
    settings_new->setValue("Id/FinalValue",2);
    settings_new->setValue("Id/Divisibility",2);
    settings_new->setValue("Measure_type/StaringValue",1);
    settings_new->setValue("Measure_type/FinalValue",10);
    settings_new->setValue("Measure_type/Divisibility",2);
    settings_new->setValue("Time/StaringValue",1);
    settings_new->setValue("Time/FinalValue",10000);
    settings_new->setValue("Time/Divisibility",4);
    settings_new->setValue("Value/StaringValue",1);
    settings_new->setValue("Value/FinalValue",10000);
    settings_new->setValue("Value/Divisibility",3);
}



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"GENERATOR!\n";
    char port_name[10];                     //Имя порта подключения
    bool create_settings;
    qDebug()<<"Create file of settings?(1 - Yes, 0 - No)";
    cin >>create_settings;
    if (create_settings)
    {
        creating_settings();
    qDebug()<<"File of settings created!";
    }
    qDebug()<<"Input name of port: ";
    cin >> port_name;
   // qDebug()<<port_name;
   // generator.generate_random_packet();
   // generator.showInfo();
    Generator generator(settings);
   // Device dev1(&generator);

    SerialPort serial_port(buffer_size,port_name, &generator);// ИЗ ЗА ЗАЦИКЛИВАНИЯ НЕ РАБОТАЮТ ТАЙМЕРЫ
    if (serial_port.get_status())
    {
        qDebug()<<"Input device count: ";
        int device_count;
        cin>>device_count;
        ControlCenter control_center(device_count,&generator,&serial_port);
    }
    qDebug()<<"Program end :-)";

    /*****************************************************************************/
    //Блок тестирования
    /*****************************************************************************/

    //Device devices(&generator);

   /* int i;
    QVector<Device> devices(5);
    ifstream fin("devices.txt");
    if (fin)
    {
        qDebug()<<"File opening sucsessful!"<<'\n';
        QString buffer;
    for (i=0;i<5;i++)
    {
        fin>>buffer;
        devices.append(Device(buffer.split("   ")));  //Считывание с табуляцией
    }
    }
    else qDebug()<<"File device.txt  is not found!";*/
    return a.exec();//Если возвращать 0, то всё работает нормально
}

