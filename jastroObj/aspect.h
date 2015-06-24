#ifndef ASPECT_H
#define ASPECT_H

#include <QObject>

class Aspect : public QObject {
    Q_OBJECT

public:
    Aspect(float p1, float p2, int angle, float deviation, float orbis);
    float p1;          /* first planets position [0 .. 35959']                    */
    float p2;          /* second planets position [0 .. 35959']                   */
    int angle;         /* e.g. 0, 30, 72, 90 etc.                                 */
    float deviation;   /* deviation of aspect from exact angle, in                */
    float orbis;       /* maximum allowed deviation of this type of aspect, in    */
};

#endif // ASPECT_H
