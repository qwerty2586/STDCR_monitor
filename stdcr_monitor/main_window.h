//
// Created by qwerty on 11.03.2016.
//



#ifndef STDCR_MONITOR_MAIN_WINDOW_H
#define STDCR_MONITOR_MAIN_WINDOW_H

#include <QObject>
#include <QWidget>


class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private:
    explicit void setWindowLayout();


};


#endif //STDCR_MONITOR_MAIN_WINDOW_H
