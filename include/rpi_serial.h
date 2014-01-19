#ifndef RPI_SERIAL_H_
#define RPI_SERIAL_H_
#include <QObject>
#include "qextserialport.h"
#include <QThread>
#include <QTimer>
#include <QDateTime>

class rpi_sensor_compass;
class rpi_sensor_gps;
class rpi_sensor_acc;
class rpi_sensor_ultrasonic;
class rpi_sensor_pressure;


class rpi_Serial : public QObject /*QThread*/ /*QTimer*/
{
    Q_OBJECT;
public:
    
    QextSerialPort *sPort;
    rpi_Serial(QObject *parent=0,QDateTime *t=0);
    ~rpi_Serial();
    QTimer *timer;
    QString portName;
public slots:
    void run();
    void getSensorValues();
    void setPortName(QString p);
    void setupSerialPort();
    void onReadyRead();
    void onDsrChanged(bool status);
    void setPressure(rpi_sensor_pressure *p);
    void setCompass(rpi_sensor_compass *c);
    void setAccelerometer(rpi_sensor_acc *a);
    void setUltraSonic(int i,rpi_sensor_ultrasonic *s);
    void setUltraSonicSize(int s);
    const int ultraSonicSize();

private:
    QDateTime *starttime;
    int ultrasonicsize;
    rpi_sensor_compass *compass;
    rpi_sensor_acc *accelerometer;
    rpi_sensor_ultrasonic **ultrasonic;
    rpi_sensor_pressure *bpressure;
} ;

#endif
