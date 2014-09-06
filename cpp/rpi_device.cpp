#include "rpi_device.h"
#include <QDebug>

QByteArray reverseHexArray(QByteArray s) {
    QByteArray r;
    if (s.size() % 2) { return "0000000";}
    for (int k=s.size()/2-1;k>=0;k--)
    {
        r += s.mid(k*2,2);
    }
    return r;
}

rpi_device::rpi_device() {
    id_v=0;
    type_v =0;
    device_v = "/dev";
    addr_v = 1;
    node_v =1;

    dev_theta =0;
    dev_phi =0;
    dev_sigma =0;
    dev_x =0;
    dev_y =0;
    dev_z =0;

}
rpi_device::~rpi_device() { }

int rpi_device::type() {return type_v;}
void rpi_device::setType(int t) { type_v =t;}

rpi_device* rpi_device::basicDevice() {
    return this;
}
void rpi_device::setId(int i) { id_v =i;}
int rpi_device::id() { return id_v; }
void rpi_device::setDevice(QString d) { device_v = d;}
void rpi_device::setAddress(uint a) { addr_v =a;}
void rpi_device::setNode(uint n) { node_v =n; }
uint rpi_device::node() { return node_v;}
uint rpi_device::address() {return addr_v; }
QString rpi_device::device() { return device_v;}

rpi_device::rpi_device(const rpi_device &r) {
    type_v = r.type_v;
    addr_v = r.addr_v;
    node_v = r.node_v;
    device_v = r.device_v;
    id_v = r.id_v;
    dev_phi = r.dev_phi;
    dev_sigma = r.dev_sigma;
    dev_theta = r.dev_theta;
    dev_x = r.dev_x;
    dev_y = r.dev_y;
    dev_z = r.dev_z;

}



void rpi_device::addtoDomDoc(QDomDocument &d,QDomElement &e,bool conf) {

    QDomElement eaddress = d.createElement("address");
    QDomText taddress = d.createTextNode(QString("%1").arg(addr_v));
    QDomElement edev = d.createElement("device");
    QDomText tdev = d.createTextNode(device_v);
    QDomElement enode = d.createElement("node");
    QDomText tnode = d.createTextNode(QString("%1").arg(node_v));
    QDomElement eid = d.createElement("id");
    QDomText tid = d.createTextNode(QString("%1").arg(id_v));
    QDomElement etype = d.createElement("type");
    QDomText ttype = d.createTextNode(QString("%1").arg(type_v));


    e.appendChild(eaddress);
    eaddress.appendChild(taddress);
    e.appendChild(edev);
    edev.appendChild(tdev);
    e.appendChild(enode);
    enode.appendChild(tnode);
    e.appendChild(eid);
    eid.appendChild(tid);
    e.appendChild(etype);
    etype.appendChild(ttype);

    if (conf) {

        QDomElement elPosition = d.createElement("Location");
        QDomElement elxPosition = d.createElement("x");
        QDomElement elyPosition = d.createElement("y");
        QDomElement elzPosition = d.createElement("z");
        QDomText txPosition = d.createTextNode(QString(fromDouble(xDev())));
        QDomText tyPosition = d.createTextNode(QString(fromDouble(yDev())));
        QDomText tzPosition = d.createTextNode(QString(fromDouble(zDev())));
        e.appendChild(elPosition);
        elPosition.appendChild(elxPosition);
        elPosition.appendChild(elyPosition);
        elPosition.appendChild(elzPosition);
        elxPosition.appendChild(txPosition);
        elyPosition.appendChild(tyPosition);
        elzPosition.appendChild(tzPosition);

        QDomElement elOrientation = d.createElement("Orientation");
        QDomElement elthetaOrientation = d.createElement("theta");
        QDomElement elphiOrientation = d.createElement("phi");
        QDomElement elsigmaOrientation = d.createElement("sigma");
        QDomText tthetaOrientation = d.createTextNode(QString(fromDouble(thetaDev())));
        QDomText tphiOrientation = d.createTextNode(QString(fromDouble(phiDev())));
        QDomText tsigmaOrientation = d.createTextNode(QString(fromDouble(sigmaDev())));
        e.appendChild(elOrientation);
        elOrientation.appendChild(elthetaOrientation);
        elOrientation.appendChild(elphiOrientation);
        elOrientation.appendChild(elsigmaOrientation);
        elthetaOrientation.appendChild(tthetaOrientation);
        elphiOrientation.appendChild(tphiOrientation);
        elsigmaOrientation.appendChild(tsigmaOrientation);
    }


    return ;


}
void rpi_device::readDomElement(QDomElement &elem) {
    QDomNode n = elem.firstChild();
    while(!n.isNull()) {
        QDomElement f = n.toElement(); // try to convert the node to an element.
        if(!f.isNull()) {
            //qDebug() << f.tagName() << f.text();
            if (f.tagName() == "device") {
                device_v = f.text();
            }
            else if (f.tagName() == "address") {
                addr_v = f.text().toInt();
            }
            else if (f.tagName() == "node") {
                node_v = f.text().toInt();
            }
            else if (f.tagName() == "id") {
                id_v = f.text().toInt();
            }
            else if (f.tagName() == "type") {
                type_v = f.text().toInt();
            }

            else if (f.tagName() == "Location") {

                double *xyz = xyzNode(f.firstChildElement());
                setXDev(xyz[0]);
                setYDev(xyz[1]);
                setZDev(xyz[2]);
                //qDebug() << "Location" << xyz[0] << xyz[1] << xyz[2];
                delete [] xyz;
            }

            else if (f.tagName() == "Orientation") {
                double *tps = tpsNode(f.firstChildElement());
                setThetaDev(tps[0]);
                setPhiDev(tps[1]);
                setSigmaDev(tps[2]);
                delete [] tps;
            }
        }
        n = n.nextSibling();
    }

}

