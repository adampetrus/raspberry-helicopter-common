#include "rpi_sensors.h"
#include <math.h>
#include <QDateTime>
#include <QDomDocument>
#include <QDebug>


const char qdatetimestring[] = "dd.MM.yyyy hh:mm:ss.zzz";



inline void quaternionToEuler(float *q, float *euler) {
  euler[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1); // psi
  euler[1] = -asin(2 * q[1] * q[3] + 2 * q[0] * q[2]); // theta
  euler[2] = atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1); // phi
}


QString nowString() {
    QDateTime n = QDateTime::currentDateTime();
    QString r = n.toString("d-M-yy hh:mm:ss.zzz");
    int s = n.time().msec();
    r.append(",").append(s);
    return r;
}

rpi_sensor_compass::rpi_sensor_compass(const int mode) {
    cMode = mode;
    theta =new double [mode];
    phi =new double [mode];
    currentIndex =0;\
    setType(rpi_device::RPI_COMPASS);
    if (cMode == SENSOR_MODE_CLIENT) currentIndex =0;
}
rpi_sensor_compass::~rpi_sensor_compass() {
    delete [] phi;
    delete [] theta;
}



void rpi_sensor_compass::setAngle(double t,double p) {
    currentIndex = (currentIndex+1)%cMode;
    theta[currentIndex] = t;
    phi[currentIndex] = p;
}
double rpi_sensor_compass::getTheta() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return theta[currentIndex];}
double rpi_sensor_compass::getPhi() {return phi[currentIndex]; }
void rpi_sensor_compass::startLog(QString filename) {
    filename ="";
}

void rpi_sensor_compass::readXML(QString i) {
    QDomDocument xmlDoc("Compass");
    xmlDoc.setContent(i);

    QDomElement docElem = xmlDoc.documentElement();
    readDomElement(docElem);
}
void rpi_sensor_compass::readDomElement(QDomElement &elem){
    double ttheta =0;
    double tphi =0;

    QDomNode n = elem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            if (e.tagName() == "Theta") {
                ttheta = fromHex(e.text().toLocal8Bit());
                //qDebug() << "pressure" << e.text() << tpress;
            }
            if (e.tagName() == "Phi") {
                tphi = fromHex(e.text().toLocal8Bit());
            }
            setAngle(ttheta,tphi);
            //setPressureTemperature(tpress,ttemp);
            //cout << qPrintable(e.tagName()) << endl; // the node really is an element.
        }
        n = n.nextSibling();
    }

}



bool rpi_sensor_compass::test() {
    double egTheta = double( rand())/ double(RAND_MAX ) * 180.0;
    double egPhi = double( rand())/ double(RAND_MAX ) * 360;

    setAngle(egTheta,egPhi);
    QString xmlStr = toXML();
    setAngle(0,0);
    readXML(xmlStr);

    if ( (getTheta() == egTheta) && (getPhi() == egPhi)) return true;
    qDebug() << xmlStr;
    qDebug() << "Compass Failure";
    qDebug() << egTheta << egTheta;
    qDebug() << getTheta() << getPhi();

    return false;
}







void rpi_sensor_compass::logEntry() {}
void rpi_sensor_compass::stopLog() {}


QString rpi_sensor_compass::toXML(bool values,bool conf) {

    QDomDocument xmlDoc("Compass");
    QDomElement root = xmlDoc.createElement("Compass");
    xmlDoc.appendChild(root);
    addtoDomDoc(xmlDoc,root,values,conf);
    return xmlDoc.toString();
}
void rpi_sensor_compass::addtoDomDoc(QDomDocument &d,QDomElement &e,bool values,bool conf){
    rpi_device::addtoDomDoc(d,e,conf);
    if (values) {
        QDomElement xtheta = d.createElement("Theta");
        QDomText ttheta = d.createTextNode(QString(fromDouble(getTheta())));
        QDomElement xphi = d.createElement("Phi");
        QDomText tphi = d.createTextNode(QString(fromDouble(getPhi())));
        e.appendChild(xtheta);
        e.appendChild(xphi);
        xtheta.appendChild(ttheta);
        xphi.appendChild(tphi);
    }
}


rpi_sensor_pressure::rpi_sensor_pressure(const int mode) {
    cMode = mode;
    pressure =new double [mode];
    temperature =new double [mode];
    currentIndex =0;
    log =0;
    setType(rpi_device::RPI_PRESSURE);

}
rpi_sensor_pressure::~rpi_sensor_pressure() {
    delete [] pressure;
    delete [] temperature;
}
void rpi_sensor_pressure::setPressureTemperature(double t,double p) {
    currentIndex = (currentIndex+1) % cMode ;
    pressure[currentIndex] = p;
    temperature[currentIndex] = t;
}

double rpi_sensor_pressure::getSeaLevelPressure() {
    return seaLevelPressure_v;
}
void rpi_sensor_pressure::setSeaLevelPressure(double slp){
    seaLevelPressure_v = slp;
}
#ifndef DEV_MS5611_I2C
void rpi_sensor_pressure::measure(){}
#endif

bool rpi_sensor_pressure::test() {
    double egTemp = double( rand())/ double(RAND_MAX ) * 21 + 273;
    double egPress = double( rand())/ double(RAND_MAX ) * 10 +1010;

    setPressureTemperature(egPress,egTemp);
    //qDebug() << getPressure() << getTemperature();
    QString xmlStr = toXML();
    //qDebug() << xmlStr;
    setPressureTemperature(0,0);
    readXML(xmlStr);
    //qDebug() << getPressure() << getTemperature();

    if ( (getPressure() == egPress) && (getTemperature() == egTemp)) return true;
    qDebug() << "Pressure / Temperature Failure";
    qDebug() << egTemp << egPress;
    qDebug() << getPressure() << getTemperature();

    return false;
}

double rpi_sensor_pressure::getPressure() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return pressure[currentIndex]; }
double rpi_sensor_pressure::getTemperature() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return temperature[currentIndex]; }
void rpi_sensor_pressure::startLog(QString filename) {
    if (log) delete log;
    log = new rpi_csv_log(filename);
    log->write("#Date/Time,Pressure,Temperature");
}
void rpi_sensor_pressure::logEntry() {
    if (!log) return;
    log->nextString(nowString());
    log->nextDouble(getPressure());
    log->nextDouble(getTemperature());
    log->newLine();
}
void rpi_sensor_pressure::stopLog() {
    if (log) delete log;
    log =0;
}
QString rpi_sensor_pressure::toXML(bool values,bool conf) {

    QDomDocument xmlDoc("BarometricPressure");
    QDomElement root = xmlDoc.createElement("BarometricPressure");
    xmlDoc.appendChild(root);
    addtoDomDoc(xmlDoc,root,values,conf);
    return xmlDoc.toString();
}
void rpi_sensor_pressure::addtoDomDoc(QDomDocument &d,QDomElement &e,bool values,bool conf){
    rpi_device::addtoDomDoc(d,e,conf);
    if (values) {
    QDomElement xPressure = d.createElement("Pressure");
    QDomText tPressure = d.createTextNode(QString(fromDouble(getPressure())));
    QDomElement xSeaLevelPressure = d.createElement("SeaLevelPressure");
    QDomText tSeaLevelPressure = d.createTextNode(QString(fromDouble(getSeaLevelPressure())));


    QDomElement xTemperature = d.createElement("Temperature");
    QDomText tTemperature = d.createTextNode(QString(fromDouble(getTemperature())));

    e.appendChild(xPressure);
    e.appendChild(xTemperature);

    xPressure.appendChild(tPressure);
    xTemperature.appendChild(tTemperature);

    e.appendChild(xSeaLevelPressure);
    xSeaLevelPressure.appendChild(tSeaLevelPressure);

    }
    //d.appendChild(e);
}

