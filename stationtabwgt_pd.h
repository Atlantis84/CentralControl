#ifndef STATIONTABWGT_H
#define STATIONTABWGT_H

#include <QWidget>
#include <QTableWidget>
class QCheckBox;
class StationTabWgt : public QWidget
{
    Q_OBJECT
public:
    explicit StationTabWgt(QWidget *parent = nullptr);

private:
    QWidget* createTableWidget();
    QTableWidget* tableWidget;
    QCheckBox* checkStationName;
    QCheckBox* checkProduceCapacity;
    QCheckBox* checkProduceCadence;
    QCheckBox* checkDate;
    QCheckBox* checkTime;

signals:

public slots:
};

#endif // STATIONTABWGT_H
