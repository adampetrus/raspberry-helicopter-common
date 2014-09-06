#ifndef RPI_VECTOR_H
#define RPI_VECTOR_H
#include <QByteArray>





QByteArray fromDouble(double d);
double fromHex(QByteArray h);
double earth_radius();
double math_pi();

class rpi_vector {
public:
    rpi_vector();
    rpi_vector(const rpi_vector &s);
    rpi_vector(double a,double b,double c,bool latlong=false);
    void setVector(double a,double b,double c,bool latlong=false);
    double cartX();
    double cartY();
    double cartZ();
    double latitude();
    double longitude();
    double height();
    rpi_vector operator-(const rpi_vector &s);
protected:
    void calc();
    bool mode;
    bool calcd;
    double x;
    double y;
    double z;
    double lat;
    double longit;
    double theta;
    double r;

};

#endif // RPI_VECTOR_H
