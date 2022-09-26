#ifndef MESPROCESS_H
#define MESPROCESS_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
class MesProcess : public QObject
{
    Q_OBJECT
public:
    explicit MesProcess(QObject *parent = nullptr);

    void exec_http_post(int desSign);//0:Mes 1:AOI 2:register 3:visuallead
    void exec_http_get(QString productsn);//for change production scanning code
private:
    QNetworkAccessManager* accessManager;

signals:
    void signal_to_ui(QByteArray* data);
public slots:
    void slot_urlReply(QNetworkReply* reply);
};

#endif // MESPROCESS_H
