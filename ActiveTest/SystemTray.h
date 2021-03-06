#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include <QSystemTrayIcon>

class SystemTray : public QObject
{
  Q_OBJECT
public:
  explicit SystemTray(QObject *parent = 0);

signals:
  void signalIconActivated();

private slots:
  void iconActivated(QSystemTrayIcon::ActivationReason reason);

public slots:
  void hideIconTray();

private:
  QSystemTrayIcon* trayIcon;
};

#endif // SYSTEMTRAY_H
