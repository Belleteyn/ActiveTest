#ifndef SMSCALLBACK_H
#define SMSCALLBACK_H

#include <atlbase.h>
#include <atlcom.h>
#include "SMSObjMgr.tlh"

extern "C"{

EXTERN_C const CLSID CLSID_SMSCallBack;

class DECLSPEC_UUID("D2485CC4-A286-45AC-8AEC-2504F873B382")
SMSCallBack;

#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_SMSChatClientLib,0xb6d8e162,0x192b,0x4fc8,0x8e,0xb3,0x51,0x0a,0xad,0xd7,0xb0,0x16);


MIDL_DEFINE_GUID(CLSID, CLSID_SMSCallBack,0xD2485CC4,0xA286,0x45AC,0x8A,0xEC,0x25,0x04,0xF8,0x73,0xB3,0x82);

#undef MIDL_DEFINE_GUID
}

typedef class SMSCallBack SMSCallBack;

class CSMSCallBack : public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<CSMSCallBack, &CLSID_SMSCallBack>,
    public IDispatchImpl<ISMSCallBack, &IID_ISMSCallBack, &LIBID_SMSObjMgrLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
  CSMSCallBack()
    : m_pUnkMarshaler(NULL)
  {}

  DECLARE_REGISTRY_RESOURCEID(101/*IDR_SMSCALLBACK*/)
  DECLARE_NOT_AGGREGATABLE(CSMSCallBack)

  BEGIN_COM_MAP(CSMSCallBack)
      COM_INTERFACE_ENTRY(ISMSCallBack)
      COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
  END_COM_MAP()

  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

  HRESULT FinalConstruct()
  {
    return CoCreateFreeThreadedMarshaler(GetControllingUnknown(), &m_pUnkMarshaler.p);
  }

  void FinalRelease()
  {
    m_pUnkMarshaler.Release();
  }

  STDMETHOD(OnMessageShown)(long nMsgID)
  {
    Q_UNUSED(nMsgID);
    return S_OK;
  }

public:
  CComPtr<IUnknown> m_pUnkMarshaler;
};

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(SMSCallBack), CSMSCallBack)

#endif // SMSCALLBACK_H
