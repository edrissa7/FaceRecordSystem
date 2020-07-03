#include "controller.h"

controller::controller()
{

}

void controller::run(){
    for(int i = 0; i <= 10000000; i++){
        qDebug()<<i;
    }
}