void rpi_sensor_pressure::readXML(QString i) {
    QDomDocument xmlDoc("BarometricPressure");
    xmlDoc.setContent(i);

    QDomElement docElem = xmlDoc.documentElement();
    readDomElement(docElem);
}

QDateTime& rpi_sensor_gyro::ZeroTime() {
    return zeroTime;
}


QDateTime& rpi_sensor_acc::ZeroTime() {
    return zeroTime;
}

QDateTime& rpi_sensor_ultrasonic::ZeroTime() {
    return zeroTime;
}

void rpi_sensor_pressure::readDomElement(QDomElement &elem){
    double tpress =0;
    double ttemp =0;
    double tseap =0;
    QDomNode n = elem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            if (e.tagName() == "Pressure") {
                tpress = fromHex(e.text().toLocal8Bit());
                //qDebug() << "pressure" << e.text() << tpress;
            }
            if (e.tagName() == "Temperature") {
                ttemp = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "SeaLevelPressure") {
                tseap = fromHex(e.text().toLocal8Bit());
            }

            setPressureTemperature(tpress,ttemp);
            setSeaLevelPressure(tseap);
            //cout << qPrintable(e.tagName()) << endl; // the node really is an element.
        }
        n = n.nextSibling();
    }
    rpi_device::readDomElement(elem);
}

rpi_sensor_gps::rpi_sensor_gps(const int mode){
    /*if (mode >0) {
        sensorSize =1;
    }else{
        sensorSize = mode;
    }*/
    cMode = mode;
    date = new QDateTime[mode];
    latitude = new double[mode];
    longitude = new double[mode];
    height = new double[mode];
    accuracy = new double[mode];
	speed = new double[mode];

    currentIndex =0;
    setType(rpi_device::RPI_GPS);
}

void rpi_sensor_gps::readXML(QString i) {
    QDomDocument xmlDoc("GPS");
    xmlDoc.setContent(i);

    QDomElement docElem = xmlDoc.documentElement();
    readDomElement(docElem);
}

void rpi_sensor_gps::nextValue() {
    currentIndex = ++currentIndex % cMode;
}



void rpi_sensor_gps::readDomElement(QDomElement &elem){

    /*double tlatitude =0;
     *
    double tlongitude =0;
    double theight =0;
    QDateTime ttime;*/
    QDomNode n = elem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "Latitude") {
                latitude[currentIndex] = fromHex(e.text().toLocal8Bit());
                //qDebug() << "pressure" << e.text() << tpress;
            }
            if (e.tagName() == "Longitude") {
                longitude[currentIndex] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "Height") {
                height[currentIndex] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "Date") {
                //qDebug() << e.text();
                date[currentIndex] =QDateTime::fromString(e.text(),qdatetimestring);
                //qDebug() << date[currentIndex] ;
            }



            //setPressureTemperature(tpress,ttemp);
            //cout << qPrintable(e.tagName()) << endl; // the node really is an element.
        }
        n = n.nextSibling();
    }
    rpi_device::readDomElement(elem);
}



bool rpi_sensor_gps::test() {
    double egLong = double( rand())/ double(RAND_MAX ) * 360.0 -180.0;
    double egLat= double( rand())/ double(RAND_MAX ) * 180.0 -90.0;
    double egHeight = double( rand())/ double(RAND_MAX ) * 100;
	double egSpeed = double( rand())/ double(RAND_MAX ) * 100;
	
    QDateTime now = QDateTime::currentDateTime();
    //setAngle(egTheta,egPhi);

    setGPS(egLat,egLong,egHeight,egSpeed,4,now);
    QString xmlStr = toXML();
    /*setGPS(0,0,0,QDateTime()
           .fromString("01.01.1900 00:00:00.000",qdatetimestring));
    readXML(xmlStr);

    if ( (getLatitude() == egLat) && (getLongitude() == egLong)
         && (getHeight() == egHeight) && (getTime() == now)) return true;


	*/
    qDebug() << xmlStr;
	
    qDebug() << "GPS Failure";
    qDebug() << getLatitude() << egLat ;//<<(getLatitude() == egLong)
    qDebug() << getLongitude() << egLong;
    qDebug() << getHeight() << egHeight;
    qDebug() << getTime().toString(qdatetimestring) << now.toString(qdatetimestring);


    return false;
}



const qlonglong rpi_sensor_gps::size(){return sensorSize;}

void rpi_sensor_gps::setGPS(double lat,double longit,double h,QDateTime time) {
    currentIndex = currentIndex++ % cMode;
    longitude[currentIndex] = longit;
    latitude[currentIndex] = lat;
    height[currentIndex] = h;
    date[currentIndex] = time;

    double xyz[3];
    double trueradius = earth_radius() + h;
}

void rpi_sensor_gps::setGPS(double lat,double longit,double h,double sp,int nSats,QDateTime time){
	    currentIndex = currentIndex++ % cMode;
    longitude[currentIndex] = longit;
    latitude[currentIndex] = lat;
    height[currentIndex] = h;
	speed[currentIndex] =sp;
	noSatellites = nSats;
	if (noSatellites) { isFixed = 1; }else{ isFixed = 0;}
	
    date[currentIndex] = time;

    double xyz[3];
    double trueradius = earth_radius() + h;
}




rpi_sensor_gps::~rpi_sensor_gps(){
    if (size() > 1) {
        delete [] date;
        delete [] latitude;
        delete [] longitude;
        delete [] height;
        delete [] accuracy;
		delete [] speed;
    }else{
        delete date;
        delete latitude;
        delete longitude;
        delete height;
        delete accuracy;
		delete speed;
    }
}
void rpi_sensor_gps::startLog(QString filename){}
void rpi_sensor_gps::logEntry(){}
void rpi_sensor_gps::stopLog(){}

double &rpi_sensor_gps::getLatitude() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return latitude[currentIndex]; }
	
double &rpi_sensor_gps::getSpeed() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return speed[currentIndex]; }	
double &rpi_sensor_gps::getLongitude() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return longitude[currentIndex]; }
double &rpi_sensor_gps::getHeight() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return height[currentIndex]; }
double &rpi_sensor_gps::getAccuracy() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return accuracy[currentIndex]; }

QDateTime rpi_sensor_gps::getTime() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return date[currentIndex]; }


QString rpi_sensor_gps::toXML(bool values,bool conf) {
    QDomDocument xmlDoc("GPS");
    QDomElement root = xmlDoc.createElement("GPS");
    xmlDoc.appendChild(root);
    addtoDomDoc(xmlDoc,root,values,conf);
    return xmlDoc.toString();

}

void rpi_sensor_gps::addtoDomDoc(QDomDocument &d,QDomElement &e,
                                 bool values,bool conf){
    rpi_device::addtoDomDoc(d,e,conf);

    if (values) {
    QDomElement xLatitude = d.createElement("Latitude");
    QDomText tLatitude = d.createTextNode(QString(fromDouble(getLatitude())));
    QDomElement xLongitude = d.createElement("Longitude");
    QDomText tLongitude = d.createTextNode(QString(fromDouble(getLongitude())));
    QDomElement xHeight = d.createElement("Height");
    QDomText tHeight = d.createTextNode(QString(fromDouble(getHeight())));
    QDomElement xDate = d.createElement("Date");
    QDomText tDate = d.createTextNode(getTime().toString(qdatetimestring));


    e.appendChild(xLatitude);
    e.appendChild(xLongitude);
    e.appendChild(xHeight);
    e.appendChild(xDate);

    xLatitude.appendChild(tLatitude);
    xLongitude.appendChild(tLongitude);
    xHeight.appendChild(tHeight);
    xDate.appendChild(tDate);
    }
    rpi_device::addtoDomDoc(d,e,conf);
}

