#include <QDomElement>
#include <QDateTime>
#include <QByteArray>
#ifndef RPI_SENSORS_H
#define RPI_SENSORS_H
#include "rpi_log.h"
#include "rpi_vector.h"
#include "rpi_device.h"
inline void quaternionToEuler(float *q, float *euler);

class rpi_sensor_compass : public rpi_device {
public:
    rpi_sensor_compass(const int mode);
    ~rpi_sensor_compass();
    //enum SENSOR_MODE { SENSOR_MODE_CLIENT =1, SENSOR_MODE_SERVER =20 };
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
    //enum SENSOR_MODE { SENSOR_MODE_CLIENT =1, SENSOR_MODE_SERVER =20 };
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
    //enum SENSOR_MODE { SENSOR_MODE_CLIENT =1, SENSOR_MODE_SERVER =20 };

    void setGPS(double lat,double longit,double h,QDateTime time);
	void setGPS(double lat,double longit,double h,double sp,int nSats,QDateTime time);
	
    double &getLatitude();
    double &getLongitude();
    double &getHeight();
    double &getAccuracy();
	double &getSpeed();
	


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
    void nextValue();
	int isFixed;
	int noSatellites;
private:
    qlonglong sensorSize;
    QDateTime *date;
    double *latitude;
    double *longitude;
    double *height;
	double *speed;
    double *accuracy;
    double *time;
    int cMode;
    int currentIndex;

    rpi_csv_log *log;
};


class rpi_sensor_acc : public rpi_device{
public:
    rpi_sensor_acc(const int mode);
    //enum SENSOR_MODE { SENSOR_MODE_CLIENT =1, SENSOR_MODE_SERVER =20 };
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

double *quaternionNode(QDomNode n);

class rpi_sensor_ultrasonic : public rpi_device{
public:
    //enum { SENSOR_MODE_CLIENT =1, SENSOR_MODE_SERVER =20 };
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


class rpi_imu_9dof : public rpi_device {
public:
    rpi_imu_9dof(const rpi_imu_9dof &srcI);
    rpi_imu_9dof(const int mode,QDateTime z);
    ~rpi_imu_9dof();
    void setQuaternion(const float q1,const float q2,const float q3,const float q4);
    void setQuaternion(const float *q);
    float getQuaternion(int k,int i=-1);
    float getEuler(int k,int i=-1);

    /** Set Quaternion floating point, Accelerometer integer, Gyroscope integer, Magnetometer integer value
    by a hex string comma seperated string     **/
    void setQuatFAccelIGyroIMagnIByHexMessage(QByteArray &qData);
    void setQuatFByHexMessage(QByteArray &qData);
    void addtoDomDoc(QDomDocument &d,QDomElement &e,bool values =true,bool conf=true);
    QString toXML(bool values=true,bool conf=true);
    void readXML(QString i);
    void setMagn(const int x,const int y,const int z);
    void setGyro(const int x,const int y,const int z);
    void setAccel(const int x,const int y,const int z);

    void nextIndex();
    float *getCurrentQuaternionPointer();
    int *getCurrentMagnPointer();
    int *getCurrentAccelPointer();
    int *getCurrentGyroPointer();
    void readDomElement(QDomElement &elem);
    void quat2euler();
    static QStringList functionTest();
    bool operator==(const rpi_imu_9dof &i);

    void setHexMode(QByteArray &qData);
    int hexMode;
    int motorValue1;
    int motorValue2;
    int motorValue3;
    int motorValue4;


private:
    int currentIndex;
    float **quaternion;
    float **euler;
    double *time;
    int **gyro;
    int **magn;
    int **accel;
    int cMode;





};

#endif // RPI_SENSORS_H
