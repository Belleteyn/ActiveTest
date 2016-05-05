#ifndef STUFF_H
#define STUFF_H

#include <QObject>

#include "SMSObjMgr.tlh"

class Stuff : public QObject
{
    Q_OBJECT
public:
    explicit Stuff(QObject *parent = 0);

    Q_INVOKABLE void doSome();


public slots:
};

#endif // STUFF_H
