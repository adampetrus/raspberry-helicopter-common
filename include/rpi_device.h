#ifndef RPI_DEVICE_H
#define RPI_DEVICE_H
#include <QDomElement>
#include <QString>
#include <QStringList>
#include <QDomElement>
#include <QDomDocument>
#include <QMap>
#include <stdint.h>

QByteArray reverseHexArray(QByteArray s);
double *tpsNode(QDomNode n);
double *xyzNode(QDomNode n);

void doubleXYZNode(QDomNode n,double *dest);
void floatXYZNode(QDomNode n,float *dest);
void intXYZNode(QDomNode n,int *dest);

void floatAngleNode(QDomNode n,float *dest);
void floatQuantNode(QDomNode n,float *dest);

void addXYZElementsFromDouble(QDomDocument &d,QDomElement &e,double *src);
void addXYZElementFromFloat(QDomDocument &d,QDomElement &e,float *src);
void addTPSElementsFromDouble(QDomDocument &d,QDomElement &e,double *src);
void addTPSElementsFromFloat(QDomDocument &d,QDomElement &e,float *src);
void addQuantElementsFromFloat(QDomDocument &d,QDomElement &e,float *src);
void addXYZElementFromInt(QDomDocument &d,QDomElement &e,int *src);

void addIntElement(QDomDocument &d,QDomElement &r,QString t,const int v);
void readElementInt32(QDomElement &e,QMap<QString,int32_t> &m);
void readElementUint16(QDomElement &e,QMap<QString,uint16_t> &m);

template<class T> void copyArray(T* srcArray,T* destArray,const int size) {
    for (int k=0;k<size;k++) {
        destArray[k] = srcArray[k];
    }
}

bool isArrayEqual(float *a,float *b,int s);
bool isArrayEqual(double *a,double *b,int s);
bool isArrayEqual(int *a,int *b,int s);


QByteArray fromDouble(double d);
double fromHex(QByteArray h);
QByteArray hexFromFloat(float d);
float floatFromHex(QByteArray h);
class rpi_device {
public:
    enum RPI_DEVICE { RPI_ACC, RPI_GYRO, RPI_PRESSURE, RPI_GPS, RPI_COMPASS, RPI_ULTRASONICSENSOR, RPI_IMU9DOF, RPI_CONTROL=50, RPI_SERVO, RPI_BIMOTOR, RPI_ESCMOTOR,  };
    enum SENSOR_MODE { SENSOR_MODE_CLIENT =1, SENSOR_MODE_SERVER =20 };
    enum RPI_DEVICE_NUMBERS{NUM_ZERO=0, NUM_ONE, NUM_TWO, NUM_THREE, NUM_FOUR, NUM_FIVE, NUM_SIX, NUM_SEVEN, NUM_EIGHT, NUM_NINE };
    rpi_device();
    rpi_device(const rpi_device &r);
    ~rpi_device();
    void setDevice(QString d);
    void setAddress(uint a);
    void setNode(uint n);
    uint node();
    uint address();
    QString device();
    void addtoDomDoc(QDomDocument &d,QDomElement &e,bool conf=true);
   void readDomElement(QDomElement &e);
    double xDev();
    double yDev();
    double zDev();
    double thetaDev();
    double phiDev();
    double sigmaDev();
    void setXDev(double i);
    void setYDev(double i);
    void setZDev(double i);
    void setThetaDev(double i);
    void setPhiDev(double i);
    void setSigmaDev(double i);
    int type();
    void setType(int t);
    rpi_device* basicDevice();
    void setId(int i);
    int id();
    static QStringList functionTest();
    bool operator==(const rpi_device &r);
    QString errorString();
protected:
    QString errorstring;
    uint type_v ;
    uint addr_v;
    QString device_v;
    uint node_v;
    double dev_x;
    double dev_y;
    double dev_z;
    double dev_theta;
    double dev_phi;
    double dev_sigma;
    int id_v;

};

class rpi_request {
public:
    enum REQUEST_TYPE{ REQUEST=1,REPLY, REQUEST_ACCELEROMETER, REQUEST_GYROSCOPE, REQUEST_COMPASS,
                     REQUEST_PRESSSURE, REQUEST_ULTRASONIC, REQUEST_CONTROL, REQUEST_GPS,
                     REQUEST_ULTRASONIC_LIST_SIZE,REQUEST_CONTROL_LIST_SIZE,
                     REQUEST_DEVICES_LIST, REQUEST_SAVE_CONFIG,
                     REQUEST_INSERT_CONTROL, REQUEST_INSERT_ULTRASONIC,
                     REQUEST_REMOVE_CONTROL, REQUEST_REMOVE_ULTRASONIC, REQUEST_MULTIWII_STATUS,
                     REQUEST_MULTIWII_ENABLEBOX, REQUEST_MULTIWII_SETWP, REQUEST_MULTIWII_SETRAW_MOTORS
                       , REQUEST_MULTIWII_SETRAW_RC,
                     REQUEST_MULTIWII_ACC_CALIB, REQUEST_MULTIWII_MAG_CALIB, REQUEST_MULTIWII_RESET_CONF,
                       REQUEST_MULTIWII_SETHEAD, REQUEST_MULTIWII_EERROM_WRITE
                     };
    rpi_request(int mode);
    ~rpi_request();
    QString toXML();

    void readXML(QString i);
    void readDomElement(QDomElement &elem);
    QString& request();
    QStringList& reply();
    int type();
private:
    QString request_v;
    QStringList reply_v;
    int type_v;
    int cMode;
};

#endif // RPI_DEVICE_H
