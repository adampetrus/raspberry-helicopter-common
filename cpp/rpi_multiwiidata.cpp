#include "rpi_multiwiidata.h"


void appendDoubleDocElement(QDomDocument &d,QDomElement &e,QString label,double v) {
    QDomElement xe = d.createElement(label);
    QDomText txt = d.createTextNode(QString(fromDouble(v)));
    e.appendChild(xe);
    xe.appendChild(txt);
}

void appendIntDocElement(QDomDocument &d,QDomElement &e,QString label,int v) {
    QDomElement xe = d.createElement(label);
    QDomText txt = d.createTextNode(QString("%1").arg(v));
    e.appendChild(xe);
    xe.appendChild(txt);
}

uint8_t mspGPSWayPoint::getGpsFix() const
{
    return gpsFix;
}

void mspGPSWayPoint::setGpsFix(const uint8_t &value)
{
    gpsFix = value;
}
uint8_t mspGPSWayPoint::getGpsNumSat() const
{
    return gpsNumSat;
}

void mspGPSWayPoint::setGpsNumSat(const uint8_t &value)
{
    gpsNumSat = value;
}
int32_t mspGPSWayPoint::getGpsLat() const
{
    return gpsLat;
}

void mspGPSWayPoint::setGpsLat(const int32_t &value)
{
    gpsLat = value;
}
int32_t mspGPSWayPoint::getGpsLong() const
{
    return gpsLong;
}

void mspGPSWayPoint::setGpsLong(const int32_t &value)
{
    gpsLong = value;
}
uint16_t mspGPSWayPoint::getGpsAlt() const
{
    return gpsAlt;
}

void mspGPSWayPoint::setGpsAlt(const uint16_t &value)
{
    gpsAlt = value;
}
uint16_t mspGPSWayPoint::getGpsSpeed() const
{
    return gpsSpeed;
}

void mspGPSWayPoint::setGpsSpeed(const uint16_t &value)
{
    gpsSpeed = value;
}
uint16_t mspGPSWayPoint::getGpsCOG() const
{
    return gpsCOG;
}

void mspGPSWayPoint::setGpsCOG(const uint16_t &value)
{
    gpsCOG = value;
}

double mspGPSWayPoint::getGpsLatDouble() const
{
    return double(gpsLat)/1e7;
}

void mspGPSWayPoint::setGpsLatDouble(const double &value)
{
    gpsLat = value *1e7;

}

double mspGPSWayPoint::getGpsLongDouble() const
{
    return double(gpsLong)/1e7;
}

void mspGPSWayPoint::setGpsLongDouble(const double &value)
{
    gpsLong = value *1e7;
}

double mspGPSWayPoint::getGpsAltDouble() const
{
    return double(gpsAlt);
}

void mspGPSWayPoint::setGpsAltDouble(const double &value)
{
    gpsAlt = value;
}

double mspGPSWayPoint::getGpsSpeedDouble() const
{
    return double(gpsSpeed);
}

void mspGPSWayPoint::setGpsSpeedDouble(const double &value)
{
    gpsSpeed = value;
}

double mspGPSWayPoint::getGpsCOGDouble() const
{
    return double(gpsCOG)/10;
}

void mspGPSWayPoint::setGpsCOG(const double &value)
{
    gpsCOG = uint32_t(value*10);
}