rpi_sensor_acc::rpi_sensor_acc(const int mode) {
    setType(rpi_device::RPI_ACC);
    zeroTime = QDateTime::currentDateTime();
    time= new double[mode];
    acceleration= new double*[mode];
    velocity= new double*[mode];
    position=new double*[mode];
    cMode = mode;
    reset();
    if (cMode == SENSOR_MODE_CLIENT) currentIndex =0;
}

void rpi_sensor_acc::reset() {
    zeroTime = QDateTime::currentDateTime();
    for (int c=0;c<cMode;c++) {
        time[c] = 0;
        acceleration[c] = new double [3];
        velocity[c] = new double [3];
        position[c] = new double [3];
        for (int k=0;k<3;k++) {
            acceleration[c][k] =0;
            velocity[c][k] =0;
            position[c][k] =0;
        }
    }
    currentIndex =-1;
}
void rpi_sensor_acc::setAcceleration(double x, double y,double z,QDateTime t){
    //currentIndex = (++currentIndex % cMode);
    acceleration[currentIndex][0] =x;
    acceleration[currentIndex][1] =y;
    acceleration[currentIndex][2] =z;
    time[currentIndex] = double(zeroTime.msecsTo(t)) / 1000;



    if (cMode == SENSOR_MODE_SERVER) {

        int lastVel = currentIndex-1;
        if (lastVel < 0) lastVel+=cMode;
        if (time[currentIndex] <= time[lastVel] ) return;
        for (int c=0;c<3;c++) {
            velocity[currentIndex][c]=velocity[lastVel][c] + acceleration[currentIndex][c]  * (time[currentIndex] - time[lastVel]);
            position[currentIndex][c] = position[lastVel][c] + velocity[currentIndex][c] * (time[currentIndex] - time[lastVel]);
        }
    }
}
void rpi_sensor_acc::setPosition(double x, double y,double z){
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    position[currentIndex][0] =x;
    position[currentIndex][1] =y;
    position[currentIndex][2] =z;
}
void rpi_sensor_acc::setVelocity(double x, double y,double z){
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    velocity[currentIndex][0] =x;
    velocity[currentIndex][1] =y;
    velocity[currentIndex][2] =z;
}
void rpi_sensor_acc::startLog(QString filename){}
void rpi_sensor_acc::logEntry(){}
void rpi_sensor_acc::stopLog(){}
bool rpi_sensor_acc::test(){

    setAcceleration(1,0,0,zeroTime.addSecs(1));
    setAcceleration(0,1,0,zeroTime.addSecs(2));
    setAcceleration(0,0,1,zeroTime.addSecs(3));

    QString xmlStr = toXML();

    rpi_sensor_acc client(rpi_sensor_acc::SENSOR_MODE_CLIENT);

    client.readXML(xmlStr);
    //return false;

    int diff =
    int(currentxPosition() != client.currentxPosition() )
    + int(currentyPosition() != client.currentyPosition())
    + int(currentzPosition() != client.currentzPosition())
    + int(currentxVelocity() != client.currentxVelocity())
    + int(currentyVelocity() != client.currentyVelocity())
    + int(currentzVelocity() != client.currentzVelocity())
    + int(currentxAcceleration() != client.currentxAcceleration())
    + int(currentyAcceleration() != client.currentyAcceleration())
    + int(currentzAcceleration() != client.currentzAcceleration());


    if (diff) {
        qDebug() << xmlStr;

        qDebug() << "diff" << diff;

    qDebug() << "x" << currentxPosition() << client.currentxPosition();
    qDebug() << "y" << currentyPosition() << client.currentyPosition();
    qDebug() << "z" << currentzPosition() << client.currentzPosition();
    //return false;

    qDebug() << "u" << currentxVelocity() << client.currentxVelocity();
    qDebug() << "v" << currentyVelocity() << client.currentyVelocity();
    qDebug() << "w" << currentzVelocity() << client.currentzVelocity();


    qDebug() << "a" << currentxAcceleration() << client.currentxAcceleration();
    qDebug() << "b" << currentyAcceleration() << client.currentyAcceleration();
    qDebug() << "c" << currentzAcceleration() << client.currentzAcceleration();

    qDebug() << "Time" << currentTime() << client.currentTime();
    }
    reset();
    return (!diff);
}

double rpi_sensor_acc::currentTime() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return time[currentIndex];
}

QString rpi_sensor_acc::toXML(bool values,bool conf) {
    QDomDocument xmlDoc("Accelerometer");
    QDomElement root = xmlDoc.createElement("Accelerometer");
    xmlDoc.appendChild(root);
    addtoDomDoc(xmlDoc,root,values,conf);
    return xmlDoc.toString();
}

void rpi_sensor_acc::addtoDomDoc(QDomDocument &d,QDomElement &e,
                                 bool values,bool conf){
    rpi_device::addtoDomDoc(d,e,conf);


    if (values) {
        QDomElement elPosition = d.createElement("Position");
        QDomElement elxPosition = d.createElement("x");
        QDomElement elyPosition = d.createElement("y");
        QDomElement elzPosition = d.createElement("z");
        QDomText txPosition = d.createTextNode(QString(fromDouble(currentxPosition())));
        QDomText tyPosition = d.createTextNode(QString(fromDouble(currentyPosition())));
        QDomText tzPosition = d.createTextNode(QString(fromDouble(currentzPosition())));


    e.appendChild(elPosition);
    elPosition.appendChild(elxPosition);
    elPosition.appendChild(elyPosition);
    elPosition.appendChild(elzPosition);
    elxPosition.appendChild(txPosition);
    elyPosition.appendChild(tyPosition);
    elzPosition.appendChild(tzPosition);

    QDomElement elVelocity = d.createElement("Velocity");
    QDomElement elxVelocity = d.createElement("x");
    QDomElement elyVelocity = d.createElement("y");
    QDomElement elzVelocity = d.createElement("z");
    QDomText txVelocity = d.createTextNode(QString(fromDouble(currentxVelocity())));
    QDomText tyVelocity = d.createTextNode(QString(fromDouble(currentyVelocity())));
    QDomText tzVelocity = d.createTextNode(QString(fromDouble(currentzVelocity())));

    e.appendChild(elVelocity);
    elVelocity.appendChild(elxVelocity);
    elVelocity.appendChild(elyVelocity);
    elVelocity.appendChild(elzVelocity);
    elxVelocity.appendChild(txVelocity);
    elyVelocity.appendChild(tyVelocity);
    elzVelocity.appendChild(tzVelocity);


    //QDomText tLongitude = xmlDoc.createTextNode(QString(fromDouble(getLongitude())));
    QDomElement elAcceleration = d.createElement("Acceleration");
    QDomElement elxAcceleration = d.createElement("x");
    QDomElement elyAcceleration = d.createElement("y");
    QDomElement elzAcceleration = d.createElement("z");
    QDomText txAcceleration = d.createTextNode(QString(fromDouble(currentxAcceleration())));
    QDomText tyAcceleration = d.createTextNode(QString(fromDouble(currentyAcceleration())));
    QDomText tzAcceleration = d.createTextNode(QString(fromDouble(currentzAcceleration())));
    e.appendChild(elAcceleration);
    elAcceleration.appendChild(elxAcceleration);
    elAcceleration.appendChild(elyAcceleration);
    elAcceleration.appendChild(elzAcceleration);
    elxAcceleration.appendChild(txAcceleration);
    elyAcceleration.appendChild(tyAcceleration);
    elzAcceleration.appendChild(tzAcceleration);

    QDomElement elTime = d.createElement("Time");
    QDomText tTime = d.createTextNode(QString(fromDouble(currentTime())));

    }
    rpi_device::addtoDomDoc(d,e,conf);
}


