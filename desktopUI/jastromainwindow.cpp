#include "jastromainwindow.h"
#include <QIcon>

JAstroMainWindow::JAstroMainWindow() {
    l = new QStackedLayout();
    setLayout(l);

    // load images from resource file
    QString imgPath = ":/img/";

    qDebug() << "JAstroMainWindow: imgPath =" << imgPath;

    QIcon windowIcon  = QIcon(imgPath+"composite.png");

    /* set application icon                         */
    this->setWindowIcon(windowIcon);
}

/*----------------------------------------------------------------------------------------------*/

void JAstroMainWindow::mousePressEvent ( QMouseEvent * event ) {
    emit (mainwinClicked(event));
}

/*----------------------------------------------------------------------------------------------*/

void JAstroMainWindow::closeEvent(QCloseEvent *event) {
    emit (mainwinClosed());
}

/*----------------------------------------------------------------------------------------------*/

void JAstroMainWindow::moveEvent ( QMoveEvent * event ) {
    emit (mainwinMoved (event));
}

/*----------------------------------------------------------------------------------------------*/

void JAstroMainWindow::resizeEvent ( QResizeEvent * event ) {
    emit mainwinResized (event->size());
}

/*----------------------------------------------------------------------------------------------*/

void JAstroMainWindow::setCurrentWidget(QWidget * widget) {
    this->l->setCurrentWidget(widget);
}

/*----------------------------------------------------------------------------------------------*/

int JAstroMainWindow::addWidget(QWidget * widget) {
    return this->l->addWidget(widget);
}

