#include <QCoreApplication>
#include <QDebug>
#include <QTime>
#include <math.h>
#include <fstream>
#include <string>
#include <iostream>
#include <QSettings>

#include "generator.h"

typedef unsigned char BYTE;
typedef unsigned long ULONG;
typedef unsigned short WORD;
using namespace std;



Generator::Generator(bool settings)
{
   this->generate_random_packet();
}



void Generator::showInfo()
{
    qDebug()<<"\nid - "<<id;
    qDebug()<<"measure_type - "<<measure_type;
    qDebug()<<"value - "<<value;
    QTime time_buffer(time/3600,(time%3600)/60,time%60);
    qDebug()<<"time - "<<time_buffer.toString("hh:mm:ss");
    qDebug()<<"CRC8 - "<<crc8<<'\n';
}

void Generator::generate_random_packet(bool settings)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    //Если есть настройки
    //Доделать в случае необходимости
    if (settings)
    {
        int start_value;
        int last_value;
        int divisibility;
        int i;
        QSettings *settings_load = new QSettings("settings.ini",QSettings::IniFormat);
        QString parametr_type;
        for (i=0;i<4;i++)
        {
            switch (i)
            {
            case 0:
                parametr_type = "Id";
                start_value = settings_load->value(parametr_type+"/StaringValue","None").toInt();
                last_value = settings_load->value(parametr_type+"/FinalValue", "None").toInt();
                divisibility = settings_load->value(parametr_type+"/Divisibility", "None").toInt();
                id = (qrand())%(last_value+1)/divisibility*divisibility;
                if (id<(unsigned long int)(start_value)) id = last_value/divisibility*divisibility;
                break;
            case 1:
                parametr_type = "Measure_type";
                start_value = settings_load->value(parametr_type+"/StaringValue","None").toInt();
                last_value = settings_load->value(parametr_type+"/FinalValue", "None").toInt();
                divisibility = settings_load->value(parametr_type+"/Divisibility", "None").toInt();
                measure_type = (qrand())%(last_value+1)/divisibility*divisibility;
                if (measure_type<(unsigned char)(start_value)) measure_type = last_value/divisibility*divisibility;
                break;
            case 2:
                parametr_type = "Time";
                start_value = settings_load->value(parametr_type+"/StaringValue","None").toInt();
                last_value = settings_load->value(parametr_type+"/FinalValue", "None").toInt();
                divisibility = settings_load->value(parametr_type+"/Divisibility", "None").toInt();
                time = (qrand())%(last_value+1)/divisibility*divisibility;
                if (time<(unsigned int)(start_value)) time = last_value/divisibility*divisibility;
                break;
            case 3:
                parametr_type = "Value";
                start_value = settings_load->value(parametr_type+"/StaringValue","None").toInt();
                last_value = settings_load->value(parametr_type+"/FinalValue", "None").toInt();
                divisibility = settings_load->value(parametr_type+"/Divisibility", "None").toInt();
                value = (qrand())%(last_value+1)/divisibility*divisibility;
                if (value<(unsigned long int)(start_value)) value = last_value/divisibility*divisibility;
                break;
            }
        }
        /*int id_start_value = settings_load->value("Id/StaringValue","None").toInt();
        int id_last_value = settings_load->value("Id/FinalValue", "None").toInt();
        int id_divisibility = settings_load->value("Id/Divisibility", "None").toInt();
        id = (qrand())%(id_last_value+1)/id_divisibility*id_divisibility;
        //id = (qrand())*id_divisibility % id_last_value;
        if (id<id_start_value) id = id_last_value/id_divisibility*id_divisibility;
        measure_type = qrand();
        time = qrand();
        value = qrand();*/
    }
    else
    {
        id = qrand();
        measure_type = qrand();
        time = qrand();
        value = qrand();
    }
    //packet.clear();
    generate_packet(id, measure_type, value, time);
   /* id = qrand();
    measure_type = qrand();
    time = qrand();
    value = qrand();*/
    //ФОРМИРОВАНИЕ ПАКЕТА
    /*
    unsigned char buffer; //8-битовый буфер для формирования пакета
    int i;                //Счётчик
    for (i=3; i>=0; i--)
    {
        buffer = (id>>(8*i));
        packet.append(buffer);
    }
    packet.append(measure_type);
    for (i=1; i>=0; i--)
    {
        buffer = (time>>(8*i));
        packet.append(buffer);
    }

    for (i=3; i>=0; i--)
    {
        buffer = (value>>(8*i));
        packet.append(buffer);
    }
    //Пакет сформирован, остаётся только рассчитать контрольную сумму
    //Разработать рассчёт CRC8 (1-wire)
    unsigned char packet_size = packet.length();
    unsigned char packet_char[packet_size+1];
    //Переписывает текст сообщения в массив unsigned char для функции расчёта CRC8
    foreach (unsigned char symbol, packet)
    {
        packet_char[i] = symbol;
        if (i==packet_size-1) break;
        else
        i++;
    }
    crc8 = calc_CRC8(packet_char,packet_size-1);
    packet.append(crc8);
   // qDebug()<<"\nPacket creating sucsessful!";
    //qDebug()<<"Hex: "<<packet.toHex()<<'\n';*/
    //КОНЕЦ ФОРМИРОВАНИЯ ПАКЕТА
}