void rpi_sensor_acc::readXML(QString i) {
    QDomDocument xmlDoc("Accelerometer");
    xmlDoc.setContent(i);

    QDomElement docElem = xmlDoc.documentElement();
    readDomElement(docElem);
}
void rpi_sensor_acc::readDomElement(QDomElement &elem){

    double *tempAcceleration =0;
    double *tempVelocity=0;
    double *tempPosition=0;
    double tempTime;
    QDomNode n = elem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "Acceleration") {
                tempAcceleration = xyzNode(n.firstChild());
            }
            if (e.tagName() == "Velocity") {
                tempVelocity = xyzNode(n.firstChild());
            }
            if (e.tagName() == "Position") {
                tempPosition = xyzNode(n.firstChild());
            }
            if (e.tagName() == "Time") {
                tempTime = fromHex(e.text().toLocal8Bit());
            }
        }
        n = n.nextSibling();
    }
    qint64 mmsec = 100* tempTime;
    if (tempAcceleration) {
        setAcceleration(tempAcceleration[0],tempAcceleration[1],tempAcceleration[2],zeroTime.addMSecs(mmsec));
        delete [] tempAcceleration;
    }
    if (tempVelocity) {
        setVelocity(tempVelocity[0],tempVelocity[1],tempVelocity[2]);
        delete [] tempVelocity;
    }
    if (tempPosition) {
        setPosition(tempPosition[0],tempPosition[1],tempPosition[2]);
        delete [] tempPosition;
    }

    rpi_device::readDomElement(elem);
}



double rpi_sensor_acc::currentxPosition() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return position[currentIndex][0] ;}
double rpi_sensor_acc::currentyPosition() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return position[currentIndex][1];}
double rpi_sensor_acc::currentzPosition() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return position[currentIndex][2];}

double rpi_sensor_acc::currentxVelocity() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return velocity[currentIndex][0]; }
double rpi_sensor_acc::currentyVelocity() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return velocity[currentIndex][1]; }
double rpi_sensor_acc::currentzVelocity() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return velocity[currentIndex][2]; }

double rpi_sensor_acc::currentxAcceleration() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return acceleration[currentIndex][0]; }
double rpi_sensor_acc::currentyAcceleration() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return acceleration[currentIndex][1]; }
double rpi_sensor_acc::currentzAcceleration() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return acceleration[currentIndex][2]; }


rpi_sensor_ultrasonic::rpi_sensor_ultrasonic(const int mode,QDateTime z){
    zeroTime =z;
    cMode = mode;
    distance = new double[mode];
    time = new double [mode];
    currentIndex =-1;
    for (int c=0;c<cMode;c++) {
        distance[c] =0;
        time[c] =0;
    }
    setType(rpi_device::RPI_ULTRASONICSENSOR);

}
void rpi_sensor_ultrasonic::setDistance(double d,QDateTime t)
{
    currentIndex = (++currentIndex)%cMode;
    distance[currentIndex] = d;
    time[currentIndex] = double(zeroTime.msecsTo(t))/1000;
}
double rpi_sensor_ultrasonic::getDistance(){ return distance[currentIndex];}
rpi_sensor_ultrasonic::~rpi_sensor_ultrasonic(){
    delete [] time;
    delete [] distance;
}


rpi_sensor_gyro::rpi_sensor_gyro(const int mode) {
    zeroTime = QDateTime::currentDateTime();
    time= new double[mode];
    acceleration= new double*[mode];
    velocity= new double*[mode];
    angle=new double*[mode];
    quaternion =new double*[mode];
    cMode = mode;
    reset();
    setType(rpi_device::RPI_GYRO);
    if (cMode == SENSOR_MODE_CLIENT) currentIndex =0;
}

void rpi_sensor_gyro::reset() {
    zeroTime = QDateTime::currentDateTime();
    for (int c=0;c<cMode;c++) {
        time[c] = 0;
        acceleration[c] = new double [3];
        velocity[c] = new double [3];
        angle[c] = new double [3];
        quaternion[c] =new double[4];
        for (int k=0;k<3;k++) {
            acceleration[c][k] =0;
            velocity[c][k] =0;
            angle[c][k] =0;
            quaternion[c][k] =0;
        }
        quaternion[c][3] =0;
    }
    currentIndex =-1;
    setType(rpi_device::RPI_GYRO);
}
void rpi_sensor_gyro::setQuaternion(double *q) {
  currentIndex = (++currentIndex % cMode);
  for (int i=0;i<4;i++) quaternion[currentIndex][i] = q[i];
}

void rpi_sensor_gyro::setAcceleration(double t, double p,double s,QDateTime d){
    currentIndex = (++currentIndex % cMode);
    acceleration[currentIndex][0] =t;
    acceleration[currentIndex][1] =p;
    acceleration[currentIndex][2] =s;
    time[currentIndex] = double(zeroTime.msecsTo(d)) / 1000;



    if (cMode == SENSOR_MODE_SERVER) {

        int lastVel = currentIndex-1;
        if (lastVel < 0) lastVel+=cMode;
        if (time[currentIndex] <= time[lastVel] ) return;
        for (int c=0;c<3;c++) {
            velocity[currentIndex][c]=velocity[lastVel][c] + acceleration[currentIndex][c]  * (time[currentIndex] - time[lastVel]);
            angle[currentIndex][c] = angle[lastVel][c] + velocity[currentIndex][c] * (time[currentIndex] - time[lastVel]);
        }
    }
}
void rpi_sensor_gyro::setAngle(double t, double p,double s){
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    angle[currentIndex][0] =t;
    angle[currentIndex][1] =p;
    angle[currentIndex][2] =s;
}
void rpi_sensor_gyro::setVelocity(double t, double p,double s){
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    velocity[currentIndex][0] =t;
    velocity[currentIndex][1] =p;
    velocity[currentIndex][2] =s;
}
void rpi_sensor_gyro::startLog(QString filename){}
void rpi_sensor_gyro::logEntry(){}
void rpi_sensor_gyro::stopLog(){}
bool rpi_sensor_gyro::test(){

    setAcceleration(1,0,0,zeroTime.addSecs(1));
    setAcceleration(0,1,0,zeroTime.addSecs(2));
    setAcceleration(0,0,1,zeroTime.addSecs(3));

    QString xmlStr = toXML();

    rpi_sensor_gyro client(rpi_sensor_acc::SENSOR_MODE_CLIENT);

    client.readXML(xmlStr);
    //return false;

    int diff =
    int(currentThetaAngle() != client.currentThetaAngle() )
    + int(currentPhiAngle() != client.currentPhiAngle())
    + int(currentSigmaAngle() != client.currentSigmaAngle())
    + int(currentThetaVelocity() != client.currentThetaVelocity())
    + int(currentPhiVelocity() != client.currentPhiVelocity())
    + int(currentSigmaVelocity() != client.currentSigmaVelocity())
    + int(currentThetaAcceleration() != client.currentThetaAcceleration())
    + int(currentPhiAcceleration() != client.currentPhiAcceleration())
    + int(currentSigmaAcceleration() != client.currentSigmaAcceleration());


    if (diff) {
        qDebug() << xmlStr;

        qDebug() << "diff" << diff;

        qDebug() << "theta" << currentThetaAngle() << client.currentThetaAngle();
        qDebug() << "phi" << currentPhiAngle() << client.currentPhiAngle();
        qDebug() << "sigma" << currentSigmaAngle() << client.currentSigmaAngle();
    //return false;

        qDebug() << ".theta" << currentThetaVelocity() << client.currentThetaVelocity();
        qDebug() << ".phi" << currentPhiVelocity() << client.currentPhiVelocity();
        qDebug() << ".sigma" << currentSigmaVelocity() << client.currentSigmaVelocity();


        qDebug() << "..theta" << currentThetaAcceleration() << client.currentThetaAcceleration();
        qDebug() << "..phi" << currentPhiAcceleration() << client.currentPhiAcceleration();
        qDebug() << "..sigma" << currentSigmaAcceleration() << client.currentSigmaAcceleration();

        qDebug() << "Time" << currentTime() << client.currentTime();
    }
    reset();

    return (!diff);
}