double rpi_device::xDev() { return dev_x; }
double rpi_device::yDev() { return dev_y; }
double rpi_device::zDev() { return dev_z; }
double rpi_device::thetaDev(){ return dev_theta;}
double rpi_device::phiDev() { return dev_phi; }
double rpi_device::sigmaDev(){ return dev_sigma; }
void rpi_device::setXDev(double i) {  dev_x =i ;}
void rpi_device::setYDev(double i) { dev_y =i;}
void rpi_device::setZDev(double i) { dev_z =i;}
void rpi_device::setThetaDev(double i) {dev_theta =i;}
void rpi_device::setPhiDev(double i) { dev_phi= i;}
void rpi_device::setSigmaDev(double i) {dev_sigma = i; }
QString rpi_device::errorString(){
    return errorstring;
}

bool rpi_device::operator==(const rpi_device &r){
    bool ret =true;
    errorstring = "";
    if (r.type_v != type_v) { errorstring += QString("Type is not equal,(%1,%2):").arg(r.type_v).arg(type_v); ret= false; }
    if (r.addr_v != addr_v) { errorstring  += QString("Address is not equal,(%1,%2):").arg(r.addr_v).arg(addr_v); ret= false; }
    if (r.node_v != node_v) { errorstring  += QString("Node Value is not equal,(%1,%2):").arg(r.node_v).arg(node_v); ret= false; }
    if (r.dev_x != dev_x) { errorstring  += QString("Device X Location is not equal,(%1,%2):").arg(r.dev_x).arg(dev_x); ret= false; }
    if (r.dev_y != dev_y) { errorstring  += QString("Device Y Location is not equal,(%1,%2):").arg(r.dev_y).arg(dev_y); ret= false; }
    if (r.dev_z != dev_z) { errorstring  += QString("Device Z Location is not equal,(%1,%2):").arg(r.dev_z).arg(dev_z); ret= false; }
    if (r.dev_theta != dev_theta) { errorstring  += QString("Device Theta Orientation is not equal,(%1,%2):").arg(r.dev_theta).arg(dev_theta); ret= false; }
    if (r.dev_phi != dev_phi) { errorstring  += QString("Device Phi Orientation is not equal,(%1,%2):").arg(r.dev_phi).arg(dev_phi); ret= false; }
    if (r.dev_sigma != dev_sigma) { errorstring  += QString("Device Sigma Orientation is not equal,(%1,%2):").arg(r.dev_sigma).arg(dev_sigma); ret= false; }
    if (r.id_v != id_v) { errorstring  += QString("Device ID is not equal,(%1,%2):").arg(r.id_v).arg(id_v); ret= false; }
    return ret;
}

