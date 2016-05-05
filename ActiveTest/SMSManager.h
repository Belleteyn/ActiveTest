/****************************************************************************
**
** Namespace SMSManager generated by dumpcpp v5.6.0 using
** D:\develop\sdk\qt\win32\5.6\msvc2015\bin\dumpcpp.exe -compat -n SMSManager -o SMSManager {4892F679-C799-439A-828D-3C7592D4D201}
** from the type library C:\Program Files\ForwardT Software\FDTitle\SMSObjMgr.exe
**
****************************************************************************/

#ifndef QAX_DUMPCPP_SMSMANAGER_H
#define QAX_DUMPCPP_SMSMANAGER_H

// Define this symbol to __declspec(dllexport) or __declspec(dllimport)
#ifndef SMSMANAGER_EXPORT
#define SMSMANAGER_EXPORT
#endif

#include <qaxobject.h>
#include <qaxwidget.h>
#include <qdatetime.h>
#include <qpixmap.h>

#include <guiddef.h>

struct IDispatch;


// Referenced namespace

namespace SMSManager {

// forward declarations
    class ISMSCallBack;
    class ISMSCallBack;
    typedef GUID GUID;

// stub for vtable-only interface
class ISMSCallBack : public QAxObject { Q_DISABLE_COPY(ISMSCallBack) };

class SMSMANAGER_EXPORT ISMSObject : public QAxObject
{
public:
    ISMSObject(IDispatch *subobject = 0, QAxObject *parent = 0)
    : QAxObject((IUnknown*)subobject, parent)
    {
        internalRelease();
    }

    /*
    Method Advise

    method Advise

    */
    inline void Advise(IUnknown* pIUnk);

    /*
    Method GetStyleCount

    method GetStyleCount

    */
    inline int GetStyleCount();

    /*
    Method SetMessage

    method SetMessage

    */
    inline void SetMessage(const QString& Text, const QString& Nick, int StyleIndex, int MsgID);

    /*
    Method SetStyleMessage

    method SetStyleMessage

    */
    inline void SetStyleMessage(const QString& bstrNickStyle, const QString& bstrNick, const QString& bstrTextStyle, const QString& bstrText, int lMsgID, int bNewLine);

    /*
    Method TestMessage

    method TestMessage - without drawing

    */
    inline double TestMessage(const QString& bstrText, const QString& bstrNick, int lStyleIndex, int lMsgID);

    /*
    Method UnAdvise

    method UnAdvise

    */
    inline void UnAdvise();

// meta object functions
    static const QMetaObject staticMetaObject;
    virtual const QMetaObject *metaObject() const { return &staticMetaObject; }
    virtual void *qt_metacast(const char *);
};

class SMSMANAGER_EXPORT ISMSObjectCheck : public QAxObject
{
public:
    ISMSObjectCheck(IDispatch *subobject = 0, QAxObject *parent = 0)
    : QAxObject((IUnknown*)subobject, parent)
    {
        internalRelease();
    }

    /*
    Method CheckMessage

    CheckMessage - without drawing

    */
    inline void CheckMessage(const QString& bstrText, const QString& bstrNick, int lStyleIndex, double& pfWidthFullness, double& pfHeightFullness);

    /*
    Method CheckStyleMessage

    CheckStyleMessage - without drawing

    */
    inline void CheckStyleMessage(const QString& bstrNickStyle, const QString& bstrNick, const QString& bstrTextStyle, const QString& bstrText, int bNewLine, double& pfWidthFullness, double& pfHeightFullness);

// meta object functions
    static const QMetaObject staticMetaObject;
    virtual const QMetaObject *metaObject() const { return &staticMetaObject; }
    virtual void *qt_metacast(const char *);
};

class SMSMANAGER_EXPORT ISMSObjectsManager : public QAxObject
{
public:
    ISMSObjectsManager(IDispatch *subobject = 0, QAxObject *parent = 0)
    : QAxObject((IUnknown*)subobject, parent)
    {
        internalRelease();
    }

