#ifndef COMMANAGER_H
#define COMMANAGER_H

#include <QAxBase>

class COMManager : public QObject
{
  Q_OBJECT

public:
  COMManager();
  ~COMManager();

private:
  //QAxObject axObject_;
};

#endif // COMMANAGER_H