double rpi_sensor_gyro::currentTime() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return time[currentIndex];
}
QString rpi_sensor_gyro::toXML(bool values,bool conf) {
    QDomDocument xmlDoc("Gyroscope");
    QDomElement root = xmlDoc.createElement("Gyroscope");
    xmlDoc.appendChild(root);
    addtoDomDoc(xmlDoc,root,values,conf);
    return xmlDoc.toString();

}
void rpi_sensor_gyro::addtoDomDoc(QDomDocument &d, QDomElement &e, bool values, bool conf){
    rpi_device::addtoDomDoc(d,e,conf);
    if (values) {
    QDomElement elPosition = d.createElement("Angle");
    QDomElement eltPosition = d.createElement("theta");
    QDomElement elpPosition = d.createElement("phi");
    QDomElement elsPosition = d.createElement("sigma");
    QDomText ttPosition = d.createTextNode(QString(fromDouble(currentThetaAngle())));
    QDomText tpPosition = d.createTextNode(QString(fromDouble(currentPhiAngle())));
    QDomText tsPosition = d.createTextNode(QString(fromDouble(currentSigmaAngle())));

    e.appendChild(elPosition);
    elPosition.appendChild(eltPosition);
    elPosition.appendChild(elpPosition);
    elPosition.appendChild(elsPosition);
    eltPosition.appendChild(ttPosition);
    elpPosition.appendChild(tpPosition);
    elsPosition.appendChild(tsPosition);

    QDomElement elVelocity = d.createElement("Velocity");
    QDomElement eltVelocity = d.createElement("theta");
    QDomElement elpVelocity = d.createElement("phi");
    QDomElement elsVelocity = d.createElement("sigma");
    QDomText ttVelocity = d.createTextNode(QString(fromDouble(currentThetaVelocity())));
    QDomText tpVelocity = d.createTextNode(QString(fromDouble(currentPhiVelocity())));
    QDomText tsVelocity = d.createTextNode(QString(fromDouble(currentSigmaVelocity())));

    e.appendChild(elVelocity);
    elVelocity.appendChild(eltVelocity);
    elVelocity.appendChild(elpVelocity);
    elVelocity.appendChild(elsVelocity);
    eltVelocity.appendChild(ttVelocity);
    elpVelocity.appendChild(tpVelocity);
    elsVelocity.appendChild(tsVelocity);

    QDomElement elAcceleration = d.createElement("Acceleration");
    QDomElement eltAcceleration = d.createElement("theta");
    QDomElement elpAcceleration = d.createElement("phi");
    QDomElement elsAcceleration = d.createElement("sigma");
    QDomText ttAcceleration = d.createTextNode(QString(fromDouble(currentThetaAcceleration())));
    QDomText tpAcceleration = d.createTextNode(QString(fromDouble(currentPhiAcceleration())));
    QDomText tsAcceleration = d.createTextNode(QString(fromDouble(currentSigmaAcceleration())));
    e.appendChild(elAcceleration);
    elAcceleration.appendChild(eltAcceleration);
    elAcceleration.appendChild(elpAcceleration);
    elAcceleration.appendChild(elsAcceleration);
    eltAcceleration.appendChild(ttAcceleration);
    elpAcceleration.appendChild(tpAcceleration);
    elsAcceleration.appendChild(tsAcceleration);


   QDomElement elQuaternion = d.createElement("Quaternion");
    QDomElement elQuaternion0 = d.createElement("q0");
    QDomElement elQuaternion1 = d.createElement("q1");
    QDomElement elQuaternion2 = d.createElement("q2");
    QDomElement elQuaternion3 = d.createElement("q3");

    QDomText ttQuaternion0 = d.createTextNode(QString(fromDouble(currentQuaternion(0))));
    QDomText ttQuaternion1 = d.createTextNode(QString(fromDouble(currentQuaternion(1))));
    QDomText ttQuaternion2 = d.createTextNode(QString(fromDouble(currentQuaternion(2))));
    QDomText ttQuaternion3 = d.createTextNode(QString(fromDouble(currentQuaternion(3))));
    
    e.appendChild(elQuaternion);
    elQuaternion0.appendChild(elQuaternion0);
    elQuaternion1.appendChild(elQuaternion1);
    elQuaternion2.appendChild(elQuaternion2);
    elQuaternion3.appendChild(elQuaternion3);
    
    elQuaternion0.appendChild(ttQuaternion0);
    elQuaternion1.appendChild(ttQuaternion1);
    elQuaternion2.appendChild(ttQuaternion2);
    elQuaternion3.appendChild(ttQuaternion3);

    QDomElement elTime = d.createElement("Time");
    QDomText tTime = d.createTextNode(QString(fromDouble(currentTime())));
    }
}


void rpi_sensor_gyro::readXML(QString i) {
    QDomDocument xmlDoc("Accelerometer");
    xmlDoc.setContent(i);
    QDomElement docElem = xmlDoc.documentElement();
    readDomElement(docElem);
}
void rpi_sensor_gyro::readDomElement(QDomElement &elem){
    /*double tlatitude =0;
    double tlongitude =0;
    double theight =0;
    QDateTime ttime;*/
    double *tempAcceleration =0;
    double *tempVelocity =0;
    double *tempAngle =0;
    double *tempQuaternion =0;
    
    double tempTime;
    QDomNode n = elem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "Acceleration") {
                tempAcceleration = tpsNode(n.firstChild());
            }
            if (e.tagName() == "Velocity") {
                tempVelocity = tpsNode(n.firstChild());
            }
            if (e.tagName() == "Angle") {
                tempAngle = tpsNode(n.firstChild());
            }
            if (e.tagName() == "Quaternion") {
                tempQuaternion = quaternionNode(n.firstChild());
            }
            if (e.tagName() == "Time") {
                tempTime = fromHex(e.text().toLocal8Bit());
            }
        }
        n = n.nextSibling();
    }
    qint64 mmsec = 100* tempTime;
    if (tempAcceleration) {
        setAcceleration(tempAcceleration[0],tempAcceleration[1],tempAcceleration[2],zeroTime.addMSecs(mmsec));
        delete [] tempAcceleration;
    }
    if (tempVelocity) {
        setVelocity(tempVelocity[0],tempVelocity[1],tempVelocity[2]);
        delete [] tempVelocity;
    }
    if (tempAngle) {
        setAngle(tempAngle[0],tempAngle[1],tempAngle[2]);
        delete [] tempAngle;
    }
    if (tempQuaternion) {
        setQuaternion(tempQuaternion);
        delete [] tempQuaternion;
    }
    
    rpi_device::readDomElement(elem);
}

double rpi_sensor_gyro::currentThetaAngle() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return angle[currentIndex][0] ;}
double rpi_sensor_gyro::currentPhiAngle() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return angle[currentIndex][1];}
double rpi_sensor_gyro::currentSigmaAngle() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return angle[currentIndex][2];}

double rpi_sensor_gyro::currentQuaternion(const int i) {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return quaternion[currentIndex][i]; }


