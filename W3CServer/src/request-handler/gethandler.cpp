#include "gethandler.h"
#include <QDebug>


GetHandler::GetHandler(QObject* parent):RequestHandler(parent){

}

void GetHandler::processRequest(){
    qDebug() << " processing get handler requests";

}
