#include "rpi_device.h"
#include <QDebug>

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

    e.appendChild(eaddress);
    eaddress.appendChild(taddress);
    e.appendChild(edev);
    edev.appendChild(tdev);
    e.appendChild(enode);
    enode.appendChild(tnode);
    e.appendChild(eid);
    eid.appendChild(tid);

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
                r[0] = fromHex(e.text().toLocal8Bit());
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



double *tpsNode(QDomNode n) {
    double *r = new double [3];
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            //qDebug() << e.tagName();
            if (e.tagName() == "theta") {
                r[0] = fromHex(e.text().toLocal8Bit());
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
