#ifndef SMSCALLBACK_H
#define SMSCALLBACK_H

#include <functional>

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
CComModule _Module;
#define _pAtlModule _Module;

class ATL_NO_VTABLE CSMSCallBack :
    public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<CSMSCallBack, &CLSID_SMSCallBack>,
    public IDispatchImpl<ISMSCallBack, &IID_ISMSCallBack, &LIBID_SMSObjMgrLib>
{
public:
  CSMSCallBack()
    : m_pUnkMarshaler(NULL)
  {
    qDebug() << "created sms callback";
  }

  DECLARE_REGISTRY_RESOURCEID(101/*IDR_SMSCALLBACK*/)

  DECLARE_NOT_AGGREGATABLE(CSMSCallBack)

  BEGIN_COM_MAP(CSMSCallBack)
      COM_INTERFACE_ENTRY(ISMSCallBack)
      COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
  END_COM_MAP()

  DECLARE_PROTECT_FINAL_CONSTRUCT()
  //Protects your object from being deleted if (during FinalConstruct)
  //the internal aggregated object increments the reference count then decrements the count to 0.
  DECLARE_GET_CONTROLLING_UNKNOWN()
  //Add this macro to your object if you get the compiler error message that GetControllingUnknown is undefined

  HRESULT FinalConstruct()
  {
    qDebug() << "final construct";
    return CoCreateFreeThreadedMarshaler(GetControllingUnknown(), &m_pUnkMarshaler.p);
    //IUnknown* GetControllingUnknown( );
    //The CoCreateFreeThreadedMarshaler function enables an object
    //to efficiently marshal interface pointers between threads in the same process.
  }

  void FinalRelease()
  {
    qDebug() << "final release";
    m_pUnkMarshaler.Release();
  }

  inline void setCallback(const std::function<void(long)>& func) { callback_ = func; };

  STDMETHOD(OnMessageShown)(long nMsgID)
  {
    qDebug() << "on message shown" << nMsgID;
    callback_(nMsgID);
    return S_OK;
  }

public:
  CComPtr<IUnknown> m_pUnkMarshaler;
  std::function<void(long)> callback_;
};

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(SMSCallBack), CSMSCallBack)
/*
You use the OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO macro to specify a class
that does not have an associated class object.
Often these are non-top-level objects in an object model.
Clients typically must call a method on a higher-level object
in the object hierarchy to obtain an instance of this class.
Because the specified class does not have an associated class object
, clients cannot create an instance by calling CoCreateInstance.

You use the OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO macro primarily for
noncreateable classes that need class-level initialization and uninitialization.
Occasionally, you might want to have a noncreateable class maintain Registry entries
, possibly persistent class configuration information and component categories.
*/
#endif // SMSCALLBACK_H
