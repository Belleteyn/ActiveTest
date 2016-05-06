#include "SMSObjectManager.h"

#include <QSettings>
#include <QGuiApplication>
#include <QTime>

#include <SMSCallback.h>

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
    qDebug() << "succes smsObjectManager";
    return true;
  }

  qDebug() << "failed smsObjectManager";
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

void SMSObjectManager::setMessage(int id, const QByteArray &message, int priority, int sender
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

    BSTR bText = SysAllocString(reinterpret_cast<const OLECHAR*>(QString::fromUtf8(message).utf16()));
    BSTR bInfo = SysAllocString(reinterpret_cast<const OLECHAR*>(messageInfo.utf16()));

    HRESULT hr = iSMSObject_->SetMessage(bText, bInfo, id % 7, id);
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
  SysFreeString(bSmsObjectName);
  if (FAILED(hr))
  {
    qDebug() << "failed to find" << smsObjectName;
    resetSMSObject();
    return;
  }

  if (iSMSObject_ == NULL || iSMSObject_ != iUnknown)
  {
    qDebug() << "update SMS object" << smsObjectName;
    updateSMSObject(iUnknown);
  }

  titleCheck(iSMSObject_ != NULL);
}

void SMSObjectManager::updateSMSObject(const IUnknownPtr &iUnknown)
{
  if (iSMSObject_ != NULL)
    resetSMSObject();

  if (iSMSCallBack_ != NULL)
    resetCallback();

  iSMSObject_ = iUnknown;
  if (iSMSObject_ == NULL)
  {
    qWarning() << "tried to set sms object to NULL";
    return;
  }

  LONG styleCount;
  iSMSObject_->GetStyleCount(&styleCount);
  qDebug() << styleCount << "styles";

  HRESULT hr = CComObject<CSMSCallBack>::CreateInstance
      (reinterpret_cast<CComObject<CSMSCallBack>**>(&iSMSCallBack_));
  if (FAILED(hr))
  {
    qDebug() << "failed create smsCallback";
    resetSMSObject();
    resetCallback();
    return;
  }

  iSMSCallBack_->AddRef();

  IUnknownPtr spIUnk = NULL;
  hr = iSMSCallBack_->QueryInterface(IID_IUnknown,(void**)&spIUnk);
  if (FAILED(hr))
  {
    qDebug() << "failed query smsCallback";
    resetSMSObject();
    resetCallback();
    return;
  }

  hr = iSMSObject_->Advise(spIUnk);
  if (FAILED(hr))
  {
    qDebug() << "failed to advise sms callback";
    resetSMSObject();
    resetCallback();
    return;
  }
  else
  {
    qDebug() << "WOW! Such advise! Much callback!";
  }

  iSMSCallBack_->setCallback([this](long id)
  {
    qDebug() << "done message" << id;
    messageDone();
  });
}

void SMSObjectManager::resetSMSObject()
{
  qDebug() << "reset SMS object";
  if (iSMSObject_ != NULL)
  {
    iSMSObject_->UnAdvise();
    iSMSObject_->Release();
    iSMSObject_ = NULL;
  }

  titleCheck(iSMSObject_ != NULL);
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
}
