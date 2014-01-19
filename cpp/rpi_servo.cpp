#include "rpi_log.h"
#include "rpi_control.h"
#include <QDebug>
/*
rpi_servo::rpi_servo() : rpi_control()
{
    //if (mode == 8) return;
}

rpi_servo::~rpi_servo() { }
*/
bool rpi_servo::setValue(uint v){
    qDebug() << "Set Servo" << node_v << v;
    char hexValue[4];
    sprintf(hexValue,"%.2X",v);
    QString cmd = "/usr/bin/bw_tool -a FE -w %1:%2";

    for (int s=20;s<27;s++) {
        qDebug() << cmd.arg(s).arg(hexValue);
    }
    return false;
}

uint rpi_servo::readValue(){
    return value_v;
}


