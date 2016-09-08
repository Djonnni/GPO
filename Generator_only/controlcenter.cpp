#include <QDebug>
#include <conio.h>
#include <fstream>

#include "controlcenter.h"

const bool settings = true;

using namespace std;

ControlCenter::ControlCenter(int device_count, Generator *load_gen, SerialPort *load_serial_port, QObject *parent)
{    
    bool add_device = true;
    generator = load_gen;
    serial_port = load_serial_port;
    devices.resize(0);//= new QVector<Device*>(0);
    timers.resize(0);
    /******************************************************************/
    //Блок для тестирования
   // load_devices();
   // _getch();
    /******************************************************************/
    for (int i=0;i<device_count;i++)
    {
        qDebug()<<"...Put anything to continue...";
        _getch();
        Device *dev;
        dev = new Device(generator, serial_port);
        for (int j=0; j<(devices.size()); j++)
            if ((devices.at(j)->get_id()) == dev->get_id())
            {
                add_device = false;
                qDebug()<<"Device with id = "<<dev->get_id()<<" already in the list!";
                break;
            }
        if (add_device)
        {
          // qDebug()<<"Adding device...Put anything to continue...";
           //_getch();
           QTimer *timer;
           timer = new QTimer();
           timers.push_back(timer);
           devices.push_back(dev);
           //timers.

           connect(timers.at(timers.size()-1), SIGNAL(timeout()), devices.at(devices.size()-1), SLOT(send_packet()));
           //connect(timers.at(timers.size()-1), SIGNAL(timeout()),this , SLOT(send_packet(devices.at(timers.size()-1))));
           timers.at(timers.size()-1)->start((dev->get_time()%50+50)*100);
           qDebug()<<"Device "<<i<<"with id"<<dev->get_id()<<" added!"<<'\n';
        }
        add_device = true;        
    }

}

void ControlCenter::delete_device(unsigned long device_id)
{
    bool delete_dev = false;
    int i;
    for (i=0; i<devices.size(); i++)
      if (devices.at(i)->get_id() == device_id)
            {delete_dev = true;break;}
    if (delete_dev)
    {
          disconnect(timers.at(i), SIGNAL(timeout()), devices.at(i), SLOT(send_packet()));
          timers.at(i)->stop();
    }


    //******************************************
    //Блок проверки
    /*for (i=0; i<devices.size(); i++)
        qDebug()<<devices.at(i)->get_id();*/
    //******************************************
}

void ControlCenter::add_device(unsigned long int device_id)
{
    bool add_dev = true;
    int i;
    for (i=0; i<devices.size(); i++)
        if (devices.at(i)->get_id() == device_id)
              {add_dev = false;break;}
    if (add_dev)
    {
        generator->generate_random_packet(settings);
        Device *dev;
        dev = new Device(generator, serial_port);
        dev->set_id(device_id);
        QTimer *timer;
        timer = new QTimer();
        timers.push_back(timer);
        devices.push_back(dev);
        connect(timers.at(timers.size()-1), SIGNAL(timeout()), devices.at(devices.size()-1), SLOT(send_packet()));
        //connect(timers.at(timers.size()-1), SIGNAL(timeout()),this , SLOT(send_packet(devices.at(timers.size()-1))));
        timers.at(timers.size()-1)->start((dev->get_time()%50+50)*100);
        qDebug()<<"Device "<<i<<"with id"<<dev->get_id()<<" added!"<<'\n';
    }


}

void ControlCenter::connect_device(unsigned long device_id)
{
    bool connect_dev = false;
    int i;
    for (i=0; i<devices.size(); i++)
        if (devices.at(i)->get_id() == device_id)
              {connect_dev = true;break;}
    if (connect_dev)
    {
        disconnect(timers.at(i), SIGNAL(timeout()), devices.at(i), SLOT(send_packet()));
        timers.at(i)->start((devices.at(i)->get_time()%50+50)*100);
    }
}

void ControlCenter::load_devices()
{
    ifstream fin("devices.txt");
    if (fin) qDebug()<<"File opening sucsessful!!"<<'\n';
    int i=1;
    string container;
    getline(fin, container);//Считывание первой строки с описанием
    container.clear();
    devices.resize(0);
    timers.resize(0);
    while (!fin.eof())
    {
        unsigned long int load_id;
        unsigned char load_meas_type;
        unsigned int meas_buffer;//Для избежания проблем со считыванием символов
        unsigned long int load_value;
        unsigned int load_time;
        fin>>load_id;
        fin>>meas_buffer;
        load_meas_type = meas_buffer;
        fin>>load_value;
        fin>>load_time;
        qDebug()<<"Device #"<<i;
        qDebug()<<"id - "<<load_id;
        qDebug()<<"measure_type - "<<load_meas_type;
        qDebug()<<"value - "<<load_value;
        qDebug()<<"time - "<<load_time;
        Device *dev;
        dev = new Device(load_id, load_meas_type, load_value, load_time);
        QTimer *timer;
        timer = new QTimer();
        timers.push_back(timer);
        devices.push_back(dev);
        connect(timers.at(i-1), SIGNAL(timeout()), devices.at(i-1), SLOT(send_packet()));
        timers.at(i-1)->start((dev->get_time()%50+50)*100);
        qDebug()<<"Device "<<i<<"with id"<<dev->get_id()<<" added!"<<'\n';
        i++;
    }
    fin.close();
}