double rpi_sensor_gyro::currentThetaVelocity() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return velocity[currentIndex][0]; }
double rpi_sensor_gyro::currentPhiVelocity() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return velocity[currentIndex][1]; }
double rpi_sensor_gyro::currentSigmaVelocity() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return velocity[currentIndex][2]; }

double rpi_sensor_gyro::currentThetaAcceleration() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return acceleration[currentIndex][0]; }
double rpi_sensor_gyro::currentPhiAcceleration() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return acceleration[currentIndex][1]; }
double rpi_sensor_gyro::currentSigmaAcceleration() {
    if ( (currentIndex<0) || (currentIndex>=cMode) ) currentIndex =0;
    return acceleration[currentIndex][2]; }

QString rpi_sensor_ultrasonic::toXML(bool values,bool conf) {
    QDomDocument xmlDoc("Ultrasonic");
    QDomElement root = xmlDoc.createElement("Ultrasonic");
    addtoDomDoc(xmlDoc,root,values,conf);
    xmlDoc.appendChild(root);
    return xmlDoc.toString();
}

void rpi_sensor_ultrasonic::readDomElement(QDomElement &elem) {
    QDomNode n = elem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            if (e.tagName() == "Distance") {
                //value_v = e.text().toInt();
               setDistance(e.text().toDouble(),QDateTime::currentDateTime());
            }
        }
        n = n.nextSibling();
    }
    rpi_device::readDomElement(elem);
}

void rpi_sensor_ultrasonic::addtoDomDoc(QDomDocument &d,QDomElement &e,bool values,bool conf){
    if (values) {
        int cI = currentIndex;
        double cDistance = distance[cI];
        double cTime = time[cI];
        QDomElement edistance = d.createElement("Distance");
        QDomElement etime = d.createElement("Time");
        e.appendChild(edistance);
        e.appendChild(etime);
        QDomText tDistance = d.createTextNode(QString(fromDouble(cDistance)));
        QDomText tTime = d.createTextNode(QString(fromDouble(cTime)));
        edistance.appendChild(tDistance);
        etime.appendChild(tTime);
    }
    rpi_device::addtoDomDoc(d,e,conf);
}

double *quaternionNode(QDomNode n) {
    double *q = new double [4];
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "q0") {
                q[0] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "q1") {
                q[1] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "q2") {
                q[2] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "q3") {
                q[2] = fromHex(e.text().toLocal8Bit());
            } 
            }
        n = n.nextSibling();
    }
    return q;
}




//-----------------------------  9DOF IMU ----------------------------------------
rpi_imu_9dof::rpi_imu_9dof(const int mode,QDateTime z):rpi_device(){

    //zeroTime = QDateTime::currentDateTime();
    time= new double[mode];
    quaternion =new float*[mode];
    euler =new float*[mode];
    accel = new int*[mode];
    gyro = new int*[mode];
    magn = new int*[mode];

    for (int k=0;k<mode;k++) {
        quaternion[k] = new float[NUM_FOUR];
        euler[k] = new float[NUM_THREE];
        accel[k] = new int[NUM_THREE];
        gyro[k] = new int[NUM_THREE];
        magn[k] = new int[NUM_THREE];
    }

    cMode = mode;
    setType(rpi_device::RPI_IMU9DOF);    //if (cMode == SENSOR_MODE_CLIENT) currentIndex =0;
    currentIndex =0;
    hexMode =0;

}

rpi_imu_9dof::~rpi_imu_9dof() {
    for (int k=0;k<cMode;k++) {
        delete [] quaternion[k];
        delete [] euler[k];
        delete [] accel[k];
        delete [] gyro[k];
        delete [] magn[k];
    }
    delete [] quaternion;
    delete [] euler;
    delete [] accel;
    delete [] gyro;
    delete [] magn;
}

void rpi_imu_9dof::nextIndex() {
    currentIndex = (++currentIndex % cMode);
}
void rpi_imu_9dof::setQuaternion(const float q1,const float q2,const float q3,const float q4){
    quaternion[currentIndex][0] = q1;
    quaternion[currentIndex][1] = q2;
    quaternion[currentIndex][2] = q3;
    quaternion[currentIndex][3] = q4;
}
void rpi_imu_9dof::setQuaternion(const float *q){
    for (int j=0;j<NUM_FOUR;j++) quaternion[currentIndex][j] = q[j];
}
float rpi_imu_9dof::getQuaternion(int k,int i){
    if (i == -1) i = currentIndex;
    return quaternion[i%cMode][k];
}
void rpi_imu_9dof::addtoDomDoc(QDomDocument &d,QDomElement &e,bool values,bool conf){

    rpi_device::addtoDomDoc(d,e,conf);
    if (values) {

        QDomElement eAccel = d.createElement("Acceleration");
        QDomElement eMagn = d.createElement("Magnetometer");
        QDomElement eGyro = d.createElement("Gyroscope");
        QDomElement eQuant = d.createElement("Quaternion");
        QDomElement eEuler = d.createElement("Euler");

        e.appendChild(eAccel);
        e.appendChild(eMagn);
        e.appendChild(eGyro);
        e.appendChild(eQuant);
        e.appendChild(eEuler);

        addQuantElementsFromFloat(d,eQuant,quaternion[currentIndex]);

        addXYZElementFromInt(d,eMagn,magn[currentIndex]);
        addXYZElementFromInt(d,eGyro,gyro[currentIndex]);
        addXYZElementFromInt(d,eAccel,accel[currentIndex]);
        addTPSElementsFromFloat(d,eEuler,euler[currentIndex]);

        /*if (e.tagName() == "Acceleration") {
            intXYZNode(n.firstChild(),accel[currentIndex]);
            //tempAcceleration = tpsNode(n.firstChild());
        }
        if (e.tagName() == "Gyroscope") {
            intXYZNode(n.firstChild(),gyro[currentIndex]);
        }
        if (e.tagName() == "Quaternion") {
            floatQuantNode(n.firstChild(),quaternion[currentIndex]);
        }
        if (e.tagName() == "Magnetometer") {
            intXYZNode(n.firstChild(),magn[currentIndex]);
        }*/

    }





}
QString rpi_imu_9dof::toXML(bool values,bool conf){
    QDomDocument xmlDoc("imu9dof");
    QDomElement root = xmlDoc.createElement("imu9dof");
    addtoDomDoc(xmlDoc,root,values,conf);
    xmlDoc.appendChild(root);
    return xmlDoc.toString();
}
void rpi_imu_9dof::readXML(QString i){
    QDomDocument xmlDoc("imu9dof");
    xmlDoc.setContent(i);
    QDomElement docElem = xmlDoc.documentElement();
    readDomElement(docElem);
}
void rpi_imu_9dof::setMagn(const int x,const int y,const int z){
    magn[currentIndex][NUM_ZERO] =x;
    magn[currentIndex][NUM_ONE] =y;
    magn[currentIndex][NUM_TWO] =z;
}
void rpi_imu_9dof::setGyro(const int x,const int y,const int z){
    gyro[currentIndex][NUM_ZERO] =x;
    gyro[currentIndex][NUM_ONE] =y;
    gyro[currentIndex][NUM_TWO] =z;
}
void rpi_imu_9dof::setAccel(const int x,const int y,const int z){
    accel[currentIndex][NUM_ZERO] = x;
    accel[currentIndex][NUM_ONE] = y;
    accel[currentIndex][NUM_TWO] = z;
}
float* rpi_imu_9dof::getCurrentQuaternionPointer(){
    return quaternion[currentIndex];

}
int* rpi_imu_9dof::getCurrentMagnPointer(){
    return magn[currentIndex];
}
int* rpi_imu_9dof::getCurrentAccelPointer(){
    return accel[currentIndex];
}
int* rpi_imu_9dof::getCurrentGyroPointer(){
    return gyro[currentIndex];
}


