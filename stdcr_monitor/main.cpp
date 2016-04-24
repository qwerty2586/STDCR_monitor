/**
 * STDCR_monitor
 *
 * Program slouzi k nastaveni stimulatoru vyvijem Zapadoceskou univerzitou v Plzni
 * Jedna se u uzivatelske rozhrani, primarne urcene k chodu na raspberry pi,
 * ktere pomoci serioveho portu bude odesilat data do stimulatoru, a zobrazovat data prijata.
 * Vice o samotnem stimulatoru
 * https://www.zcu.cz/research/publications/?id=43913534
 *
 * github projektu
 * https://github.com/neurofishteam/STDCR_monitor
 *
 * V programu je pouzita knihovna QextSerialPort, dekuji vyvojarum za funkci komunikacni knihovnu
 * https://github.com/qextserialport/qextserialport
 *
 */

#include <QApplication>
#include <QPushButton>
#include "main_window.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    MainWindow window;

    window.show();
    return app.exec();
}
