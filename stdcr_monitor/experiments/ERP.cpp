//
// Created by qwerty on 16. 3. 2016.
//

#include "ERP.h"
//ERP::NAME = QString::fromUtf8("ERP");

ERP::ERP(QWidget *parent) : Experiment(parent) {


};


QString ERP::getName() {
    return QString::fromUtf8("ERP");
};

bool ERP::isImplemented() {
    return true;
}
