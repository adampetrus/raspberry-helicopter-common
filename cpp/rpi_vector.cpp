#include "rpi_vector.h"
#include <math.h>
#include <QDebug>


QByteArray fromDouble(double d) {
    // create a char point referenced to the same address as the double
    char *c = (char*)&d;
    QByteArray r(c,8);
    return r.toHex();
}

double fromHex(QByteArray hex) {
    double r =0;
    QByteArray a =QByteArray::fromHex(hex);
    //qDebug() << "hex Size:" << hex.size();
    if (hex.size() == 16) {

        double *ra = (double*)a.data();
        r =*ra;
    }
    return r;

}

double earth_radius() {return 64000000;}
const double c_math_pi = 4.0 * atan(1);

double math_pi() { return c_math_pi;}
double deg2rad(double r) {return r*180.0/math_pi(); }
double rad2deg(double d) {return d*math_pi()/180.0; }

rpi_vector::rpi_vector(){}
rpi_vector::rpi_vector(double a,double b,double c,bool latlong){
    setVector(a,b,c,latlong);
}
void rpi_vector::setVector(double a,double b,double c,bool latlong){
    mode = latlong;
    if (latlong) {
       lat =a;
       longit = b;
       r = c + earth_radius();
    }else {
       x =a; y=b; z=c;
    }
    calcd = false;
}

void rpi_vector::calc(){
    if (calcd) return;
    if (mode) {
        x = r * sin(deg2rad(lat)) * cos(deg2rad(longit));
        y = r * sin(deg2rad(lat)) * sin(deg2rad(longit));
        z = r * cos(deg2rad(lat)) ;
    }else{

        r = sqrt(x*x+y*y+z*z);
        if (z!=0)
        {
            lat = rad2deg(acos(z/r));
            longit = rad2deg(acos(x/(r * sin(deg2rad(lat)))));
        }else{
            lat=0;
            longit = rad2deg(atan(y/x));
        }

    }
    calcd = true;
}


double rpi_vector::cartX(){
    if (mode) calc();
    return x;
}
double rpi_vector::cartY(){
    if (mode) calc();
    return y;
}
double rpi_vector::cartZ(){
    if (mode) calc();
    return z;
}
double rpi_vector::latitude(){
    if (!mode) calc();
    return lat;
}
double rpi_vector::longitude(){
    if (!mode) calc();
    return longit;
}
double rpi_vector::height(){
    if (!mode) calc();
    return r-earth_radius();
}
