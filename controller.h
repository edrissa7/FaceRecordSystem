#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QtCore>


class controller:public QThread
{
public:
    controller();
    void run();
};

#endif // CONTROLLER_H
