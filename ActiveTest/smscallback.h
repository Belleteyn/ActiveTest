#ifndef SMSCALLBACK_H
#define SMSCALLBACK_H

#include "SMSObjMgr.tlh"

#include <QDebug>

class CSMSCallback : public ISMSCallBack
{
public:
  CSMSCallback() {}
  ~CSMSCallback() {}

  virtual HRESULT __stdcall OnMessageShown(long nMsgID)
  {
    Q_UNUSED(nMsgID);

    qDebug() << "OnMessageShown callback";

    return S_OK;
  }

  virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv)
  {
    *ppv = static_cast<ISMSCallBack*>(this);
    return S_OK;
  }
  // method for incrementing the reference count variable "m_cRef"
  virtual ULONG __stdcall AddRef() { return InterlockedIncrement(&m_cRef);}
  // method for decrementing the reference count variable "m_cRef"
  virtual ULONG __stdcall Release()
  {
    if(InterlockedDecrement(&m_cRef) == 0)
    {
      delete this ;
      return 0 ;
    }
    return m_cRef ;
  }

private:
  long m_cRef;

};

_COM_SMARTPTR_TYPEDEF(CSMSCallback, __uuidof(ISMSCallBack));



#endif // SMSCALLBACK_H