QString mspGPSWayPoint::toXML(bool values,bool conf){
    return QString();
}
void mspGPSWayPoint::readXML(QString i){}
void mspGPSWayPoint::readDomElement(QDomElement &elem){

    QDomNode n = elem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "wpid") {
                gpsWPID = e.text().toInt();
            }
            else if (e.tagName() == "fix") {
                gpsFix = e.text().toInt();
            }
            else if (e.tagName() == "numsat") {
                gpsNumSat = e.text().toInt();
            }
            else if (e.tagName() == "lat") {
                gpsLat = e.text().toInt();
            }
            else if (e.tagName() == "long") {
                gpsLong = e.text().toInt();
            }
            else if (e.tagName() == "alt") {
                gpsAlt = e.text().toInt();
            }
            else if (e.tagName() == "speed") {
                gpsSpeed = e.text().toInt();
            }
            else if (e.tagName() == "cog") {
                gpsCOG = e.text().toInt();
            }
            else if (e.tagName() == "althold") {
                wpAltHold = e.text().toInt();
            }
            else if (e.tagName() == "heading") {
                wpHeading = e.text().toInt();
            }
            else if (e.tagName() == "timetostay") {
                wpTimeToStay = e.text().toInt();
            }
            else if (e.tagName() == "navflag") {
                wpNavFlag = e.text().toInt();
            }
        }
        n = n.nextSibling();
    }

}
void mspGPSWayPoint::addtoDomDoc(QDomDocument &d,QDomElement &e,bool values,bool conf){

    QDomElement wpi = d.createElement("gps");
    e.appendChild(wpi);

  if (values || conf) {
    addIntElement(d,wpi,"wpid",gpsWPID);
    addIntElement(d,wpi,"fix",gpsFix);
    addIntElement(d,wpi,"numsat",gpsNumSat);
    addIntElement(d,wpi,"lat",gpsLat);
    addIntElement(d,wpi,"long",gpsLong);
    addIntElement(d,wpi,"alt",gpsAlt);
    addIntElement(d,wpi,"speed",gpsSpeed);
    addIntElement(d,wpi,"cog",gpsCOG);
    addIntElement(d,wpi,"althold",wpAltHold);
    addIntElement(d,wpi,"heading",wpHeading);
    addIntElement(d,wpi,"timetostay",wpTimeToStay);
    addIntElement(d,wpi,"navflag",wpNavFlag);
  }
}




//-- END OF mspGPSWayPoint


rpi_multiwiidata::rpi_multiwiidata()
{
    motorLevel = new int[8] ;
    rcLevel = new int[8];
}

rpi_multiwiidata::~rpi_multiwiidata()
{
    delete [] motorLevel ;
    delete [] rcLevel ;
}

QString rpi_multiwiidata::toXML(bool values,bool conf) {
    QDomDocument xmlDoc("GPS");
    QDomElement root = xmlDoc.createElement("GPS");
    xmlDoc.appendChild(root);
    addtoDomDoc(xmlDoc,root,values,conf);
    return xmlDoc.toString();
}

void rpi_multiwiidata::addtoDomDoc(QDomDocument &d,QDomElement &e,
                                 bool values,bool conf){
    rpi_device::addtoDomDoc(d,e,conf);


    QDomElement wpe = d.createElement("waypoints");
    e.appendChild(wpe);
    QDomElement gpse = d.createElement("currentgps");
    e.appendChild(gpse);

    QDomElement uint16e = d.createElement("uint16");
    e.appendChild(uint16e);


    QDomElement int32e = d.createElement("int32");
    e.appendChild(int32e);

    QList<QString> uint16l = UnSigned16Bit.keys();
    QList<QString> int32l = Signed32Bit.keys();


    if (values) {
        currentGPSPosition.addtoDomDoc(d,gpse,values,conf);

        for(int k=0;k<wayPoints.size();k++)
        {
            wayPoints[k].addtoDomDoc(d,wpe,values,conf);
        }

        for(int k=0;k<UnSigned16Bit.size();k++)
        {
            addIntElement(d,uint16e,uint16l[k],UnSigned16Bit[uint16l[k]]);
        }

        for(int k=0;k<Signed32Bit.size();k++)
        {
            addIntElement(d,int32e,int32l[k],Signed32Bit[int32l[k]]);
            //wayPoints[k].addtoDomDoc(d,wpe,values,conf);
        }


        appendDoubleDocElement(d,e,"pitch",pitch);
        appendDoubleDocElement(d,e,"yaw",yaw);
        appendDoubleDocElement(d,e,"roll",roll);
        appendDoubleDocElement(d,e,"angx",angx);
        appendDoubleDocElement(d,e,"anyy",angy);
        appendDoubleDocElement(d,e,"heading",heading);

        appendIntDocElement(d,e,"sensorstatus",sensorStatus);
        appendIntDocElement(d,e,"motorlevel0",motorLevel[0]);
        appendIntDocElement(d,e,"motorlevel1",motorLevel[1]);
        appendIntDocElement(d,e,"motorlevel2",motorLevel[2]);
        appendIntDocElement(d,e,"motorlevel3",motorLevel[3]);
        appendIntDocElement(d,e,"motorlevel4",motorLevel[4]);
        appendIntDocElement(d,e,"motorlevel5",motorLevel[5]);
        appendIntDocElement(d,e,"motorlevel6",motorLevel[6]);
        appendIntDocElement(d,e,"motorlevel7",motorLevel[7]);

        appendIntDocElement(d,e,"rclevel0",rcLevel[0]);
        appendIntDocElement(d,e,"rclevel1",rcLevel[1]);
        appendIntDocElement(d,e,"rclevel2",rcLevel[2]);
        appendIntDocElement(d,e,"rclevel3",rcLevel[3]);
        appendIntDocElement(d,e,"rclevel4",rcLevel[4]);
        appendIntDocElement(d,e,"rclevel5",rcLevel[5]);
        appendIntDocElement(d,e,"rclevel6",rcLevel[6]);
        appendIntDocElement(d,e,"rclevel7",rcLevel[7]);



        QDomElement uint16_t_x = d.createElement("uint16");
        e.appendChild(uint16_t_x);

        QMap<QString, uint16_t>::const_iterator i16 = UnSigned16Bit.constBegin();
        while (i16 != UnSigned16Bit.constEnd()) {
            appendIntDocElement(d,uint16_t_x,i16.key(),i16.value());
            ++i16;
        }

        QDomElement int32_t_x = d.createElement("int32");
        e.appendChild(int32_t_x);

        QMap<QString, int32_t>::const_iterator i32 = Signed32Bit.constBegin();
        while (i32 != Signed32Bit.constEnd()) {
            appendIntDocElement(d,int32_t_x,i32.key(),i32.value());
            ++i32;

        }


    }

    rpi_device::addtoDomDoc(d,e,conf);
}

