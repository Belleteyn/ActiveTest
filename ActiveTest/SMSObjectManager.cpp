#include "SMSObjectManager.h"

#include <QSettings>
#include <QGuiApplication>
#include <QTime>

SMSObjectManager::SMSObjectManager(QObject *parent)
  : QObject(parent)
  , iSMSObjectManager_(NULL)
  , iSMSCallBack_(NULL)
  , iSMSObject_(NULL)
  , titleMonitorTimer_(nullptr)
{
//  iSMSCallBack_ = ISMSCallBackPtr(new CSMSCallback());
//  if (iSMSCallBack_)
//  {
//      qDebug() << "succes CreateInstance smsCallback";
//  }
//  else
//  {
//      qDebug() << "failed CreateInstance smsCallback";
//  }
//
//  hr = pIUnknown->QueryInterface(IID_ISMSCallBack,(void**)&iSMSCallBack_);
//  if (FAILED(hr))
//  {
//      qDebug() << "failed smsCallback";
//  }
//  else
//  {
//      qDebug() << "succes smsCallback";
//      iSMSCallBack_->AddRef();
//  }
}

SMSObjectManager::~SMSObjectManager()
{
  resetSMSObject();

  if (iSMSObjectManager_)
  {
    iSMSObjectManager_->Release();
    iSMSObjectManager_ = NULL;
  }

  if (iSMSCallBack_)
  {
    iSMSCallBack_->Release();
    iSMSCallBack_ = NULL;
  }
}

bool SMSObjectManager::init()
{
  if (!initSmsObjectManager())
    return false;

  if (!initSmsCallback())
    return false;

  showSmsObjects();
  return true;
}

bool SMSObjectManager::initSmsObjectManager()
{
  HRESULT hr = iSMSObjectManager_.CreateInstance(CLSID_SMSObjectsManager);
  if (!FAILED(hr))
  {
    qDebug() << "succes smsObjectManager";
    return true;
  }

  qDebug() << "failed smsObjectManager";
  return false;
}

bool SMSObjectManager::initSmsCallback()
{
  IUnknown* pIUnknown = static_cast<ISMSCallBack*>(new CSMSCallback);
  HRESULT hr = pIUnknown->QueryInterface(IID_ISMSCallBack,(void**)&iSMSCallBack_);
  if (!FAILED(hr))
  {
    qDebug() << "succes smsCallback";
    iSMSCallBack_->AddRef();
    return true;
  }

  qDebug() << "failed smsCallback";
  return false;
}

void SMSObjectManager::showSmsObjects()
{
  INT nObjectsCount = 0;
  HRESULT hr = iSMSObjectManager_->GetSMSObjectsCount(&nObjectsCount);
  if (FAILED(hr))
  {
    qDebug() << "failed GetSMSObjectsCount";
  }

  for (INT i = 0; i < nObjectsCount; i++)
  {
    BSTR sName;
    hr = iSMSObjectManager_->GetSMSObjectName(i, &sName);
    if (!FAILED(hr))
    {
      QString qstr((QChar*)sName, ::SysStringLen(sName));
      qDebug() << "found sms object name" << i << qstr;
    }
  }
}

void SMSObjectManager::start()
{
  titleMonitorTimer_ = new QTimer(this);
  QObject::connect(titleMonitorTimer_, SIGNAL(timeout()), this, SLOT(onTitleMonitorTimer()));
  titleMonitorTimer_->start(1000);
}

void SMSObjectManager::setMessage(int id, const QString message, int priority, int sender
                                  , const QTime &receiveTime)
{
  Q_UNUSED(priority);
  Q_UNUSED(sender);
  Q_UNUSED(receiveTime);

  if (iSMSObject_)
  {
    QString messageInfo = QString("prior = " + QString::number(priority)
                                  + "; sender = " + QString::number(sender)
                                  + "; time = " + receiveTime.toString("h:mm:ss"));

    BSTR bText = SysAllocString(reinterpret_cast<const OLECHAR*>(message.utf16()));
    BSTR bInfo = SysAllocString(reinterpret_cast<const OLECHAR*>(messageInfo.utf16()));

    HRESULT hr = iSMSObject_->SetMessage(bText, bInfo, 0, id);
    if (FAILED(hr))
    {
      qDebug() << "failed send message";
      //TODO error handling
    }
    else
    {
      qDebug() << "succes send message";
      messageSet();
    }

    SysFreeString(bText);
    SysFreeString(bInfo);
  }
  else
  {
    qWarning() << "error showing message: no sms object";
    //TODO error handling
  }
}

void SMSObjectManager::onTitleMonitorTimer()
{
  QSettings settings(QGuiApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
  QString smsObjectName = settings.value("Other/smsobjectname").toString();
  BSTR bSmsObjectName = SysAllocString(reinterpret_cast<const OLECHAR*>(smsObjectName.utf16()));

  IUnknownPtr iUnknown;
  HRESULT hr = iSMSObjectManager_->GetSMSObjectByName(bSmsObjectName, &iUnknown);
  if (FAILED(hr))
  {
    qDebug() << "failed to find" << smsObjectName;
    resetSMSObject();
    return;
  }

  if (iSMSObject_ == NULL || iSMSObject_ != iUnknown)
  {
    qDebug() << "update SMS object" << smsObjectName;
    iSMSObject_ = iUnknown;
  }

  SysFreeString(bSmsObjectName);

  titleCheck(iSMSObject_ != NULL);
}

void SMSObjectManager::resetSMSObject()
{
  qDebug() << "reset SMS object";
  if (iSMSObject_ != NULL)
  {
    iSMSObject_->Release();
    iSMSObject_ = NULL;
  }
}
