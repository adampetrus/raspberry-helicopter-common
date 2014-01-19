#include <QString>
#include <QTextStream>
#include <QFile>
#include "rpi_log.h"

rpi_csv_log::rpi_csv_log(QString f){
     filename = f;
     outStream =0;
     file.setFileName(filename);
     if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        outStream = new QTextStream(&file);
     }
     column=0;
     row = 0;

}


QTextStream& rpi_csv_log::out() { return *outStream;
}

int rpi_csv_log::currentColumn() { return column;}
int rpi_csv_log::currentRow() { return row;}

void rpi_csv_log::newLine() {
    if (outStream) { (*outStream) << endl;
        row++;
        column =0;
    } }
void rpi_csv_log::nextDouble(double n) {
    if (outStream) {
        if (column++) (*outStream) << ",";
        (*outStream) << n; }
}
void rpi_csv_log::nextString(QString s) {
    if (outStream) {
        if (column++) (*outStream) << ",";
        (*outStream) << s; }
}

QString rpi_csv_log::write(QString text){
    if (outStream)  {
        (*outStream) << text << endl;
        return text; }
    return "";

}
rpi_csv_log::~rpi_csv_log() {
     if (outStream) {
            file.close();
            delete outStream;
            outStream =0;
     }

}