void rpi_multiwiidata::readXML(QString i){

    QDomDocument xmlDoc("multiwii");
    xmlDoc.setContent(i);

    QDomElement docElem = xmlDoc.documentElement();
    readDomElement(docElem);
}
void rpi_multiwiidata::readDomElement(QDomElement &elem){
    QDomNode n = elem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName(); currentgps waypoints uint16 int32
            if (e.tagName() == "currentgps") {
                //gpsWPID = e.text().toInt();
            }else if (e.tagName() == "currentgps") {
                currentGPSPosition.readDomElement(e);
            }else if (e.tagName() == "waypoints") {
                mspGPSWayPoint in;
                in.readDomElement(e);
                wayPoints << in;
            }else if (e.tagName() == "uint16") {
                readElementUint16(e,UnSigned16Bit);
            }else if (e.tagName() == "int32") {
                readElementInt32(e,Signed32Bit);
            }else if (e.tagName() == "pitch") {
                    pitch = fromHex(e.text().toLocal8Bit());
            }else if (e.tagName() == "yaw") {
                    yaw = fromHex(e.text().toLocal8Bit());
            }else if (e.tagName() == "roll") {
                    roll = fromHex(e.text().toLocal8Bit());
            }else if (e.tagName() == "angx") {
                    angx = fromHex(e.text().toLocal8Bit());
            }else if (e.tagName() == "anyy") {
                    angy = fromHex(e.text().toLocal8Bit());
            }else if (e.tagName() == "heading") {
                    heading = fromHex(e.text().toLocal8Bit());
            }else if (e.tagName() == "sensorstatus") {
                    sensorStatus = e.text().toInt();
            }else if (e.tagName() == "motorlevel0") {
                    motorLevel[0] = e.text().toInt();
            }else if (e.tagName() == "motorlevel1") {
                    motorLevel[1] = e.text().toInt();
            }else if (e.tagName() == "motorlevel2") {
                    motorLevel[2] = e.text().toInt();
            }else if (e.tagName() == "motorlevel3") {
                    motorLevel[3] = e.text().toInt();
            }else if (e.tagName() == "motorlevel4") {
                    motorLevel[4] = e.text().toInt();
            }else if (e.tagName() == "motorlevel5") {
                    motorLevel[5] = e.text().toInt();
            }else if (e.tagName() == "motorlevel6") {
                    motorLevel[6] = e.text().toInt();
            }else if (e.tagName() == "motorlevel7") {
                    motorLevel[7] = e.text().toInt();
            }else if (e.tagName() == "rclevel0") {
                    rcLevel[0] = e.text().toInt();
            }else if (e.tagName() == "rclevel1") {
                    rcLevel[1] = e.text().toInt();
            }else if (e.tagName() == "rclevel2") {
                    rcLevel[2] = e.text().toInt();
            }else if (e.tagName() == "rclevel3") {
                    rcLevel[3] = e.text().toInt();
            }else if (e.tagName() == "rclevel4") {
                    rcLevel[4] = e.text().toInt();
            }else if (e.tagName() == "rclevel5") {
                    rcLevel[5] = e.text().toInt();
            }else if (e.tagName() == "rclevel6") {
                    rcLevel[6] = e.text().toInt();
            }else if (e.tagName() == "rclevel7") {
                    rcLevel[7] = e.text().toInt();


            }

            int sensorStatus;
            int *motorLevel;
            int *rcLevel;
        }
        n = n.nextSibling();
    }
}