    /*
    Method GetSMSObjectByIndex

    method GetSMSObjectByIndex

    */
    inline IUnknown* GetSMSObjectByIndex(int Index);

    /*
    Method GetSMSObjectByName

    method GetSMSObjectByName

    */
    inline IUnknown* GetSMSObjectByName(const QString& Name);

    /*
    Method GetSMSObjectName

    method GetSMSObjectName

    */
    inline QString GetSMSObjectName(int Index);

    /*
    Method GetSMSObjectsCount

    method GetSMSObjectsCount

    */
    inline int GetSMSObjectsCount();

    /*
    Method SetMessage

    method SetMessage

    */
    inline void SetMessage(const QString& bstrSMSName, const QString& bstrText, const QString& bstrNick, int lStyleIndex, int lMsgID);

    /*
    Method SetStyleMessageTo

    method SetStyleMessageTo

    */
    inline void SetStyleMessageTo(const QString& bstrSMSName, const QString& bstrNickStyle, const QString& bstrNick, const QString& bstrTextStyle, const QString& bstrText, int lMsgID, int bNewLine);

// meta object functions
    static const QMetaObject staticMetaObject;
    virtual const QMetaObject *metaObject() const { return &staticMetaObject; }
    virtual void *qt_metacast(const char *);
};

class SMSMANAGER_EXPORT IRegistration : public QAxObject
{
public:
    IRegistration(IDispatch *subobject = 0, QAxObject *parent = 0)
    : QAxObject((IUnknown*)subobject, parent)
    {
        internalRelease();
    }

    /*
    Method Register

    method Register

    */
    inline void Register(IUnknown* pUnk, const QString& Name, GUID*& pGuid);

    /*
    Method RenameByGuid

    method RenameByGuid

    */
    inline void RenameByGuid(SMSManager::GUID guid, const QString& NewName);

    /*
    Method Reregister

    method Reregister

    */
    inline void Reregister(const QString& OldName, const QString& NewName);

    /*
    Method Unregister

    method Unregister

    */
    inline void Unregister(const QString& Name);

    /*
    Method UnregisterByGuid

    method UnregisterByGuid

    */
    inline void UnregisterByGuid(SMSManager::GUID guid);

// meta object functions
    static const QMetaObject staticMetaObject;
    virtual const QMetaObject *metaObject() const { return &staticMetaObject; }
    virtual void *qt_metacast(const char *);
};

// Actual coclasses
class SMSMANAGER_EXPORT SMSObjectsManager : public QAxObject
{
public:
    SMSObjectsManager(QObject *parent = 0)
    : QAxObject(parent)
    {
        setControl("{915f2249-d3ad-415d-8477-28b51f555cea}");
    }

    SMSObjectsManager(ISMSObjectsManager *iface)
    : QAxObject()
    {
        initializeFrom(iface);
        delete iface;
    }

    SMSObjectsManager(IRegistration *iface)
    : QAxObject()
    {
        initializeFrom(iface);
        delete iface;
    }

    SMSObjectsManager(ISMSObject *iface)
    : QAxObject()
    {
        initializeFrom(iface);
        delete iface;
    }

    SMSObjectsManager(ISMSObjectCheck *iface)
    : QAxObject()
    {
        initializeFrom(iface);
        delete iface;
    }

    /*
    Method Advise
    */
    inline void Advise(IUnknown* pIUnk);

    /*
    Method CheckMessage
    */
    inline void CheckMessage(const QString& bstrText, const QString& bstrNick, int lStyleIndex, double& pfWidthFullness, double& pfHeightFullness);

    /*
    Method CheckStyleMessage
    */
    inline void CheckStyleMessage(const QString& bstrNickStyle, const QString& bstrNick, const QString& bstrTextStyle, const QString& bstrText, int bNewLine, double& pfWidthFullness, double& pfHeightFullness);

    /*
    Method GetSMSObjectByIndex

    method GetSMSObjectByIndex

    */
    inline IUnknown* GetSMSObjectByIndex(int Index);