rpi_request::rpi_request(int mode) {
    type_v= mode;
    cMode = mode;
    switch (type_v) {
        case rpi_request::REQUEST_ACCELEROMETER :  {
            request_v = "Accelerometer";
            break;
        }
        case rpi_request::REQUEST_GYROSCOPE :  {
            request_v = "Gyroscope";
            break;
        }
        case rpi_request::REQUEST_COMPASS :  {
            request_v = "Compass";
            break;
        }
        case rpi_request::REQUEST_GPS :  {
            request_v = "GPS";
            break;
        }
        case rpi_request::REQUEST_PRESSSURE :  {
            request_v = "Pressure";
            break;
        }
        case rpi_request::REQUEST_ULTRASONIC :  {
            request_v = "Ultrasonic";
            break;
        }
        case rpi_request::REQUEST_CONTROL :  {
            request_v = "Control";
            break;
        }
        case rpi_request::REQUEST_ULTRASONIC_LIST_SIZE :  {
            request_v = "Ultrasonic_List";
            break;
        }
        case rpi_request::REQUEST_CONTROL_LIST_SIZE :  {
            request_v = "Control_List";
            break;
        }

        case rpi_request::REQUEST_INSERT_CONTROL:  {
            request_v = "Insert_Control";
            break;
        }
        case rpi_request::REQUEST_INSERT_ULTRASONIC :  {
            request_v = "Insert_Ultrasonic";
            break;
        }
        case rpi_request::REQUEST_REMOVE_CONTROL :  {
            request_v = "Remove_Control";
            break;
        }
        case rpi_request::REQUEST_REMOVE_ULTRASONIC :  {
            request_v = "Remove_Ultrasonic";
            break;
        }
        case rpi_request::REQUEST_SAVE_CONFIG :  {
            request_v = "Save_Config";
            break;
        }

        case rpi_request::REQUEST_DEVICES_LIST :  {
            request_v = "Device_List";
            break;
        }
        default : {break;}
    }

}
rpi_request::~rpi_request() {}
QString rpi_request::toXML() {
    QDomDocument xmlDoc("request");
    QDomElement root = xmlDoc.createElement("request");
    QDomElement erequest = xmlDoc.createElement("request");
    QDomText trequest = xmlDoc.createTextNode(request());
    QDomElement ereply = xmlDoc.createElement("reply");
    xmlDoc.appendChild(root);
    root.appendChild(erequest);
    erequest.appendChild(trequest);
    root.appendChild(ereply);
    for (int k=0;k<reply().size();k++) {
        QDomElement ereplyk = xmlDoc.createElement("reply");
        QDomText treplyk = xmlDoc.createTextNode(reply()[k]);
        ereply.appendChild(treplyk);
        ereply.appendChild(ereplyk);
    }

    return xmlDoc.toString();
}
int rpi_request::type() { return type_v; }

void rpi_request::readDomElement(QDomElement &docElem) {
    reply().clear();

    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            if (e.tagName() == "request") {
                request() = e.text();
                    if (request_v == "Accelerometer") { type_v =REQUEST_ACCELEROMETER; }
                    else if (request_v == "Gyroscope") { type_v =REQUEST_GYROSCOPE; }
                    else if (request_v == "Compass") { type_v =REQUEST_COMPASS; }
                    else if (request_v == "Pressure") { type_v =REQUEST_PRESSSURE; }
                    else if (request_v == "Ultrasonic") { type_v =REQUEST_ULTRASONIC; }
                    else if (request_v == "Controls") { type_v =REQUEST_CONTROL; }
                    else if (request_v == "Device_List") { type_v =REQUEST_DEVICES_LIST; }
                    else if (request_v == "Save_Config") { type_v =REQUEST_SAVE_CONFIG; }
                    else if (request_v == "GPS") { type_v =REQUEST_GPS; }
                    else if (request_v == "Ultrasonic_List") { type_v =REQUEST_ULTRASONIC_LIST_SIZE; }
                    else if (request_v == "Control_List") { type_v =REQUEST_CONTROL_LIST_SIZE; }

                    else if (request_v == "Insert_Control") { type_v =REQUEST_INSERT_CONTROL; }
                    else if (request_v == "Insert_Ultrasonic") { type_v =REQUEST_INSERT_ULTRASONIC; }
                    else if (request_v == "Remove_Control") { type_v =REQUEST_REMOVE_CONTROL; }
                    else if (request_v == "Remove_Ultrasonic") { type_v =REQUEST_REMOVE_ULTRASONIC; }
                    //else if (request_v == "Accelerometer") { type_v =REQUEST_ACCELEROMETER; }
                    //else if (request_v == "Accelerometer") { type_v =REQUEST_ACCELEROMETER; }
                
                //qDebug() << "pressure" << e.text() << tpress;
            }
            if (e.tagName() == "reply") {
                QDomNode nr = e.firstChild();
                while(!nr.isNull()) {
                    QDomElement e = n.toElement(); // try to convert the node to an element.
                    if(!e.isNull()) {
                        if (e.tagName() == "reply") {
                            reply() << e.text();
                        }
                    }
                    }
                    nr = nr.nextSibling();
                }

            }
        n = n.nextSibling();
    }

}


