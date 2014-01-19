#include "rpi_serial.h"
#include <QDebug>
#include <QList>
#include <QStringList>
#include "qextserialenumerator.h"
#include <rpi_sensors.h>
#include <QDateTime>

rpi_Serial::rpi_Serial(QObject *parent,QDateTime *s): QObject(parent){
    sPort=0;
    timer =0;
    ultrasonicsize =0;
    compass =0;
    accelerometer =0;
    ultrasonic =0;
    bpressure =0;
    starttime=s;

    //start();
    setupSerialPort();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getSensorValues()));
    timer->start(1000);
    
}
rpi_Serial::~rpi_Serial(){  
    if (sPort) delete sPort;
    sPort =0;
}


void rpi_Serial::run() {

        qDebug() << "Hello Serial";
        
}
void rpi_Serial::getSensorValues(){
    //! [2]
    qDebug() << "Get Values";
    /*
        qDebug() << "port name:"       << info.portName;
        qDebug() << "friendly name:"   << info.friendName;
        qDebug() << "physical name:"   << info.physName;
        qDebug() << "enumerator name:" << info.enumName;
        qDebug() << "vendor ID:"       << info.vendorID;
        qDebug() << "product ID:"      << info.productID;

        qDebug() << "===================================";
    }*/

    qint64 timefromStart;
    //double dbltime;
    timefromStart= starttime->msecsTo(QDateTime::currentDateTime());
    //dbltime = timefromStart/100.00;

    //qDebug() << "Time Since Start:" << dbltime;

    if (bpressure) { bpressure->setPressureTemperature(0,0);}
    //timefromStart= starttime->msecsTo(QDateTime::currentDateTime());
    //dbltime = timefromStart/100.00;
    if (compass) {compass->setAngle(0,0);}
    for (int j=0;j<4;j++) {
    //if (ultrasonic[j]) ultrasonic[j]->setDistance(0,0);
    }
}

void rpi_Serial::setupSerialPort() {
    //bool nport = false;
    
QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    //! [1]
    QStringList portNames;
    qDebug() << "List of ports:";
    foreach (QextPortInfo info, ports) {
    portNames << info.portName;
    }
    qDebug() << portNames;
    setPortName("COM19");
    if (!portNames.contains(portName)) 
        { 
            qDebug() << portName << " does exist"; 
            return;
        }
    if (!sPort) { sPort = new QextSerialPort(portName, QextSerialPort::/*Polling*/EventDriven/**/);  }else{
        sPort->setBaudRate(BAUD9600);
        sPort->setFlowControl(FLOW_OFF);
        sPort->setParity(PAR_NONE);
        sPort->setDataBits(DATA_8);
        sPort->setStopBits(STOP_1);
    }
    if (sPort->open(QIODevice::ReadWrite | QIODevice::Unbuffered ) == true) {
        qDebug("is open: %d", sPort->isOpen());
        connect(sPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(sPort, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
        if (!(sPort->lineStatus() & LS_DSR))
        qDebug() << "warning: device is not turned on";
        qDebug() << "listening for data on" << sPort->portName();
    }
    else {
        qDebug() << "device failed to open:" << sPort->errorString();
    }
    
}
void rpi_Serial::setPortName(QString p) {
    portName =p ;
}
void rpi_Serial::onReadyRead()
{
    QByteArray bytes;
    int a = sPort->bytesAvailable();
    bytes.resize(a);
    sPort->read(bytes.data(), bytes.size());
    qDebug() << "bytes read:" << bytes.size();
    qDebug() << "bytes:" << bytes;
}
void rpi_Serial::onDsrChanged(bool status)
{
    if (status)
        qDebug() << "device was turned on";
    else
        qDebug() << "device was turned off";
}
void rpi_Serial::setPressure(rpi_sensor_pressure *p) {bpressure=p;}
void rpi_Serial::setCompass(rpi_sensor_compass *c) {compass=c;}
void rpi_Serial::setAccelerometer(rpi_sensor_acc *a){accelerometer=a;}
void rpi_Serial::setUltraSonic(int i,rpi_sensor_ultrasonic *s){
    if (i>ultraSonicSize()) return;
    ultrasonic[i] = s;
}

void rpi_Serial::setUltraSonicSize(int s) {
    if (ultrasonic) delete [] ultrasonic;
    ultrasonic = new rpi_sensor_ultrasonic*[ultraSonicSize()];
    for (int k=0;k<ultraSonicSize();k++) ultrasonic[k] = 0;
}

const int rpi_Serial::ultraSonicSize() {
        return ultrasonicsize;
}