    /*
    Method GetSMSObjectByName

    method GetSMSObjectByName

    */
    inline IUnknown* GetSMSObjectByName(const QString& Name);

    /*
    Method GetSMSObjectName

    method GetSMSObjectName

    */
    inline QString GetSMSObjectName(int Index);

    /*
    Method GetSMSObjectsCount

    method GetSMSObjectsCount

    */
    inline int GetSMSObjectsCount();

    /*
    Method GetStyleCount
    */
    inline int GetStyleCount();

    /*
    Method Register
    */
    inline void Register(IUnknown* pUnk, const QString& Name, GUID*& pGuid);

    /*
    Method RenameByGuid
    */
    inline void RenameByGuid(SMSManager::GUID guid, const QString& NewName);

    /*
    Method Reregister
    */
    inline void Reregister(const QString& OldName, const QString& NewName);

    /*
    Method SetMessage

    method SetMessage

    */
    inline void SetMessage(const QString& bstrSMSName, const QString& bstrText, const QString& bstrNick, int lStyleIndex, int lMsgID);

    /*
    Method SetMessage

    method SetMessage

    */
    inline void SetMessage(const QString& Text, const QString& Nick, int StyleIndex, int MsgID);

    /*
    Method SetStyleMessage
    */
    inline void SetStyleMessage(const QString& bstrNickStyle, const QString& bstrNick, const QString& bstrTextStyle, const QString& bstrText, int lMsgID, int bNewLine);

    /*
    Method SetStyleMessageTo

    method SetStyleMessageTo

    */
    inline void SetStyleMessageTo(const QString& bstrSMSName, const QString& bstrNickStyle, const QString& bstrNick, const QString& bstrTextStyle, const QString& bstrText, int lMsgID, int bNewLine);

    /*
    Method TestMessage
    */
    inline double TestMessage(const QString& bstrText, const QString& bstrNick, int lStyleIndex, int lMsgID);

    /*
    Method UnAdvise
    */
    inline void UnAdvise();

    /*
    Method Unregister
    */
    inline void Unregister(const QString& Name);