void rpi_request::readXML(QString i) {
    QDomDocument xmlDoc("request");
    xmlDoc.setContent(i);
    QDomElement docElem = xmlDoc.documentElement();
}
QString& rpi_request::request() { return request_v;  }
QStringList& rpi_request::reply() { return reply_v ;}


double *xyzNode(QDomNode n) {
    double *r = new double [3];
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "x") {
                r[rpi_device::NUM_ZERO] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "y") {
                r[1] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "z") {
                r[2] = fromHex(e.text().toLocal8Bit());
            }
            //qDebug() << e.tagName() << e.text() << fromHex(e.text().toLocal8Bit());
        }
        n = n.nextSibling();
    }
    return r;
}


double *quadNode(QDomNode n) {
    double *r = new double [3];
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "q0") {
                r[rpi_device::NUM_ZERO] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "q1") {
                r[1] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "q2") {
                r[2] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "q3") {
                r[3] = fromHex(e.text().toLocal8Bit());
            }
        }
        n = n.nextSibling();
    }
    return r;
}


void doubleXYZNode(QDomNode n,double *dest){
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "x") {
                dest[rpi_device::NUM_ZERO] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "y") {
                dest[1] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "z") {
                dest[2] = fromHex(e.text().toLocal8Bit());
            }
        }
        n = n.nextSibling();
    }
    return ;



}
void floatXYZNode(QDomNode n,float *dest){
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "x") {
                dest[rpi_device::NUM_ZERO] = floatFromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "y") {
                dest[1] = floatFromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "z") {
                dest[2] = floatFromHex(e.text().toLocal8Bit());
            }
        }
        n = n.nextSibling();
    }
    return ;




}
void intXYZNode(QDomNode n,int *dest){
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "x") {
                dest[rpi_device::NUM_ZERO] = e.text().toInt();
            }
            if (e.tagName() == "y") {
                dest[1] = e.text().toInt();
            }
            if (e.tagName() == "z") {
                dest[2] = e.text().toInt();
            }
        }
        n = n.nextSibling();
    }
    return ;
}

void floatAngleNode(QDomNode n,float *dest){
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "theta") {
                dest[rpi_device::NUM_ZERO] = floatFromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "phi") {
                dest[1] = floatFromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "psi") {
                dest[2] = floatFromHex(e.text().toLocal8Bit());
            }
        }
        n = n.nextSibling();
    }
    return ;

}
void floatQuantNode(QDomNode n,float *dest){

    while(!n.isNull()) {
    QDomElement e = n.toElement(); // try to convert the node to an element.
    if(!e.isNull()) {
        //qDebug() << e.tagName() << floatFromHex(e.text().toLocal8Bit()) << e.text()  ;
        if (e.tagName() == "q0") {
            dest[0] = floatFromHex(e.text().toLocal8Bit());
        }
        if (e.tagName() == "q1") {
            dest[1] = floatFromHex(e.text().toLocal8Bit());
        }
        if (e.tagName() == "q2") {
            dest[2] = floatFromHex(e.text().toLocal8Bit());
        }
        if (e.tagName() == "q3") {
            dest[3] = floatFromHex(e.text().toLocal8Bit());
        }
    }
    n = n.nextSibling();
}
return ;

}


