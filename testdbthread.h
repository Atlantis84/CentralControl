#ifndef TESTDBTHREAD_H
#define TESTDBTHREAD_H

#include <QObject>
#include <QThread>
class TestDBThread : public QThread
{
    Q_OBJECT
public:
    TestDBThread();
protected:
    void run();
};

#endif // TESTDBTHREAD_H