void rpi_imu_9dof::setQuatFAccelIGyroIMagnIByHexMessage(QByteArray &qData){
    //qDebug() << "rpi_imu_9dof dataIn" <<qData ;

    int nextIndex = (currentIndex+1)%cMode;
    //<< currentIndex;
    for (int k=0;k<NUM_FOUR;k++) {
        QByteArray tmpq = reverseHexArray(qData.mid(k*9,8));
        QByteArray tmpp = QByteArray::fromHex(tmpq.toLower());



        unsigned long  intbits;

        // Debug Block
        //unsigned long  intbitsA;
        //unsigned long  intbitsB;
        //intbitsA = (tmpp[3] << 24) | ((tmpp[2] & 0xff) << 16) | ((tmpp[1] & 0xff) << 8) | (tmpp[0] & 0xff);
        //intbitsB = (tmpp[0] << 24) | ((tmpp[1] & 0xff) << 16) | ((tmpp[2] & 0xff) << 8) | (tmpp[3] & 0xff);
        //float *tmpfA = (float*)&intbitsA;
        //float *tmpfB = (float*)&intbitsB;
        //qDebug() << k << tmpq.toLower() << *tmpfA << *tmpfB;
        // End of Debug Block

        switch (hexMode) {
            default:
            case 1 : {
                intbits = (tmpp[3] << 24) | ((tmpp[2] & 0xff) << 16) | ((tmpp[1] & 0xff) << 8) | (tmpp[0] & 0xff);
                break;
                }
            case 2:

            {
                intbits = (tmpp[0] << 24) | ((tmpp[1] & 0xff) << 16) | ((tmpp[2] & 0xff) << 8) | (tmpp[3] & 0xff);
                break;
            }
        }
\

        float *tmpf = (float*)&intbits;
        //qDebug() << this->currentIndex;
        //qDebug() <<quaternion[0][k];

        quaternion[nextIndex][k] = *tmpf;//QByteArray::fromHex(tmpq).toFloat();
        //qDebug() << k << *tmpf ;
    }
    //return;
    qDebug() << nextIndex << quaternion[nextIndex][0] << quaternion[nextIndex][1] << quaternion[nextIndex][2] << quaternion[nextIndex][3];
    quaternionToEuler(quaternion[nextIndex],euler[nextIndex]);
    for (int k=0;k<NUM_THREE;k++) {
        QByteArray tmpq = reverseHexArray(qData.mid(36+k*5,4));
        QByteArray tmpp = QByteArray::fromHex(tmpq.toLower());
        accel[nextIndex][k] = ((tmpp[0] & 0xff) << 8) | (tmpp[1] & 0xff);
    }


    for (int k=0;k<NUM_THREE;k++) {
        QByteArray tmpq = reverseHexArray(qData.mid(51+k*5,4));
        QByteArray tmpp = QByteArray::fromHex(tmpq.toLower());
        gyro[nextIndex][k] = ((tmpp[0] & 0xff) << 8) | (tmpp[1] & 0xff);
    }


    for (int k=0;k<NUM_THREE;k++) {
        QByteArray tmpq = reverseHexArray(qData.mid(66+k*5,4));
        QByteArray tmpp = QByteArray::fromHex(tmpq.toLower());
        magn[nextIndex][k] = ((tmpp[0] & 0xff) << 8) | (tmpp[1] & 0xff);
    }
    currentIndex = nextIndex;
}



void rpi_imu_9dof::setQuatFByHexMessage(QByteArray &qData){
    //qDebug() << "rpi_imu_9dof dataIn" <<qData ;

    int nextIndex = (currentIndex+1)%cMode;
    //<< currentIndex;
    for (int k=0;k<NUM_FOUR;k++) {
        QByteArray tmpq = reverseHexArray(qData.mid(k*9,8));
        QByteArray tmpp = QByteArray::fromHex(tmpq.toLower());



        unsigned long  intbits;

        // Debug Block
        //unsigned long  intbitsA;
        //unsigned long  intbitsB;
        //intbitsA = (tmpp[3] << 24) | ((tmpp[2] & 0xff) << 16) | ((tmpp[1] & 0xff) << 8) | (tmpp[0] & 0xff);
        //intbitsB = (tmpp[0] << 24) | ((tmpp[1] & 0xff) << 16) | ((tmpp[2] & 0xff) << 8) | (tmpp[3] & 0xff);
        //float *tmpfA = (float*)&intbitsA;
        //float *tmpfB = (float*)&intbitsB;
        //qDebug() << k << tmpq.toLower() << *tmpfA << *tmpfB;
        // End of Debug Block

        switch (hexMode) {
            default:
            case 1 : {
                intbits = (tmpp[3] << 24) | ((tmpp[2] & 0xff) << 16) | ((tmpp[1] & 0xff) << 8) | (tmpp[0] & 0xff);
                break;
                }
            case 2:

            {
                intbits = (tmpp[0] << 24) | ((tmpp[1] & 0xff) << 16) | ((tmpp[2] & 0xff) << 8) | (tmpp[3] & 0xff);
                break;
            }
        }
\

        float *tmpf = (float*)&intbits;
        //qDebug() << this->currentIndex;
        //qDebug() <<quaternion[0][k];

        quaternion[nextIndex][k] = *tmpf;//QByteArray::fromHex(tmpq).toFloat();
        //qDebug() << k << *tmpf ;
    }
    //return;
    //qDebug() << nextIndex << quaternion[nextIndex][0] << quaternion[nextIndex][1] << quaternion[nextIndex][2] << quaternion[nextIndex][3];
    quaternionToEuler(quaternion[nextIndex],euler[nextIndex]);
    currentIndex = nextIndex;
}


float rpi_imu_9dof::getEuler(int k,int i){
    if (i=-1) i= currentIndex;
    return euler[i][k];
}

void rpi_imu_9dof::quat2euler(){
    quaternionToEuler(quaternion[currentIndex],euler[currentIndex]);
}

void rpi_imu_9dof::readDomElement(QDomElement &elem) {
//int *tempAcceleration =0;
//int *tempVelocity =0;
//int *tempAngle =0;
//int *tempQuaternion =0;

double tempTime;
QDomNode n = elem.firstChild();
while(!n.isNull()) {
    QDomElement e = n.toElement(); // try to convert the node to an element.
    if(!e.isNull()) {
        //qDebug() << e.tagName();
        if (e.tagName() == "Acceleration") {
            //qDebug() << e.tagName();
            intXYZNode(n.firstChild(),accel[currentIndex]);
            //tempAcceleration = tpsNode(n.firstChild());
        }
        if (e.tagName() == "Gyroscope") {
            //qDebug() << e.tagName();
            intXYZNode(n.firstChild(),gyro[currentIndex]);
        }
        if (e.tagName() == "Quaternion") {
            //qDebug() << e.tagName();
            floatQuantNode(n.firstChild(),quaternion[currentIndex]);
        }
        if (e.tagName() == "Magnetometer") {
            //qDebug() << e.tagName();
            intXYZNode(n.firstChild(),magn[currentIndex]);
        }
    }
    n = n.nextSibling();
}
quat2euler();
rpi_device::readDomElement(elem);
}

