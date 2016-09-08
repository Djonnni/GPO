#ifndef RECEIVER_H
#define RECEIVER_H

#include <QCoreApplication>

struct device_data
{
    unsigned long int id; //4 байта
    unsigned char measure_type;//1 байт
    unsigned long int value;//4 байта
    unsigned int time;      //2 байта
};

class Receiver
{
public:
    Receiver();
    void packet_parse(QByteArray packet);
    void add_data(unsigned long int id, unsigned char measure_type, unsigned long int value, unsigned int time);
    void showInfo();
    void showData();
    void exec_query();//Заготовка на будущее
    unsigned long int get_id();
    unsigned char get_meas_type();
    unsigned long int get_value();
    unsigned int get_time();
    void save_to_file(QVector<device_data> *new_data);
signals:

public slots:

private:
    unsigned long int id; //4 байта
    unsigned char measure_type;//1 байт
    unsigned long int value;//4 байта
    unsigned int time;      //2 байта
    unsigned char crc8;
    QVector<unsigned long int> *devices_id;
    QVector<device_data> *data;
    //device_data *data;
    //Изменить на CRC8
    unsigned char calc_CRC8(unsigned char *data, unsigned long size);
    QString create_query();     //Заготовка на будущее
    bool checking_CRC(unsigned char packet_crc8, unsigned char calc_crc8);



};

#endif // RECEIVER_H
