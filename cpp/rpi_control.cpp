#include "rpi_log.h"
#include "rpi_control.h"
#include <QDateTime>
#include <QDomDocument>
#include <QDebug>

rpi_control::rpi_control(){
      setType(rpi_device::RPI_CONTROL);
}

rpi_control::rpi_control(const rpi_control &c) : rpi_device(c)  {
    //rpi_device::rpi_device(c);
    setValue(c.value_v);
    type_v = c.value_v;

}
    //if (mode == 8) return;
//}

#ifndef BITWIZARD_SPI

bool rpi_control::setValue(uint v){
/*    qDebug() << "Set Servo" << node_v << v;
    char hexValue[4];
    sprintf(hexValue,"%.2X",v);
    QString cmd = "/usr/bin/bw_tool -a FE -w %1:%2";

    for (int s=20;s<27;s++) {
        qDebug() << cmd.arg(s).arg(hexValue);
    }*/

    qDebug() << "BITWIZARD is NOT Defined just storing value";
    value_v =v;
    return false;
}
#endif
/*
uint rpi_control::readValue(){
    return value;
}*/

//virtual bool rpi_control::setValue(uint v) {}
//virtual uint rpi_control::readValue() {}


rpi_control::~rpi_control(){}
void rpi_control::startLog(QString filename){}
void rpi_control::logEntry(){}
void rpi_control::stopLog(){}


QString rpi_control::toXML(bool values,bool conf){
    QString name = "Control";
    switch (type_v) {
        case SERVO : { name = "Servo"; break;}
        case BIMOTOR :{ name = "BidirectionalMotor"; break;}
        case ESCMOTOR :{ name = "EscMotor"; break;}\
        default : { break;}
    }
    QDomDocument xmlDoc(name);
    QDomElement root = xmlDoc.createElement(name);
    xmlDoc.appendChild(root);
    addtoDomDoc(xmlDoc,root,values,conf);
    return xmlDoc.toString();

}
void rpi_control::addtoDomDoc(QDomDocument &d,QDomElement &e,bool values,bool conf) {

    if (values) {
    QDomElement evalue = d.createElement("value");
    QDomText tvalue = d.createTextNode(QString("%1").arg(value_v));
    QDomElement etype = d.createElement("type");
    QDomText ttype = d.createTextNode(QString("%1").arg(type_v));

    e.appendChild(evalue);
    evalue.appendChild(tvalue);
    e.appendChild(etype);
    etype.appendChild(ttype);


    }
    rpi_device::addtoDomDoc(d,e,conf);
}

void rpi_control::readXML(QString i){


    QDomDocument xmlDoc("Control");
    xmlDoc.setContent(i);

    QDomElement docElem = xmlDoc.documentElement();
    readDomElement(docElem);

    /*if (docElem.tagName() == "Servo" ) { type_v = SERVO;}
    else if (docElem.tagName() == "BidirectionalMotor" ) { type_v = BIMOTOR;}
    else if (docElem.tagName() == "EscMotor" ) { type_v = ESCMOTOR;}*/


  /*  QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
	    qDebug() << e.tagName();
            if (e.tagName() == "value") {
		setValue(e.text().toInt());
                //value_v = e.text().toInt();
            }
        }
        n = n.nextSibling();
    }*/


}
void rpi_control::readDomElement(QDomElement &e) {
    rpi_device::readDomElement(e);    
    QDomNode n = e.firstChild();
    
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            if (e.tagName() == "value") {
                setValue(e.text().toInt());
            }
            if (e.tagName() == "type") {
                setType(e.text().toInt());
            }
        }
        n = n.nextSibling();
    }
    
}

void rpi_control::reset() {
    switch (type_v) {
        case SERVO : { setValue(128); break;}
        case BIMOTOR :{ setValue(128); break;}
        case ESCMOTOR  :{ setValue(0); break;}
        default : {break;}
    }
}

uint rpi_control::value() {
    return value_v;
}