void Generator::generate_packet(unsigned long load_id, unsigned char load_measure_type, unsigned long load_value, unsigned int load_time)
{
    packet.clear();
    unsigned char buffer; //8-битовый буфер для формирования пакета
    int i;                //Счётчик
    for (i=3; i>=0; i--)
    {
        buffer = (load_id>>(8*i));
        packet.append(buffer);
    }
    packet.append(load_measure_type);
    for (i=1; i>=0; i--)
    {
        buffer = (load_time>>(8*i));
        packet.append(buffer);
    }

    for (i=3; i>=0; i--)
    {
        buffer = (load_value>>(8*i));
        packet.append(buffer);
    }
    //Пакет сформирован, остаётся только рассчитать контрольную сумму
    //Разработать рассчёт CRC8 (1-wire)
    unsigned char packet_size = packet.length();
    unsigned char packet_char[packet_size+1];
    //Переписывает текст сообщения в массив unsigned char для функции расчёта CRC8
    foreach (unsigned char symbol, packet)
    {
        packet_char[i] = symbol;
        if (i==packet_size-1) break;
        else
        i++;
    }
    crc8 = calc_CRC8(packet_char,packet_size-1);
    packet.append(crc8);
   // qDebug()<<"\nPacket creating sucsessful!";
    //qDebug()<<"Hex: "<<packet.toHex()<<'\n';
}

void Generator::load_packets()
{
    ifstream fin("Generator.txt");
    if (fin) qDebug()<<"File opening sucsessful!"<<'\n';
    int i=1;
    string container;
    getline(fin, container);
    container.clear();
    while (!fin.eof())
    {
        cout<<"Counter #"<<i<<'\n';
        fin>>container;
        cout<<"id - "<<container<<'\n';
        fin>>container;
        cout<<"measure_type - "<<container<<'\n';
        fin>>container;
        cout<<"time - "<<container<<'\n';
        fin>>container;
        cout<<"value - "<<container<<'\n';
        fin>>container;
        cout<<"CRC8 - "<<container<<'\n'<<'\n';
        i++;
    }
    fin.close();
}

unsigned long Generator::get_id()
{
    return id;
}

unsigned char Generator::get_meas_type()
{
    return measure_type;
}

unsigned long Generator::get_value()
{
    return value;
}

unsigned int Generator::get_time()
{
    return time;
}

QByteArray Generator::get_packet()
{
    return packet;
}

unsigned char Generator::calc_CRC(unsigned char *Data, unsigned long size)
{
    WORD w;
    char shift_cnt;
    BYTE *ptrByte;
    ULONG byte_cnt = size;
    ptrByte = Data;
    w = 0xffffU;
    for (; byte_cnt>0; byte_cnt--)
    {
        w = (unsigned short)
            ((w / 256U) * 256U + ((w % 256U) ^ (*ptrByte++)));
        for (shift_cnt = 0; shift_cnt<8; shift_cnt++)
        {/*обработка байта*/
            if ((w & 0x1) == 1)
                w = (unsigned short)((w >> 1) ^ 0xa001U);
            else
                w >>= 1;
        }
    }
    return w;
}



unsigned char Generator::calc_CRC8(unsigned char *data, unsigned char size)
{//Полином - x8 + x^5 + x^4 + 1 или 0011 0001
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




