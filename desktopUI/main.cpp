#include <QtWidgets/QApplication>
#include "controller.h"
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Controller * controller = new Controller();
    return a.exec();
}
