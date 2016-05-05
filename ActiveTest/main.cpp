#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QAxObject>
#include <QObject>
#include <QDebug>

#include <Boss.h>

int main(int argc, char *argv[])
{
  QGuiApplication a(argc, argv);

  Boss boss;

  //    BSTR name1;
  //    BSTR name2;
  //    hr = smsObjectManager->GetSMSObjectName(0, &name1);
  //    if (!FAILED(hr))
  //    {
  //      qDebug() << "saved sms object name" << 0;
  //    }
  //    hr = smsObjectManager->GetSMSObjectName(1, &name2);
  //    if (!FAILED(hr))
  //    {
  //      qDebug() << "saved sms object name" << 1;
  //    }

  //    ISMSObjectPtr smsObject;
  //    smsObject = iUnknown;
  //    if (smsObject == NULL)
  //    {
  //      qDebug() << "failed smsObject";
  //    }
  //    else
  //    {
  //      qDebug() << "succes smsObject";
  //    }

  //    QString qMessage = "ляляляляля лялял ляляля";
  //    BSTR message = SysAllocString(reinterpret_cast<const OLECHAR*>(qMessage.utf16()));

  //    hr = smsObject->SetMessage(message, name, 0, 0);
  //    if (FAILED(hr))
  //    {
  //      qDebug() << "failed send message";
  //    }
  //    else
  //    {
  //      qDebug() << "succes send message";
  //    }

  //    SysFreeString(name);
  //    SysFreeString(message);

  return a.exec();

//    QGuiApplication app(argc, argv);

//    SMSObjectManager smsObjectManager;

//    QQmlApplicationEngine engine;

//    engine.rootContext()->setContextProperty("smsObjectManager", &smsObjectManager);

//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

//    return app.exec();
}