    /*
    Method UnregisterByGuid
    */
    inline void UnregisterByGuid(SMSManager::GUID guid);

// meta object functions
    static const QMetaObject staticMetaObject;
    virtual const QMetaObject *metaObject() const { return &staticMetaObject; }
    virtual void *qt_metacast(const char *);
};

// member function implementation
#ifndef QAX_DUMPCPP_SMSMANAGER_NOINLINES
inline void ISMSObject::Advise(IUnknown* pIUnk)
{
    void *_a[] = {0, (void*)&pIUnk};
    qt_metacall(QMetaObject::InvokeMetaMethod, 8, _a);
}

inline int ISMSObject::GetStyleCount()
{
    int qax_result;
    void *_a[] = {(void*)&qax_result};
    qt_metacall(QMetaObject::InvokeMetaMethod, 9, _a);
    return qax_result;
}

inline void ISMSObject::SetMessage(const QString& Text, const QString& Nick, int StyleIndex, int MsgID)
{
    void *_a[] = {0, (void*)&Text, (void*)&Nick, (void*)&StyleIndex, (void*)&MsgID};
    qt_metacall(QMetaObject::InvokeMetaMethod, 10, _a);
}

inline void ISMSObject::SetStyleMessage(const QString& bstrNickStyle, const QString& bstrNick, const QString& bstrTextStyle, const QString& bstrText, int lMsgID, int bNewLine)
{
    void *_a[] = {0, (void*)&bstrNickStyle, (void*)&bstrNick, (void*)&bstrTextStyle, (void*)&bstrText, (void*)&lMsgID, (void*)&bNewLine};
    qt_metacall(QMetaObject::InvokeMetaMethod, 11, _a);
}

inline double ISMSObject::TestMessage(const QString& bstrText, const QString& bstrNick, int lStyleIndex, int lMsgID)
{
    double qax_result;
    void *_a[] = {(void*)&qax_result, (void*)&bstrText, (void*)&bstrNick, (void*)&lStyleIndex, (void*)&lMsgID};
    qt_metacall(QMetaObject::InvokeMetaMethod, 12, _a);
    return qax_result;
}

inline void ISMSObject::UnAdvise()
{
    void *_a[] = {0};
    qt_metacall(QMetaObject::InvokeMetaMethod, 13, _a);
}


inline void ISMSObjectCheck::CheckMessage(const QString& bstrText, const QString& bstrNick, int lStyleIndex, double& pfWidthFullness, double& pfHeightFullness)
{
    void *_a[] = {0, (void*)&bstrText, (void*)&bstrNick, (void*)&lStyleIndex, (void*)&pfWidthFullness, (void*)&pfHeightFullness};
    qt_metacall(QMetaObject::InvokeMetaMethod, 8, _a);
}

inline void ISMSObjectCheck::CheckStyleMessage(const QString& bstrNickStyle, const QString& bstrNick, const QString& bstrTextStyle, const QString& bstrText, int bNewLine, double& pfWidthFullness, double& pfHeightFullness)
{
    void *_a[] = {0, (void*)&bstrNickStyle, (void*)&bstrNick, (void*)&bstrTextStyle, (void*)&bstrText, (void*)&bNewLine, (void*)&pfWidthFullness, (void*)&pfHeightFullness};
    qt_metacall(QMetaObject::InvokeMetaMethod, 9, _a);
}


inline void SMSObjectsManager::Advise(IUnknown* pIUnk)
{
    void *_a[] = {0, (void*)&pIUnk};
    qt_metacall(QMetaObject::InvokeMetaMethod, 8, _a);
}

inline void SMSObjectsManager::CheckMessage(const QString& bstrText, const QString& bstrNick, int lStyleIndex, double& pfWidthFullness, double& pfHeightFullness)
{
    void *_a[] = {0, (void*)&bstrText, (void*)&bstrNick, (void*)&lStyleIndex, (void*)&pfWidthFullness, (void*)&pfHeightFullness};
    qt_metacall(QMetaObject::InvokeMetaMethod, 9, _a);
}

inline void SMSObjectsManager::CheckStyleMessage(const QString& bstrNickStyle, const QString& bstrNick, const QString& bstrTextStyle, const QString& bstrText, int bNewLine, double& pfWidthFullness, double& pfHeightFullness)
{
    void *_a[] = {0, (void*)&bstrNickStyle, (void*)&bstrNick, (void*)&bstrTextStyle, (void*)&bstrText, (void*)&bNewLine, (void*)&pfWidthFullness, (void*)&pfHeightFullness};
    qt_metacall(QMetaObject::InvokeMetaMethod, 10, _a);
}

inline IUnknown* SMSObjectsManager::GetSMSObjectByIndex(int Index)
{
    IUnknown* qax_result = 0;
    void *_a[] = {(void*)&qax_result, (void*)&Index};
    qt_metacall(QMetaObject::InvokeMetaMethod, 11, _a);
    return qax_result;
}

inline IUnknown* SMSObjectsManager::GetSMSObjectByName(const QString& Name)
{
    IUnknown* qax_result = 0;
    void *_a[] = {(void*)&qax_result, (void*)&Name};
    qt_metacall(QMetaObject::InvokeMetaMethod, 12, _a);
    return qax_result;
}

inline QString SMSObjectsManager::GetSMSObjectName(int Index)
{
    QString qax_result;
    void *_a[] = {(void*)&qax_result, (void*)&Index};
    qt_metacall(QMetaObject::InvokeMetaMethod, 13, _a);
    return qax_result;
}

inline int SMSObjectsManager::GetSMSObjectsCount()
{
    int qax_result;
    void *_a[] = {(void*)&qax_result};
    qt_metacall(QMetaObject::InvokeMetaMethod, 14, _a);
    return qax_result;
}

inline int SMSObjectsManager::GetStyleCount()
{
    int qax_result;
    void *_a[] = {(void*)&qax_result};
    qt_metacall(QMetaObject::InvokeMetaMethod, 15, _a);
    return qax_result;
}

inline void SMSObjectsManager::Register(IUnknown* pUnk, const QString& Name, GUID*& pGuid)
{
    void *_a[] = {0, (void*)&pUnk, (void*)&Name, (void*)&pGuid};
    qt_metacall(QMetaObject::InvokeMetaMethod, 16, _a);
}

inline void SMSObjectsManager::RenameByGuid(SMSManager::GUID guid, const QString& NewName)
{
    void *_a[] = {0, (void*)&guid, (void*)&NewName};
    qt_metacall(QMetaObject::InvokeMetaMethod, 17, _a);
}

inline void SMSObjectsManager::Reregister(const QString& OldName, const QString& NewName)
{
    void *_a[] = {0, (void*)&OldName, (void*)&NewName};
    qt_metacall(QMetaObject::InvokeMetaMethod, 18, _a);
}

inline void SMSObjectsManager::SetMessage(const QString& bstrSMSName, const QString& bstrText, const QString& bstrNick, int lStyleIndex, int lMsgID)
{
    void *_a[] = {0, (void*)&bstrSMSName, (void*)&bstrText, (void*)&bstrNick, (void*)&lStyleIndex, (void*)&lMsgID};
    qt_metacall(QMetaObject::InvokeMetaMethod, 19, _a);
}

inline void SMSObjectsManager::SetMessage(const QString& Text, const QString& Nick, int StyleIndex, int MsgID)
{
    void *_a[] = {0, (void*)&Text, (void*)&Nick, (void*)&StyleIndex, (void*)&MsgID};
    qt_metacall(QMetaObject::InvokeMetaMethod, 20, _a);
}

inline void SMSObjectsManager::SetStyleMessage(const QString& bstrNickStyle, const QString& bstrNick, const QString& bstrTextStyle, const QString& bstrText, int lMsgID, int bNewLine)
{
    void *_a[] = {0, (void*)&bstrNickStyle, (void*)&bstrNick, (void*)&bstrTextStyle, (void*)&bstrText, (void*)&lMsgID, (void*)&bNewLine};
    qt_metacall(QMetaObject::InvokeMetaMethod, 21, _a);
}

inline void SMSObjectsManager::SetStyleMessageTo(const QString& bstrSMSName, const QString& bstrNickStyle, const QString& bstrNick, const QString& bstrTextStyle, const QString& bstrText, int lMsgID, int bNewLine)
{
    void *_a[] = {0, (void*)&bstrSMSName, (void*)&bstrNickStyle, (void*)&bstrNick, (void*)&bstrTextStyle, (void*)&bstrText, (void*)&lMsgID, (void*)&bNewLine};
    qt_metacall(QMetaObject::InvokeMetaMethod, 22, _a);
}

inline double SMSObjectsManager::TestMessage(const QString& bstrText, const QString& bstrNick, int lStyleIndex, int lMsgID)
{
    double qax_result;
    void *_a[] = {(void*)&qax_result, (void*)&bstrText, (void*)&bstrNick, (void*)&lStyleIndex, (void*)&lMsgID};
    qt_metacall(QMetaObject::InvokeMetaMethod, 23, _a);
    return qax_result;
}

inline void SMSObjectsManager::UnAdvise()
{
    void *_a[] = {0};
    qt_metacall(QMetaObject::InvokeMetaMethod, 24, _a);
}

inline void SMSObjectsManager::Unregister(const QString& Name)
{
    void *_a[] = {0, (void*)&Name};
    qt_metacall(QMetaObject::InvokeMetaMethod, 25, _a);
}

inline void SMSObjectsManager::UnregisterByGuid(SMSManager::GUID guid)
{
    void *_a[] = {0, (void*)&guid};
    qt_metacall(QMetaObject::InvokeMetaMethod, 26, _a);
}


inline IUnknown* ISMSObjectsManager::GetSMSObjectByIndex(int Index)
{
    IUnknown* qax_result = 0;
    void *_a[] = {(void*)&qax_result, (void*)&Index};
    qt_metacall(QMetaObject::InvokeMetaMethod, 8, _a);
    return qax_result;
}

inline IUnknown* ISMSObjectsManager::GetSMSObjectByName(const QString& Name)
{
    IUnknown* qax_result = 0;
    void *_a[] = {(void*)&qax_result, (void*)&Name};
    qt_metacall(QMetaObject::InvokeMetaMethod, 9, _a);
    return qax_result;
}

inline QString ISMSObjectsManager::GetSMSObjectName(int Index)
{
    QString qax_result;
    void *_a[] = {(void*)&qax_result, (void*)&Index};
    qt_metacall(QMetaObject::InvokeMetaMethod, 10, _a);
    return qax_result;
}

inline int ISMSObjectsManager::GetSMSObjectsCount()
{
    int qax_result;
    void *_a[] = {(void*)&qax_result};
    qt_metacall(QMetaObject::InvokeMetaMethod, 11, _a);
    return qax_result;
}

inline void ISMSObjectsManager::SetMessage(const QString& bstrSMSName, const QString& bstrText, const QString& bstrNick, int lStyleIndex, int lMsgID)
{
    void *_a[] = {0, (void*)&bstrSMSName, (void*)&bstrText, (void*)&bstrNick, (void*)&lStyleIndex, (void*)&lMsgID};
    qt_metacall(QMetaObject::InvokeMetaMethod, 12, _a);
}

inline void ISMSObjectsManager::SetStyleMessageTo(const QString& bstrSMSName, const QString& bstrNickStyle, const QString& bstrNick, const QString& bstrTextStyle, const QString& bstrText, int lMsgID, int bNewLine)
{
    void *_a[] = {0, (void*)&bstrSMSName, (void*)&bstrNickStyle, (void*)&bstrNick, (void*)&bstrTextStyle, (void*)&bstrText, (void*)&lMsgID, (void*)&bNewLine};
    qt_metacall(QMetaObject::InvokeMetaMethod, 13, _a);
}


inline void IRegistration::Register(IUnknown* pUnk, const QString& Name, GUID*& pGuid)
{
    void *_a[] = {0, (void*)&pUnk, (void*)&Name, (void*)&pGuid};
    qt_metacall(QMetaObject::InvokeMetaMethod, 8, _a);
}

inline void IRegistration::RenameByGuid(SMSManager::GUID guid, const QString& NewName)
{
    void *_a[] = {0, (void*)&guid, (void*)&NewName};
    qt_metacall(QMetaObject::InvokeMetaMethod, 9, _a);
}

inline void IRegistration::Reregister(const QString& OldName, const QString& NewName)
{
    void *_a[] = {0, (void*)&OldName, (void*)&NewName};
    qt_metacall(QMetaObject::InvokeMetaMethod, 10, _a);
}

inline void IRegistration::Unregister(const QString& Name)
{
    void *_a[] = {0, (void*)&Name};
    qt_metacall(QMetaObject::InvokeMetaMethod, 11, _a);
}

inline void IRegistration::UnregisterByGuid(SMSManager::GUID guid)
{
    void *_a[] = {0, (void*)&guid};
    qt_metacall(QMetaObject::InvokeMetaMethod, 12, _a);
}



#endif

}

