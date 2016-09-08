#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QTimer>

#include "generator.h"
#include "serialport.h"

using namespace std;

class Device : public QObject
{
    Q_OBJECT
public:
    Device(Generator *load_gen, SerialPort *load_serial_port, QObject *parent = 0);
    Device(unsigned long int load_id, unsigned char load_meas_type, unsigned long int load_value, unsigned long int load_time, QObject *parent = 0);
    void showInfo();
    unsigned long int get_id();
    unsigned char get_meas_type();
    unsigned int get_time();    
    unsigned long int get_value();
    void set_id(unsigned long int new_id);


signals:
     void war();
public slots:
     void send_packet();
     void test_slot();
private:
    Generator *generator;   //Используем указатель для того, чтобы не возникала ошибка с объявлением функций класса Generator
   //ControlCenter *control_center;
    //QTimer *timer;
    SerialPort *serial_port;
    unsigned long int id;       //4 байта
    unsigned char measure_type; //1 байт
    unsigned int time;          //2 байта
    unsigned long int value;//4 байта

};

#endif // DEVICE_H
