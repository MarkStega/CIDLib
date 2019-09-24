// ----------------------------------------------------------------------------
//  FILE: CIDOrbUC_CoreAdminClientProxy.hpp
//  DATE: Tue, Sep 24 17:51:15 2019 -0400
//    ID: 2117FF72EE8BD81D-F320AE6BEB1B2F0C
//
//  This file was generated by the Charmed Quark CIDIDL compiler. Do not make
//  changes by hand, because they will be lost if the file is regenerated.
// ----------------------------------------------------------------------------
#pragma once

#pragma CIDLIB_PACK(CIDLIBPACK)

class CIDORBUCEXP TCIDCoreAdminClientProxy : public TOrbClientBase
{
    public :
        // --------------------------------------------------------------------
        // Public, static data
        // --------------------------------------------------------------------
        static const TString strInterfaceId;

        // --------------------------------------------------------------------
        // Public Constructors and Destructor
        // --------------------------------------------------------------------
        TCIDCoreAdminClientProxy();
        TCIDCoreAdminClientProxy(const TOrbObjId& ooidSrc, const TString& strNSBinding);
        TCIDCoreAdminClientProxy(const TCIDCoreAdminClientProxy&) = delete;
        ~TCIDCoreAdminClientProxy();

        // --------------------------------------------------------------------
        // Public operators
        // --------------------------------------------------------------------
        TCIDCoreAdminClientProxy& operator=(const TCIDCoreAdminClientProxy&) = delete;

        // --------------------------------------------------------------------
        // Public, inherited methods
        // --------------------------------------------------------------------
        tCIDLib::TVoid SetObjId
        (
            const   TOrbObjId& ooidToSet
            , const TString& strNSBinding
            , const tCIDLib::TBoolean bCheck = kCIDLib::True
        )   override;

        // --------------------------------------------------------------------
        // Public, non-virtual methods
        // --------------------------------------------------------------------
        tCIDLib::TVoid AdminStop();

        tCIDLib::TBoolean bPollValuesInScope
        (
            const TString& strParScope
            , const tCIDLib::TBoolean bDirectOnly
            , const tCIDLib::TCard8 c8QueryStamp
            , tCIDLib::TCard8& c8PollStamp
            , TFundVector<tCIDLib::TCard4>& fcolIds
            , TFundVector<tCIDLib::TCard8>& fcolValues
        );

        tCIDLib::TCard4 c4QueryStats
        (
            const TString& strParScope
            , TVector<TStatsCacheItemInfo>& colItems
            , const tCIDLib::TBoolean bDirectOnly
            , tCIDLib::TCard8& c8Stamp
        );

        tCIDLib::TVoid QueryBasicInfo
        (
            TString& strHostName
            , TString& strProcName
            , tCIDLib::TCard4& c4CPUCount
            , TString& strOSInfo
            , tCIDLib::TCard8& c8StartTime
        );

        tCIDLib::TVoid SetModLogLevel
        (
            const TString& strModule
            , const TString& strLevel
        );

        tCIDLib::TVoid SetModLogTrace
        (
            const TString& strModule
            , const tCIDLib::TBoolean bState
        );

    protected :
        // --------------------------------------------------------------------
        // Declare our friends
        // --------------------------------------------------------------------
        friend class TFacCIDOrb;

    private :
        // --------------------------------------------------------------------
        // Magic macros
        // --------------------------------------------------------------------
        RTTIDefs(TCIDCoreAdminClientProxy,TOrbClientBase)
};

#pragma CIDLIB_POPPACK

inline tCIDLib::TVoid
TCIDCoreAdminClientProxy::SetObjId(const  TOrbObjId& ooidToSet
              , const TString& strNSBinding
              , const tCIDLib::TBoolean bCheck)
{
    TParent::SetObjId(ooidToSet, strNSBinding, bCheck);
}