double *tpsNode(QDomNode n) {
    double *r = new double [3];
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "theta") {
                r[rpi_device::NUM_ZERO] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "phi") {
                r[1] = fromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "sigma") {
                r[2] = fromHex(e.text().toLocal8Bit());
            }
        }
        n = n.nextSibling();
    }
    return r;
}



void xyzFloatNode(QDomNode n,float *dest) {
        while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "x") {
                dest[rpi_device::NUM_ZERO] = floatFromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "y") {
                dest[1] = floatFromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "z") {
                dest[2] = floatFromHex(e.text().toLocal8Bit());
            }
        }
        n = n.nextSibling();
    }
    return ;
}

void  quantFloatNode(QDomNode n,float *dest) {
        while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "q0") {
                dest[rpi_device::NUM_ZERO] = floatFromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "q1") {
                dest[1] = floatFromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "q2") {
                dest[2] = floatFromHex(e.text().toLocal8Bit());
            }
            if (e.tagName() == "q3") {
                dest[3] = floatFromHex(e.text().toLocal8Bit());
            }
        }
        n = n.nextSibling();
    }
    return ;
}

QStringList rpi_device::functionTest(){
    QStringList ret;
    rpi_device testDev1;
    testDev1.setAddress(37);
    testDev1.setDevice("Sensor");
    testDev1.setId(12);
    testDev1.setNode(67);
    testDev1.setPhiDev(34.23);
    testDev1.setThetaDev(182.35);
    testDev1.setSigmaDev(28.65);
    testDev1.setType(RPI_PRESSURE);
    testDev1.setXDev(-34.2);
    testDev1.setYDev(12.45);
    testDev1.setZDev(0.32);

    if ( (testDev1.address() != 37) ) ret << "Address Not Set";
    if ( (testDev1.device() != "Sensor") ) ret << "Device Name Not Set";
    if ( (testDev1.id() != 12) ) ret << "Id Not Set";
    if ( (testDev1.node() != 67) ) ret << "Node Not Set";
    if ( (testDev1.phiDev() != 34.23) ) ret << "Phi Not Set";
    if ( (testDev1.thetaDev() != 182.35) ) ret << "theta Not Set";
    if ( (testDev1.sigmaDev() != 28.65) ) ret << "Sigma Not Set";
    if ( (testDev1.type() != RPI_PRESSURE) ) ret << "Type Not Set";
    if ( (testDev1.xDev() != -34.2) ) ret << "x not set";
    if ( (testDev1.yDev() != 12.45) ) ret << "y not set";
    if ( (testDev1.zDev() != 0.32) ) ret << "z not set";

    rpi_device testDevEqual(testDev1);

    if (!(testDevEqual == testDev1)) {
        if (testDevEqual.errorString().size())
            ret << "Dev Equal Test";
        ret << testDevEqual.errorString();}

    QDomDocument xmlDoc("TestDevice");
    QDomElement root = xmlDoc.createElement("TestDevice");
    xmlDoc.appendChild(root);

    testDev1.addtoDomDoc(xmlDoc,root);
    QString xmlStr=  xmlDoc.toString();
    QDomDocument xmlDoc2("TestDevice");
    xmlDoc2.setContent(xmlStr);
    QDomElement docElem = xmlDoc2.documentElement();

    rpi_device testDevXml ;
    testDevXml.readDomElement(docElem);

    if (!(testDevXml == testDev1)) {
        if (testDevXml.errorString().size())
            ret << "Dev XML Test";
        ret << testDevXml.errorString();}


    return ret;
}



void addXYZElementsFromDouble(QDomDocument &d,QDomElement &e,double *src){
    QDomElement ex = d.createElement("x");
    QDomElement ey = d.createElement("y");
    QDomElement ez = d.createElement("z");
    QDomText tx = d.createTextNode(QString(fromDouble(src[rpi_device::NUM_ZERO])));
    QDomText ty = d.createTextNode(QString(fromDouble(src[1])));
    QDomText tz = d.createTextNode(QString(fromDouble(src[2])));
    e.appendChild(ex);
    e.appendChild(ey);
    e.appendChild(ez);
    ex.appendChild(tx);
    ey.appendChild(ty);
    ez.appendChild(tz);

}

