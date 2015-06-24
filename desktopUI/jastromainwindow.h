#ifndef JASTROMAINWINDOW_H
#define JASTROMAINWINDOW_H

#include <QFrame>
#include <QStackedLayout>
#include <QDebug>
#include <QResizeEvent>

class JAstroMainWindow : public QFrame {

   Q_OBJECT

public:
    JAstroMainWindow();

    void setCurrentWidget(QWidget * widget);
    int addWidget(QWidget * widget);

    void mousePressEvent (QMouseEvent * event);
    void closeEvent(QCloseEvent *event);
    void moveEvent (QMoveEvent * event);
    void resizeEvent (QResizeEvent * event );

signals:
    void mainwinClicked(QMouseEvent * event);
    void mainwinClosed (void);
    void mainwinMoved (QMoveEvent * event);
    void mainwinResized (QSize);

private:
    QStackedLayout * l;
};

#endif // JASTROMAINWINDOW_H
