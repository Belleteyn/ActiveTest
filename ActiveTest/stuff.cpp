#include "stuff.h"

#include <QDebug>
#include <QUuid>

#include "smscallback.h"
Stuff::Stuff(QObject *parent) : QObject(parent)
{

}

void Stuff::doSome()
{
  ISMSObjectsManagerPtr smsObjectManager = NULL;

  HRESULT hr = smsObjectManager.CreateInstance(CLSID_SMSObjectsManager);

  if (FAILED(hr))
  {
      qDebug() << "failed smsObjectManager";
  }
  else
  {
      qDebug() << "succes smsObjectManager";
  }

  IUnknown* pIUnknown=static_cast<ISMSCallBack*>(new CSMSCallback);
  ISMSCallBack* smsCallback = NULL;
  hr = pIUnknown->QueryInterface(IID_ISMSCallBack,(void**)&smsCallback);
  if (FAILED(hr))
  {
      qDebug() << "failed smsCallback";
  }
  else
  {
      qDebug() << "succes smsCallback";
      smsCallback->AddRef();
  }

  int nObjectsCount = 0;
  hr = smsObjectManager->GetSMSObjectsCount(&nObjectsCount);
  if (FAILED(hr))
  {
    qDebug() << "failed GetSMSObjectsCount";
  }
  else
  {
    qDebug() << "succes GetSMSObjectsCount = " << nObjectsCount;

    for (int i = 0; i < nObjectsCount; i++)
    {
      BSTR sName;
      hr = smsObjectManager->GetSMSObjectName(i, &sName);
      if (!FAILED(hr))
      {
        qDebug() << "found sms object name" << i << QString::fromWCharArray(sName);

        ISMSObjectPtr smsObject;
        IUnknown* instance;
        hr = smsObjectManager->GetSMSObjectByName(sName, (IUnknown**)&instance);

        smsObject = instance;

        if (!smsObject)
        {
          qDebug() << "failed GetSMSObjectByName";
        }
        else
        {
          qDebug() << "succes GetSMSObjectByName";

          //smsObject->Advise(smsCallback);

          QString qName("ololo-boy:");
          BSTR name = SysAllocString(reinterpret_cast<const OLECHAR*>(qName.utf16()));

          QString qMessage = "fasdf";
          BSTR message = SysAllocString(reinterpret_cast<const OLECHAR*>(qMessage.utf16()));

          hr = smsObject->SetMessage(message, name, 5, 3);
          if (FAILED(hr))
          {
            qDebug() << "failed send message";
          }
          else
          {
            qDebug() << "succes send message";
          }

          SysFreeString(name);
          SysFreeString(message);
        }
      }
      else
      {
        qDebug() << "no sms objects";
      }
    }
  }

  if (smsCallback != NULL)
  {
    smsCallback->Release();
  }
}
