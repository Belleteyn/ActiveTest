#include "SystemTray.h"

#include <QGuiApplication>

SystemTray::SystemTray(QObject *parent)
  : QObject(parent)
{
  trayIcon = new QSystemTrayIcon();
  trayIcon->setIcon(QIcon(":/icon.ico"));
  trayIcon->show();
  trayIcon->setToolTip("SMSClient");

  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void SystemTray::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch (reason)
  {
  case QSystemTrayIcon::Trigger:
      signalIconActivated();
      break;
  default:
      break;
  }
}

void SystemTray::hideIconTray()
{
  trayIcon->hide();
}