QStringList rpi_imu_9dof::functionTest() {
    QStringList ret = rpi_device::functionTest();

    QDateTime now =QDateTime::currentDateTime();

    rpi_imu_9dof testDev1(SENSOR_MODE_SERVER,now);


    int testAccelArray[] = {4,5,6};
    int testGyroArray[] = {7,8,9};
    int testMagn[] = {78,89,10};
    float testQuant[] = {0.5,0.7,0.3,0.84};



    testDev1.setAccel(testAccelArray[rpi_device::NUM_ZERO],testAccelArray[rpi_device::NUM_ONE],testAccelArray[rpi_device::NUM_TWO]);
    testDev1.setGyro(testGyroArray[rpi_device::NUM_ZERO],testGyroArray[rpi_device::NUM_ONE],testGyroArray[rpi_device::NUM_TWO]);
    testDev1.setMagn(testMagn[rpi_device::NUM_ZERO],testMagn[rpi_device::NUM_ONE],testMagn[rpi_device::NUM_TWO]);
    testDev1.setQuaternion(testQuant[rpi_device::NUM_ZERO],testQuant[rpi_device::NUM_ONE],testQuant[rpi_device::NUM_TWO],testQuant[NUM_THREE]);
    /*int *f = testDev1.getCurrentAccelPointer();
    qDebug() << f[0] << f[1] << f[2];

    f = testDev1.getCurrentGyroPointer();
    qDebug() << f[0] << f[1] << f[2];

    f = testDev1.getCurrentMagnPointer();
    qDebug() << f[0] << f[1] << f[2];*/

    //float *q = testDev1.getCurrentQuaternionPointer();
    //qDebug() << q[0] << q[1] << q[2] <<q[3];

    if (!(isArrayEqual(testAccelArray,testDev1.getCurrentAccelPointer(),NUM_THREE))) ret << "indivual Acceleration Not Defined";
    if (!(isArrayEqual(testGyroArray,testDev1.getCurrentGyroPointer(),NUM_THREE))) ret << "indivual Gyro Not Defined";
    if (!(isArrayEqual(testMagn,testDev1.getCurrentMagnPointer(),NUM_THREE))) ret << "indivual Magnetometer Not Defined";
    if (!(isArrayEqual(testQuant,testDev1.getCurrentQuaternionPointer(),NUM_FOUR))) ret << "indivual Quaternion Not Defined";


    testDev1.setQuaternion(testQuant);
    /*
    if (isEqualArray(testAccelArray,testDev1.getCurrentAccelPointer(),NUM_THREE)) ret << "array Acceleration Not Defined";
    if (isEqualArray(testGyroArray,testDev1.getCurrentGyroPointer(),NUM_THREE)) ret << "array Gyro Not Defined";
    if (isEqualArray(testMagn,testDev1.getCurrentMagnPointer(),NUM_THREE)) ret << "array Magnetometer Not Defined";
    */
    if (!(isArrayEqual(testQuant,testDev1.getCurrentQuaternionPointer(),NUM_FOUR))) ret << "array Quaternion not defined";

    rpi_imu_9dof testDevEqual(testDev1);

    if (!(testDevEqual == testDev1)) {
        if (testDevEqual.errorString().size())
            ret << "IMU Equal Test";
        ret << testDevEqual.errorString();}



    QString xmlStr= testDev1.toXML();

    //qDebug() << xmlStr;

    rpi_imu_9dof testDevXml(SENSOR_MODE_CLIENT,now); ;
    testDevXml.readXML(xmlStr);



    //qDebug() << testDevXml.toXML();
    if (!(testDevXml == testDev1)) {
        if (testDevXml.errorString().size())
            ret << "IMU XML Test";
        ret << testDevXml.errorString();}


    if (ret.size() == 0) { ret << "Function Test : PASS";
    }else{
        ret << "Function Test : FAIL";
    }


    return ret;
}


bool rpi_imu_9dof::operator==(const rpi_imu_9dof &i) {
    errorstring = "";
    //rpi_device:: *this == i;

    if (!(isArrayEqual(accel[currentIndex],i.accel[i.currentIndex],NUM_THREE))) errorstring += "Acceleration Not Equal:";
    if (!(isArrayEqual(gyro[currentIndex],i.gyro[i.currentIndex],NUM_THREE))) errorstring +=  "Gyro Not Equal:";
    if (!(isArrayEqual(magn[currentIndex],i.magn[i.currentIndex],NUM_THREE))) errorstring += "Magnetometer Not Equal:";
    if (!(isArrayEqual(quaternion[currentIndex],i.quaternion[i.currentIndex],NUM_FOUR))) errorstring += "Quanternion Not Equal:";

    return (!errorstring.size());
}
rpi_imu_9dof::rpi_imu_9dof(const rpi_imu_9dof &srcI): rpi_device(srcI) {

    const int mode = srcI.cMode;
    time= new double[mode];
    copyArray(srcI.time,time,mode);
    quaternion =new float*[mode];
    euler =new float*[mode];
    accel = new int*[mode];
    gyro = new int*[mode];
    magn = new int*[mode];

    for (int k=0;k<mode;k++) {
        quaternion[k] = new float[NUM_FOUR];
        euler[k] = new float[NUM_THREE];
        accel[k] = new int[NUM_THREE];
        gyro[k] = new int[NUM_THREE];
        magn[k] = new int[NUM_THREE];
        copyArray(srcI.quaternion[k],quaternion[k],NUM_FOUR);
        copyArray(srcI.euler[k],euler[k],NUM_THREE);
        copyArray(srcI.accel[k],accel[k],NUM_THREE);
        copyArray(srcI.gyro[k],gyro[k],NUM_THREE);
        copyArray(srcI.magn[k],magn[k],NUM_THREE);


    }

    cMode = mode;
    setType(rpi_device::RPI_IMU9DOF);    //if (cMode == SENSOR_MODE_CLIENT) currentIndex =0;

    currentIndex = srcI.currentIndex;

    hexMode = srcI.hexMode;
}


void rpi_imu_9dof::setHexMode(QByteArray &qData){
    //qDebug() << "Number Check to Process"<< qData << qData.size();
    QList<QByteArray> sData ;
    sData = qData.split(',');
    for (int j=0;j<sData.size();j++) {
        qDebug() << sData[j];//.toString();
    }
    if (2<sData.size()) {
    int randInt = sData[0].right(sData[0].size()-2).toInt();
    float randFloat = float(randInt) / float (4096);
    qDebug() << "Number:" << randInt << ";" << randFloat << sData[2];
    QByteArray reversesData;

    for (int l=sData[2].size()-1;l>=0;l--) {
       reversesData= reversesData.append(sData[2][l]);
    }

    QByteArray tmpp = QByteArray::fromHex(sData[2].toLower());
    //QByteArray tmpp2 = QByteArray::fromHex(reversesData);

    unsigned long  intbits = (tmpp[0] << 24) | ((tmpp[1] & 0xff) << 16) | ((tmpp[2] & 0xff) << 8) | (tmpp[3] & 0xff);
    //unsigned long  intbits2 = (tmpp2[3] << 24) | ((tmpp2[2] & 0xff) << 16) | ((tmpp2[1] & 0xff) << 8) | (tmpp2[0] & 0xff);

    unsigned long  intbits2 = (tmpp[3] << 24) | ((tmpp[2] & 0xff) << 16) | ((tmpp[1] & 0xff) << 8) | (tmpp[0] & 0xff);

    float *tmpf = (float*)&intbits;
    float *tmpf2 = (float*)&intbits2;

    qDebug() << "Number:" << randInt << ";" << randFloat << sData[2] << *tmpf << *tmpf2 << tmpp.toFloat();
    char *char_randFloat =  (char*)(&randFloat);

    if (*tmpf == randFloat) hexMode =1;
    if (*tmpf2 == randFloat) hexMode =2;




    QByteArray qtencodefloat(char_randFloat,4);
    //qtencodefloat.fill(char_randFloat,4);
    qDebug() << qtencodefloat.toHex() << hexMode ;

}
}
