#ifndef TESTPLANTHREAD_H
#define TESTPLANTHREAD_H

#include <QThread>
class TestPlanThread : public QThread
{
    Q_OBJECT
public:
    TestPlanThread();
protected:
    void run();
};

#endif // TESTPLANTHREAD_H