QT_BEGIN_NAMESPACE

namespace QtMetaTypePrivate {
template<>
struct QMetaTypeFunctionHelper<SMSManager::ISMSObject, /* Accepted */ true> {
    static void Destruct(void *t)
    {
        Q_UNUSED(t)
        static_cast<SMSManager::ISMSObject*>(t)->SMSManager::ISMSObject::~ISMSObject();
    }
    static void *Construct(void *where, const void *t)
    {
        Q_ASSERT(!t);
        Q_UNUSED(t)
        return new (where) SMSManager::ISMSObject;
    }
#ifndef QT_NO_DATASTREAM
    static void Save(QDataStream &stream, const void *t) { stream << *static_cast<const SMSManager::ISMSObject*>(t); }
    static void Load(QDataStream &stream, void *t) { stream >> *static_cast<SMSManager::ISMSObject*>(t); }
#endif // QT_NO_DATASTREAM
};

template<>
struct QMetaTypeFunctionHelper<SMSManager::ISMSObjectCheck, /* Accepted */ true> {
    static void Destruct(void *t)
    {
        Q_UNUSED(t)
        static_cast<SMSManager::ISMSObjectCheck*>(t)->SMSManager::ISMSObjectCheck::~ISMSObjectCheck();
    }
    static void *Construct(void *where, const void *t)
    {
        Q_ASSERT(!t);
        Q_UNUSED(t)
        return new (where) SMSManager::ISMSObjectCheck;
    }
#ifndef QT_NO_DATASTREAM
    static void Save(QDataStream &stream, const void *t) { stream << *static_cast<const SMSManager::ISMSObjectCheck*>(t); }
    static void Load(QDataStream &stream, void *t) { stream >> *static_cast<SMSManager::ISMSObjectCheck*>(t); }
#endif // QT_NO_DATASTREAM
};

template<>
struct QMetaTypeFunctionHelper<SMSManager::SMSObjectsManager, /* Accepted */ true> {
    static void Destruct(void *t)
    {
        Q_UNUSED(t)
        static_cast<SMSManager::SMSObjectsManager*>(t)->SMSManager::SMSObjectsManager::~SMSObjectsManager();
    }
    static void *Construct(void *where, const void *t)
    {
        Q_ASSERT(!t);
        Q_UNUSED(t)
        return new (where) SMSManager::SMSObjectsManager;
    }
#ifndef QT_NO_DATASTREAM
    static void Save(QDataStream &stream, const void *t) { stream << *static_cast<const SMSManager::SMSObjectsManager*>(t); }
    static void Load(QDataStream &stream, void *t) { stream >> *static_cast<SMSManager::SMSObjectsManager*>(t); }
#endif // QT_NO_DATASTREAM
};

template<>
struct QMetaTypeFunctionHelper<SMSManager::ISMSObjectsManager, /* Accepted */ true> {
    static void Destruct(void *t)
    {
        Q_UNUSED(t)
        static_cast<SMSManager::ISMSObjectsManager*>(t)->SMSManager::ISMSObjectsManager::~ISMSObjectsManager();
    }
    static void *Construct(void *where, const void *t)
    {
        Q_ASSERT(!t);
        Q_UNUSED(t)
        return new (where) SMSManager::ISMSObjectsManager;
    }
#ifndef QT_NO_DATASTREAM
    static void Save(QDataStream &stream, const void *t) { stream << *static_cast<const SMSManager::ISMSObjectsManager*>(t); }
    static void Load(QDataStream &stream, void *t) { stream >> *static_cast<SMSManager::ISMSObjectsManager*>(t); }
#endif // QT_NO_DATASTREAM
};

template<>
struct QMetaTypeFunctionHelper<SMSManager::IRegistration, /* Accepted */ true> {
    static void Destruct(void *t)
    {
        Q_UNUSED(t)
        static_cast<SMSManager::IRegistration*>(t)->SMSManager::IRegistration::~IRegistration();
    }
    static void *Construct(void *where, const void *t)
    {
        Q_ASSERT(!t);
        Q_UNUSED(t)
        return new (where) SMSManager::IRegistration;
    }
#ifndef QT_NO_DATASTREAM
    static void Save(QDataStream &stream, const void *t) { stream << *static_cast<const SMSManager::IRegistration*>(t); }
    static void Load(QDataStream &stream, void *t) { stream >> *static_cast<SMSManager::IRegistration*>(t); }
#endif // QT_NO_DATASTREAM
};

} // namespace QtMetaTypePrivate
QT_END_NAMESPACE

#endif

