#include "SMSObjectManager.h"

#include <QSettings>
#include <QGuiApplication>
#include <QTime>

#include <SMSCallback.h>

#include <LogHelper.h>

SMSObjectManager::SMSObjectManager(QObject *parent)
  : QObject(parent)
  , iSMSObjectManager_(NULL)
  , iSMSCallBack_(NULL)
  , iSMSVipCallBack_(NULL)
  , iSMSObject_(NULL)
  , iSMSVipObject_(NULL)
  , titleMonitorTimer_(nullptr)
{
  QSettings settings(QGuiApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
  vipPriority_ = settings.value("Other/vipPriority", 1).toInt();
}

SMSObjectManager::~SMSObjectManager()
{
  resetSMSObject();
  resetCallback();

  if (iSMSObjectManager_)
  {
    iSMSObjectManager_->Release();
    iSMSObjectManager_ = NULL;
  }
}

bool SMSObjectManager::init()
{
  if (!initSmsObjectManager())
    return false;

  showSmsObjects();
  return true;
}

bool SMSObjectManager::initSmsObjectManager()
{
  HRESULT hr = iSMSObjectManager_.CreateInstance(CLSID_SMSObjectsManager);
  if (!FAILED(hr))
  {
    Loggers::sms->debug() << "successful creation of smsObjectManager";
    return true;
  }

  Loggers::sms->error() << "failed smsObjectManager";
  return false;
}

void SMSObjectManager::showSmsObjects()
{
  INT nObjectsCount = 0;
  HRESULT hr = iSMSObjectManager_->GetSMSObjectsCount(&nObjectsCount);
  if (FAILED(hr))
  {
    Loggers::sms->critical() << "failed GetSMSObjectsCount";
  }

  for (INT i = 0; i < nObjectsCount; i++)
  {
    BSTR sName;
    hr = iSMSObjectManager_->GetSMSObjectName(i, &sName);
    if (!FAILED(hr))
    {
      QString qstr((QChar*)sName, ::SysStringLen(sName));
      Loggers::sms->debug() << "found sms object " << i << ": " << qstr;
    }
  }
}

void SMSObjectManager::start()
{
  titleMonitorTimer_ = new QTimer(this);
  QObject::connect(titleMonitorTimer_, SIGNAL(timeout()), this, SLOT(onTitleMonitorTimer()));
  titleMonitorTimer_->start(1000);
}

bool SMSObjectManager::testMessage(long id, const QByteArray &message, long priority)
{
  if (priority == vipPriority_)
  {
    return testMessage(iSMSVipObject_, id, message, priority);
  }
  else
  {
    return testMessage(iSMSObject_, id, message, priority);
  }
}

bool SMSObjectManager::testMessage(const ISMSObjectPtr &smsObj, long id, const QByteArray &message, long priority)
{
  if (smsObj)
  {
    BSTR bText = SysAllocString(reinterpret_cast<const OLECHAR*>(QString::fromUtf8(message).utf16()));
    float testResult;

    HRESULT hr = smsObj->TestMessage(bText, NULL, priority, id, &testResult);
    SysFreeString(bText);

    if (FAILED(hr))
    {
      Loggers::sms->warn() << "failed testing message " << id;
      return false;
    }
    else
    {
      Loggers::sms->debug() << "succes testing message " << id;
      return (testResult <= 1.0f);
    }
  }
  else
  {
    Loggers::sms->warn() << "error testing message: no sms object";
    //that message will be shown after iSMSObject was fixed. do nothing.
    return false;
    }
}

void SMSObjectManager::setMessage(long id, const QByteArray &message, long priority)
{
  if (priority == vipPriority_)
  {
    setMessage(iSMSVipObject_, id, message, priority);
  }
  else
  {
    setMessage(iSMSObject_, id, message, priority);
  }
}

void SMSObjectManager::setMessage(const ISMSObjectPtr &smsObj, long id, const QByteArray &message, long priority)
{
  if (smsObj)
  {
    //QString messageInfo = QString("id = " + QString::number(id));

    BSTR bText = SysAllocString(reinterpret_cast<const OLECHAR*>(QString::fromUtf8(message).utf16()));
    //BSTR bInfo = SysAllocString(reinterpret_cast<const OLECHAR*>(messageInfo.utf16()));

    HRESULT hr = smsObj->SetMessage(bText, NULL, priority, id);
    SysFreeString(bText);
    //SysFreeString(bInfo);

    if (FAILED(hr))
    {
      Loggers::sms->debug() << "failed set message " << id;
      messageFailed(id);
    }
    else
    {
      Loggers::sms->debug() << "succes set message " << id;
      messageSet(id);
    }
  }
  else
  {
    Loggers::sms->warn() << "error showing message: no sms object";
    //that message will be shown after iSMSObject was fixed. do nothing.
  }
}

void SMSObjectManager::onTitleMonitorTimer()
{
  QSettings settings(QGuiApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
  QString smsObjectName = settings.value("Other/smsobjectname").toString();
  QString vipSmsObjectName = settings.value("Other/vipSmsobjectname").toString();
  BSTR bSmsObjectName = SysAllocString(reinterpret_cast<const OLECHAR*>(smsObjectName.utf16()));
  BSTR bVipSmsObjectName = SysAllocString(reinterpret_cast<const OLECHAR*>(vipSmsObjectName.utf16()));

  //======SMS========
  IUnknownPtr iUnknown;
  HRESULT hr = iSMSObjectManager_->GetSMSObjectByName(bSmsObjectName, &iUnknown);
  SysFreeString(bSmsObjectName);
  if (FAILED(hr))
  {
    Loggers::sms->debug() << "failed to find " << smsObjectName;
    resetSMSObject();
    return;
  }

  if (iSMSObject_ == NULL || iSMSObject_ != iUnknown)
  {
    Loggers::sms->debug() << "update SMS object " << smsObjectName;
    updateSMSObject(iSMSObject_, iUnknown, iSMSCallBack_);
  }

  //======VIP SMS========
  hr = iSMSObjectManager_->GetSMSObjectByName(bVipSmsObjectName, &iUnknown);
  SysFreeString(bVipSmsObjectName);
  if (FAILED(hr))
  {
    Loggers::sms->debug() << "failed to find " << vipSmsObjectName;
    resetSMSObject();
    return;
  }

  if (iSMSVipObject_ == NULL || iSMSVipObject_ != iUnknown)
  {
    Loggers::sms->debug() << "update SMS object " << smsObjectName;
    updateSMSObject(iSMSVipObject_, iUnknown, iSMSVipCallBack_);
  }

  titleCheck(iSMSObject_ != NULL && iSMSVipObject_ != NULL);
}

void SMSObjectManager::updateSMSObject(ISMSObjectPtr& smsObj, const IUnknownPtr &iUnknown, CSMSCallBack* callback)
{
  if (smsObj != NULL)
    resetSMSObject();

  if (callback != NULL)
    resetCallback();

  smsObj = iUnknown;
  if (smsObj == NULL)
  {
    Loggers::sms->warn() << "tried to set sms object to NULL";
    return;
  }

  LONG styleCount;
  smsObj->GetStyleCount(&styleCount);
  Loggers::sms->debug() << "sms object holds " << styleCount << " styles";

  HRESULT hr = CComObject<CSMSCallBack>::CreateInstance
      (reinterpret_cast<CComObject<CSMSCallBack>**>(&callback));

  if (FAILED(hr))
  {
    Loggers::sms->critical() << "failed create smsCallback";
    resetSMSObject();
    resetCallback();
    return;
  }

  callback->AddRef();

  IUnknownPtr spIUnk = NULL;
  hr = callback->QueryInterface(IID_IUnknown,(void**)&spIUnk);
  if (FAILED(hr))
  {
    Loggers::sms->critical() << "failed query smsCallback";
    resetSMSObject();
    resetCallback();
    return;
  }

  hr = smsObj->Advise(spIUnk);
  if (FAILED(hr))
  {
    Loggers::sms->critical() << "failed to advise sms callback";
    resetSMSObject();
    resetCallback();
    return;
  }
  else
  {
    Loggers::sms->debug() << "Callback successfully created";
  }

  callback->setCallback([this](long id)
  {
    Loggers::sms->debug() << "done message " << id;
    messageDone(id);
  });
}

void SMSObjectManager::resetSMSObject()
{
  Loggers::sms->debug() << "reset SMS object";
  if (iSMSObject_ != NULL)
  {
    iSMSObject_->UnAdvise();
    iSMSObject_->Release();
    iSMSObject_ = NULL;
  }

  if (iSMSVipObject_ != NULL)
  {
    iSMSVipObject_->UnAdvise();
    iSMSVipObject_->Release();
    iSMSVipObject_ = NULL;
  }

  titleCheck(false);
  //reset server confirmation
  //maybe it is not necessary
}

void SMSObjectManager::resetCallback()
{
  if (iSMSCallBack_ != NULL)
  {
    iSMSCallBack_->Release();
    iSMSCallBack_ = NULL;
  }

  if (iSMSVipCallBack_ != NULL)
  {
    iSMSVipCallBack_->Release();
    iSMSVipCallBack_ = NULL;
  }
}
