#ifndef RPI_LOG_H
#define RPI_LOG_H
#include <QString>
#include <QFile>
#include <QTextStream>



enum RPI_APP_MODE{RPI_CLIENT=1,RPI_SERVER=8};

class rpi_csv_log{
public:
    rpi_csv_log(QString f);
    ~rpi_csv_log();
    QString filename;
    QFile file;
    QTextStream& out();
        void nextDouble(double n);
    void nextString(QString s);
    void newLine();
    int currentRow();
    int currentColumn();
    QString write(QString text);

protected:
    QTextStream *outStream;
    int row;
    int column;

    bool isOpen;

};


#endif // RPI_LOG_H
