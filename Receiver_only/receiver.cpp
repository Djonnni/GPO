#include "receiver.h"

//#include <iostream>
#include <fstream>
#include <conio.h>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include <math.h>

using namespace std;
Receiver::Receiver()
{
    data = new QVector<device_data>(0);
    devices_id = new QVector<unsigned long int>(0);
}

void Receiver::packet_parse(QByteArray packet)
{
    QByteArray buffer;  //Буфер
    int i=0;              //Счётчик
    bool ok;            //Для преобразования чисел в различные системы счисления
    //В начале проверим CRC8
    unsigned char packet_char[packet.size()]; //Копия пакета
    foreach (unsigned char symbol, packet)
    {
        packet_char[i] = symbol;
        if (i==packet.size()-1) break;
        else
        i++;
    }
    unsigned char calc_crc = calc_CRC8(packet_char, packet.size()-1);
    for (i=0; i<4; i++)
    {
        buffer.append(packet[0]);
        packet.remove(0,1);
    }
    id = buffer.toHex().toInt(&ok,16);
    buffer.clear();

    buffer.append(packet[0]);
    packet.remove(0,1);
    measure_type = buffer.toHex().toInt(&ok,16);
    buffer.clear();

    for (i=0; i<=1; i++)
    {
        buffer.append(packet[0]);
        packet.remove(0,1);
    }
    time = buffer.toHex().toInt(&ok,16);
    buffer.clear();

    for (i=0; i<4; i++)
    {
        buffer.append(packet[0]);
        packet.remove(0,1);
    }
    value = buffer.toHex().toInt(&ok,16);
    buffer.clear();

    buffer.append(packet[0]);
    packet.remove(0,1);
    crc8 = buffer.toHex().toInt(&ok,16);
    buffer.clear();
    qDebug()<<"\nProcessing packet ended sucsessful !\n";
    bool transfer_error = checking_CRC(calc_crc,crc8);
    if (!transfer_error)
        qDebug()<<"Mistakes in the message is not found! ";
    else
    {
        qDebug()<<"Mistake by transfer of the message was found!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    }

}

void Receiver::add_data(unsigned long id, unsigned char measure_type, unsigned long value, unsigned int time)
{
    int i;
    int data_size = data->size();
    bool add_data = true;
    for (i=0; i<data_size; i++)
    {
        if (id == data->at(i).id)
        {
            add_data = false;
            break;
        }
    }
    device_data new_data;
    new_data.id = id;
    new_data.measure_type = measure_type;
    new_data.value = value;
    new_data.time = time;
    //if (add_data)
    data->append(new_data);
    for (i=0; i<devices_id->size();i++)
        if (devices_id->at(i) == id)
        {i=-1; break;}
    if (i!=-1)
    devices_id->append(id);
   /* else
        data->replace(i, new_data);*/
    save_to_file(data);
    //*******************************
    //Блок проверки
    /*qDebug()<<"Devices id: ";
    for (i=0; i<devices_id->size();i++)
        qDebug()<<devices_id->at(i);*/
    //*******************************

}

void Receiver::showInfo()
{
    qDebug()<<"\nid - "<<id;
    qDebug()<<"measure_type - "<<measure_type;
    QTime time_buffer(time/3600,(time%3600)/60,time%60);
    qDebug()<<"time - "<<time_buffer.toString("hh:mm:ss");
    qDebug()<<"value - "<<value;
    qDebug()<<"CRC8 - "<<crc8<<'\n';

}

void Receiver::showData()
{
    int data_size = data->size();
    int i;
    QTime time_buffer;
    QString new_time;// = time_buffer.toString("hh:mm:ss");
    for (i=0; i<data_size; i++)
    {       
        qDebug()<<i+1<<" device:";
        qDebug()<<"id - "<<data->at(i).id;
        qDebug()<<"measure_type - "<<data->at(i).measure_type;
        time_buffer.setHMS(data->at(i).time/3600,(data->at(i).time%3600)/60,data->at(i).time%60);
        new_time = time_buffer.toString("hh:mm:ss");
        qDebug()<<"time - "<<new_time;
        //qDebug()<<"time - "<<data->at(i).time;
        //qDebug()<<"time - "<<data->at(i).time/3600<<':'<<(data->at(i).time%3600)/60<<':'<<data->at(i).time%60;
        qDebug()<<"value - "<<data->at(i).value;
        qDebug()<<"";

    }

}

unsigned char Receiver::calc_CRC8(unsigned char *data, unsigned long size)
{
    //Полином - x8 + x^5 + x^4 + 1 или 0011 0001
        int i,j=7, buffer;
        unsigned char current_byte[8];
        unsigned char CRC[8];
        unsigned char DoInvert;

        for (i=0; i<8; ++i)  CRC[i] = 0;    //Обнуляем буфер CRC

        for (i=0; i<size; i++)
        {
            buffer = data[i];
            while (j>=0)               //Формируем двоичное представление байта
            {
                if ((buffer - pow(2.0,j))>=0)
                {
                    buffer-=pow(2.0,j);
                    current_byte[7-j] = '1';
                    j--;
                }
                else
                {
                    current_byte[7-j] = '0';
                    j--;
                }
            }
            for (j = 0; j <8; j++)
            {
                DoInvert = ('1'==current_byte[j]) ^ CRC[7];         // XOR required?
                CRC[7] = CRC[6];
                CRC[6] = CRC[5];
                CRC[5] = CRC[4] ^ DoInvert;
                CRC[4] = CRC[3] ^ DoInvert;
                CRC[3] = CRC[2];
                CRC[2] = CRC[1];
                CRC[1] = CRC[0];
                CRC[0] = DoInvert;
            }
            j = 7;
        }
        buffer = 0;
        for (i=0; i<8; i++)
            if (CRC[i] == 1)
                buffer+=pow(2.0,i);
        return buffer;

}

bool Receiver::checking_CRC(unsigned char packet_crc8, unsigned char calc_crc8)
{
    if  (calc_crc8 == packet_crc8) return false;
    return true;
}

unsigned long Receiver::get_id()
{
    return id;
}

unsigned char Receiver::get_meas_type()
{
    return measure_type;
}

unsigned long Receiver::get_value()
{
    return value;
}

unsigned int Receiver::get_time()
{
    return time;
}

void Receiver::save_to_file(QVector<device_data> *new_data)
{
    ofstream f("new_data.txt",ios_base::app);
    if (f) qDebug()<<"File opening sucsessful!";
    else
    {
        qDebug()<<"File is not found!";
        _getch();
    }
    for (int i=0; i<new_data->size(); i++)
    {
        f<<new_data->at(i).id<<" "<<new_data->at(i).time<<" "<<new_data->at(i).value<<'\n';
    }
}

