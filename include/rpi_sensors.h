#ifndef RPI_SENSORS_H
#define RPI_SENSORS_H
#include "rpi_log.h"
#include "rpi_vector.h"
#include "rpi_device.h"
#include <QDomElement>
#include <QDateTime>


class rpi_sensor_compass : public rpi_device {
public:
    rpi_sensor_compass(const int mode);
    ~rpi_sensor_compass();
    enum SENSOR_MODE { SENSOR_MODE_CLIENT =1, SENSOR_MODE_SERVER =20 };
    void setAngle(double t,double p);
    double getTheta();
    double getPhi();
    void startLog(QString filename);
    void logEntry();
    void stopLog();
    QString toXML(bool values=true,bool conf=true);
    void addtoDomDoc(QDomDocument &d,QDomElement &e,bool values =true,bool conf=true);
    void readXML(QString i);
    void readDomElement(QDomElement &elem);
    bool test();
private:
    int cMode;
    double *theta;
    double *phi;
    int currentIndex;
    rpi_csv_log *log;

};


class rpi_sensor_pressure: public rpi_device{
public :
    rpi_sensor_pressure(const int mode);
    ~rpi_sensor_pressure();
    enum SENSOR_MODE { SENSOR_MODE_CLIENT =1, SENSOR_MODE_SERVER =20 };
    void setPressureTemperature(double p,double t);
    double getPressure();
    double getTemperature();
    void startLog(QString filename);
    void logEntry();
    void stopLog();
    QString toXML(bool values=true,bool conf=true);
    void addtoDomDoc(QDomDocument &d,QDomElement &e,bool values =true,bool conf=true);
    void readXML(QString i);
    void readDomElement(QDomElement &elem);
    bool test();
    double getSeaLevelPressure();
    void setSeaLevelPressure(double slp);
    void measure();

private:
    int cMode;
    double *pressure;
    double seaLevelPressure_v;
    double *temperature;
    int currentIndex;
    rpi_csv_log *log;
};

class rpi_sensor_gps: public rpi_device{
public:
    rpi_sensor_gps(const int mode);
    enum SENSOR_MODE { SENSOR_MODE_CLIENT =1, SENSOR_MODE_SERVER =20 };

    void setGPS(double lat,double longit,double h,QDateTime time);
    double getLatitude();
    double getLongitude();
    double getHeight();
    QDateTime getTime();
    rpi_sensor_compass *compass;
    ~rpi_sensor_gps();
    void startLog(QString filename);
    void logEntry();
    void stopLog();
    const qlonglong size();
    QString toXML(bool values=true,bool conf=true);
    void addtoDomDoc(QDomDocument &d,QDomElement &e,bool values =true,bool conf=true);
    void readXML(QString i);
    void readDomElement(QDomElement &elem);
    bool test();
private:
    qlonglong sensorSize;
    QDateTime *date;
    double *latitude;
    double *longitude;
    double *height;
    double *time;
    int cMode;
    int currentIndex;
    rpi_csv_log *log;
};


class rpi_sensor_acc : public rpi_device{
public:
    rpi_sensor_acc(const int mode);
    enum SENSOR_MODE { SENSOR_MODE_CLIENT =1, SENSOR_MODE_SERVER =20 };
    void setPosition(double x, double y,double z);
    void setVelocity(double x, double y,double z);
    void setAcceleration(double x, double y,double z,QDateTime t);
    void startLog(QString filename);
    void logEntry();
    void stopLog();
    double currentTime();

    double currentxPosition();
    double currentyPosition();
    double currentzPosition();

    double currentxVelocity();
    double currentyVelocity();
    double currentzVelocity();

    double currentxAcceleration();
    double currentyAcceleration();
    double currentzAcceleration();
    void readXML(QString i);
    void readDomElement(QDomElement &elem);
    bool test();
    void reset();
    void addtoDomDoc(QDomDocument &d,QDomElement &e,bool values =true,bool conf=true);
    QString toXML(bool values=true,bool conf=true);
    QDateTime& ZeroTime();
private:
    int cMode;
    QDateTime zeroTime;
    double *time;
    double **acceleration;
    double **velocity;
    double **position;
    int currentIndex;
    rpi_csv_log *log;

};


class rpi_sensor_gyro: public rpi_device{
public:
    rpi_sensor_gyro(const int mode);
    enum SENSOR_MODE { SENSOR_MODE_CLIENT =1, SENSOR_MODE_SERVER =20 };
    void setAngle(double t, double p,double s);
    void setVelocity(double t, double p,double s);
    void setAcceleration(double t, double p,double s,QDateTime d);
    void setQuaternion(double *q);
    void startLog(QString filename);
    void logEntry();
    void stopLog();
    double currentTime();

    double currentThetaAngle();
    double currentPhiAngle();
    double currentSigmaAngle();

    double currentThetaVelocity();
    double currentPhiVelocity();
    double currentSigmaVelocity();

    double currentThetaAcceleration();
    double currentPhiAcceleration();
    double currentSigmaAcceleration();
    double currentQuaternion(const int i);
    
    void readXML(QString i);
    void readDomElement(QDomElement &elem);
    bool test();
    void reset();
    void addtoDomDoc(QDomDocument &d,QDomElement &e,bool values =true,bool conf=true);
    QString toXML(bool values=true,bool conf=true);
    QDateTime& ZeroTime();
private:
    int cMode;
    QDateTime zeroTime;
    double *time;
    double **acceleration;
    double **velocity;
    double **angle;
    double **quaternion;
    
    int currentIndex;
    rpi_csv_log *log;

};



class rpi_sensor_ultrasonic : public rpi_device{
public:
    enum { SENSOR_MODE_CLIENT =1, SENSOR_MODE_SERVER =20 };
    rpi_sensor_ultrasonic(const int mode,QDateTime z);
    void setDistance(double d,QDateTime t);
    double getDistance();
    void addtoDomDoc(QDomDocument &d,QDomElement &e,bool values =true,bool conf=true);
    QString toXML(bool values=true,bool conf=true);
    void readXML(QString i);
    ~rpi_sensor_ultrasonic();
    void readDomElement(QDomElement &elem);
    QDateTime& ZeroTime();
private:
    int currentIndex;
    QDateTime zeroTime;
    int cMode;
    double *time;
    double *distance;
};


#endif // RPI_SENSORS_H