void addXYZElementFromFloat(QDomDocument &d,QDomElement &e,float *src){

    QDomElement ex = d.createElement("x");
    QDomElement ey = d.createElement("y");
    QDomElement ez = d.createElement("z");
    QDomText tx = d.createTextNode(QString(hexFromFloat(src[rpi_device::NUM_ZERO])));
    QDomText ty = d.createTextNode(QString(hexFromFloat(src[1])));
    QDomText tz = d.createTextNode(QString(hexFromFloat(src[2])));
    e.appendChild(ex);
    e.appendChild(ey);
    e.appendChild(ez);
    ex.appendChild(tx);
    ey.appendChild(ty);
    ez.appendChild(tz);


}



void addXYZElementFromInt(QDomDocument &d,QDomElement &e,int *src){

    QDomElement ex = d.createElement("x");
    QDomElement ey = d.createElement("y");
    QDomElement ez = d.createElement("z");
    QDomText tx = d.createTextNode(QString("%1").arg(src[rpi_device::NUM_ZERO]));
    QDomText ty = d.createTextNode(QString("%1").arg(src[1]));
    QDomText tz = d.createTextNode(QString("%1").arg(src[2]));
    e.appendChild(ex);
    e.appendChild(ey);
    e.appendChild(ez);
    ex.appendChild(tx);
    ey.appendChild(ty);
    ez.appendChild(tz);

}

void addTPSElementsFromDouble(QDomDocument &d,QDomElement &e,double *src){

    QDomElement et = d.createElement("theta");
    QDomElement ep = d.createElement("phi");
    QDomElement es = d.createElement("sigma");
    QDomText tt = d.createTextNode(QString(fromDouble(src[rpi_device::NUM_ZERO])));
    QDomText tp = d.createTextNode(QString(fromDouble(src[1])));
    QDomText ts = d.createTextNode(QString(fromDouble(src[2])));
    e.appendChild(et);
    e.appendChild(ep);
    e.appendChild(es);
    et.appendChild(tt);
    ep.appendChild(tp);
    es.appendChild(ts);

}

void addTPSElementsFromFloat(QDomDocument &d,QDomElement &e,float *src){

    QDomElement et = d.createElement("theta");
    QDomElement ep = d.createElement("phi");
    QDomElement es = d.createElement("sigma");
    QDomText tt = d.createTextNode(QString(hexFromFloat(src[rpi_device::NUM_ZERO])));
    QDomText tp = d.createTextNode(QString(hexFromFloat(src[1])));
    QDomText ts = d.createTextNode(QString(hexFromFloat(src[2])));
    e.appendChild(et);
    e.appendChild(ep);
    e.appendChild(es);
    et.appendChild(tt);
    ep.appendChild(tp);
    es.appendChild(ts);


}

void addQuantElementsFromFloat(QDomDocument &d,QDomElement &e,float *src){

    QDomElement eq0 = d.createElement("q0");
    QDomElement eq1= d.createElement("q1");
    QDomElement eq2 = d.createElement("q2");
    QDomElement eq3 = d.createElement("q3");

    QDomText tq0 = d.createTextNode(QString(hexFromFloat(src[rpi_device::NUM_ZERO])));
    QDomText tq1 = d.createTextNode(QString(hexFromFloat(src[rpi_device::NUM_ONE])));
    QDomText tq2 = d.createTextNode(QString(hexFromFloat(src[rpi_device::NUM_TWO])));
    QDomText tq3 = d.createTextNode(QString(hexFromFloat(src[rpi_device::NUM_THREE])));

    e.appendChild(eq0);
    e.appendChild(eq1);
    e.appendChild(eq2);
    e.appendChild(eq3);

    eq0.appendChild(tq0);
    eq1.appendChild(tq1);
    eq2.appendChild(tq2);
    eq3.appendChild(tq3);



}


bool isArrayEqual(float *a,float *b,int s){
    for (int k=0;k<s;k++) {
        //qDebug() isArrayEqual
        if (a[k]!=b[k]) return false;
    }
    return true;

}
bool isArrayEqual(double *a,double *b,int s){
    for (int k=0;k<s;k++) {
        if (a[k]!=b[k]) return false;
    }
    return true;
}
bool isArrayEqual(int *a,int *b,int s){
    for (int k=0;k<s;k++) {
        //qDebug()  << "isArrayEqual" << k << a[k] << b[k];
        if (a[k]!=b[k]) return false;
    }
    return true;
}
