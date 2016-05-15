#ifndef RPI_MULTIWIIDATA_H
#define RPI_MULTIWIIDATA_H
#include <stdint.h>
#include <QList>
#include <QDomElement>
#include <QDateTime>
#include <QByteArray>
#include <QMap>
#include "rpi_device.h"




class mspGPSWayPoint {
public:
    mspGPSWayPoint();
    mspGPSWayPoint(const mspGPSWayPoint &a);
    ~mspGPSWayPoint();
    uint8_t gpsWPID;
    uint8_t gpsFix;
    uint8_t gpsNumSat;
    int32_t gpsLat;
    int32_t gpsLong;
    uint16_t gpsAlt;
    uint16_t gpsSpeed;
    uint16_t gpsCOG;
    uint32_t wpAltHold;
    uint16_t wpHeading;
    uint16_t wpTimeToStay;
    uint8_t wpNavFlag;


    QString toXML(bool values=true,bool conf=true);
    void readXML(QString i);
    void readDomElement(QDomElement &elem);
    void addtoDomDoc(QDomDocument &d,QDomElement &e,bool values =true,bool conf=true);


    uint8_t getGpsFix() const;
    void setGpsFix(const uint8_t &value);

    uint8_t getGpsNumSat() const;
    void setGpsNumSat(const uint8_t &value);

    int32_t getGpsLat() const;
    void setGpsLat(const int32_t &value);

    int32_t getGpsLong() const;
    void setGpsLong(const int32_t &value);

    uint16_t getGpsAlt() const;
    void setGpsAlt(const uint16_t &value);

    uint16_t getGpsSpeed() const;
    void setGpsSpeed(const uint16_t &value);

    uint16_t getGpsCOG() const;
    void setGpsCOG(const uint16_t &value);


    double getGpsLatDouble() const;
    void setGpsLatDouble(const double &value);

    double getGpsLongDouble() const;
    void setGpsLongDouble(const double &value);

    double getGpsAltDouble() const;
    void setGpsAltDouble(const double &value);

    double getGpsSpeedDouble() const;
    void setGpsSpeedDouble(const double &value);

    double getGpsCOGDouble() const;
    void setGpsCOG(const double &value);

};


class rpi_multiwiidata : public rpi_device
{
public:
    rpi_multiwiidata();
    ~rpi_multiwiidata();
    mspGPSWayPoint currentGPSPosition;
    QList<mspGPSWayPoint> wayPoints;

    QString toXML(bool values=true,bool conf=true);
    void readXML(QString i);
    void readDomElement(QDomElement &elem);
    void addtoDomDoc(QDomDocument &d,QDomElement &e,bool values =true,bool conf=true);

    static QStringList functionTest();

//multiwiidata;

    double pitch;
    double yaw;
    double roll;
    
    int sensorStatus;
    int *motorLevel;
    int *rcLevel;
    
    double angx;
    double angy;
    double heading;

    bool operator ==(const rpi_multiwiidata &b);


    QMap<QString,uint16_t> UnSigned16Bit;
    QMap<QString,int32_t> Signed32Bit;


};

#endif // RPI_MULTIWIIDATA_H
