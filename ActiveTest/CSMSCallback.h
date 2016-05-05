#ifndef SMSCALLBACK_H
#define SMSCALLBACK_H

#include <atlbase.h>
#include <atlcom.h>
#include "SMSObjMgr.tlh"

class CSMSCallback
    : public IDispatchImpl<ISMSCallBack, &IID_ISMSCallBack, &LIBID_SMSObjMgrLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
  CSMSCallback() {}
  ~CSMSCallback() {}

  STDMETHOD(OnMessageShown)(long nMsgID)
  {
    Q_UNUSED(nMsgID);
    return S_OK;
  }
};

_COM_SMARTPTR_TYPEDEF(CSMSCallback, __uuidof(ISMSCallBack));

#endif // SMSCALLBACK_H

