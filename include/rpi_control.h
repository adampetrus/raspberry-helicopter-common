#ifndef RPI_CONTROL_H
#define RPI_CONTROL_H

#include "rpi_log.h"
#include "rpi_device.h"
#include <QString>


class rpi_control: public rpi_device {
public:
    enum SERVO_ADDRESS {
        RPI_SERVO_0 = 0x20,
        RPI_SERVO_1 = 0x21,
        RPI_SERVO_2 = 0x22,
        RPI_SERVO_3 = 0x23,
        RPI_SERVO_4 = 0x24,
        RPI_SERVO_5 = 0x25,
        RPI_SERVO_6 = 0x26
        };
    enum SERVO_TYPE {
        SERVO =1,
        BIMOTOR ,
        ESCMOTOR
    };
public:
    rpi_control();
    rpi_control(const rpi_control &c);
    //rpi_control(const int mode);
    /*virtual*/ bool setValue(uint v);//;
    /*virtual*/ uint readValue() {return 0;}//;
    ~rpi_control();
    void startLog(QString filename);
    void logEntry();
    void stopLog();
    //void setAddress(QString d,int node,int t);
    QString toXML(bool values=true,bool conf=true);
    void addtoDomDoc(QDomDocument &d,QDomElement &e,bool values =true,bool conf=true);
    void readXML(QString i);
    void readDomElement(QDomElement &e);
    uint value();
    void reset();
protected:
    uint value_v;
    uint mode_v;
    int *servo_value;
    int currentIndex;
    rpi_csv_log *log;
};

class rpi_servo : public rpi_control {
public:
    bool setValue(uint v);
    uint readValue();
};

class rpi_bimotor : public rpi_control {
public:
    bool setValue(uint v);
    uint readValue();
};

class rpi_escmotor : public rpi_control {
public:
    bool setValue(uint v);
    uint readValue();
};






#endif // RPI_CONTROL_H
