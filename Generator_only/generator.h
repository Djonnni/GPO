#ifndef GENERATOR_H
#define GENERATOR_H

#include <QCoreApplication>

class Generator
{
public:
    Generator(bool settings);
    QByteArray packet;
    void showInfo();
    void generate_random_packet(bool settings=true);
    void generate_packet(unsigned long load_id, unsigned char load_measure_type, unsigned long int load_value, unsigned int load_time);
    void load_packets();//Не нужный элемент
    unsigned long int get_id();
    unsigned char get_meas_type();
    unsigned long int get_value();
    unsigned int get_time();
    QByteArray get_packet();
signals:

public slots:

private:
    unsigned long int id; //4 байта
    unsigned char measure_type;//1 байт
    unsigned long int value;//4 байта
    unsigned int time;      //2 байта
    unsigned char crc8;
    //Изменить на CRC8
    unsigned char calc_CRC(unsigned char *Data, unsigned long size);
    unsigned char calc_CRC8(unsigned char *data, unsigned char size);
    //char *MakeCRC(char *BitString);
};

#endif // GENERATOR_H
