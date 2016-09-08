#ifndef CONTROLCENTER_H
#define CONTROLCENTER_H

#include <QObject>
#include <vector>


#include "device.h"
#include "generator.h"
#include "serialport.h"



using namespace std;

class ControlCenter : public QObject
{
    Q_OBJECT
public:
    ControlCenter(int device_count, Generator *load_gen, SerialPort *load_serial_port, QObject *parent = 0);
    void creating_devices(int count);
    void add_count(int count);
    void delete_device(unsigned long device_id);
    void add_device(unsigned long device_id);
    void connect_device(unsigned long  device_id);
    void load_devices();
    //void delete_count(int count[]);

signals:

public slots:    
private:
    //Device *devices;
    vector<Device*> devices;
    Generator *generator;
    SerialPort *serial_port;
    vector<QTimer*> timers;
    //int **table;
};

#endif // CONTROLCENTER_H