QStringList rpi_multiwiidata::functionTest() {
   QStringList r = rpi_device::functionTest();
    rpi_multiwiidata a;
    a.pitch = 2.2;
    a.roll = 10.3;
    a.yaw = 125.2;
    a.sensorStatus =12;
    a.motorLevel[0] =3;
    a.motorLevel[1] =30;
    a.motorLevel[2] =300;
    a.motorLevel[3] =323;
    a.motorLevel[4] =564;
    a.motorLevel[5] =434;
    a.motorLevel[6] =123;
    a.motorLevel[7] =564;
    a.rcLevel[0] =3;
    a.rcLevel[1] =30;
    a.rcLevel[2] =300;
    a.rcLevel[3] =323;
    a.rcLevel[4] =564;
    a.rcLevel[5] =434;
    a.rcLevel[6] =123;
    a.rcLevel[7] =564;
    a.angx = 2.2;
    a.angy = 10.3;
    a.heading = 125.2;
    a.UnSigned16Bit["boxarm"] = 3;
    a.UnSigned16Bit["boxgpshold"] = 1;

    a.Signed32Bit["boxheadhold"] =34;
    a.Signed32Bit["boxgpshome"] = 32;


    r << a.toXML();

    rpi_multiwiidata b;

    b.readXML(a.toXML());

    if (a.pitch != b.pitch) r << "Pitch Not Equal";
    if (a.yaw != b.yaw) r << "Yaw Not Equal";
    if (a.roll != b.roll) r << "Roll Not Equal";

    if (a.sensorStatus != b.sensorStatus) r << "Sensor Status Not Equal";

    for (int k=0;k<8;k++) {
        if (a.motorLevel[k] != b.motorLevel[k]) r << QString("Motor Level %1 Not Equal").arg(k);
        if (a.rcLevel[k] != b.rcLevel[k]) r << QString("RC Level %1 Not Equal").arg(k);

    }

    if (a.angx != b.angx) r << "Heading X Yaw Not Equal";
    if (a.angy != b.angy) r << "Heading Y Roll Not Equal";
    if (a.heading != b.heading) r << "Heading Not Equal";


    QMap<QString, uint16_t>::const_iterator i16 = a.UnSigned16Bit.constBegin();
    while (i16 != a.UnSigned16Bit.constEnd()) {
        if (b.UnSigned16Bit[i16.key()] != i16.value()) r << QString("UnSigned16Bit : %1 not equal").arg(i16.key());
        i16++;
    }


    QMap<QString, int32_t>::const_iterator i32 = a.Signed32Bit.constBegin();
    while (i32 != a.Signed32Bit.constEnd()) {
        if (b.Signed32Bit[i32.key()] != i32.value()) r << QString("UnSigned16Bit : %1 not equal").arg(i32.key());
        i32++;
    }



    return r;
}

bool rpi_multiwiidata::operator==(const rpi_multiwiidata &b)
{

    bool r = true;

    if (pitch!=b.pitch) return false;
    /*
    double pitch;
    double yaw;
    double roll;

    int sensorStatus;



    int *motorLevel;
    int *rcLevel;

    double angx;
    double anyy;
    double heading;
    */
    return   r;
}
