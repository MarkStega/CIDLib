// ----------------------------------------------------------------------------
//  FILE: CIDOrbUC_RemBrowserServerBase.hpp
//  DATE: Tue, Sep 24 17:51:15 2019 -0400
//    ID: 9719B1449E1B5AC2-5A871C3214D2F876
//
//  This file was generated by the Charmed Quark CIDIDL compiler. Do not make
//  changes by hand, because they will be lost if the file is regenerated.
// ----------------------------------------------------------------------------
#pragma once

#pragma CIDLIB_PACK(CIDLIBPACK)

class CIDORBUCEXP TRemBrowserServerBase : public TOrbServerBase
{
    public :
        // --------------------------------------------------------------------
        // Public, static data
        // --------------------------------------------------------------------
        static const TString strInterfaceId;
        // --------------------------------------------------------------------
        // Constructors and destructor
        // --------------------------------------------------------------------
        TRemBrowserServerBase(const TRemBrowserServerBase&) = delete;
        ~TRemBrowserServerBase();

        // --------------------------------------------------------------------
        // Public operators
        // --------------------------------------------------------------------
        TRemBrowserServerBase& operator=(const TRemBrowserServerBase&) = delete;

        // --------------------------------------------------------------------
        // Public, pure virtual methods
        // --------------------------------------------------------------------
        virtual tCIDLib::TBoolean bFileExists
        (
            const TString& strToCheck
        ) = 0;

        virtual tCIDLib::TBoolean bFind
        (
            const TString& strPath
            , const TString& strItem
            , TFindBuf& fndbToFill
        ) = 0;

        virtual tCIDLib::TBoolean bFind2
        (
            const TString& strPath
            , TFindBuf& fndbToFill
        ) = 0;

        virtual tCIDLib::TBoolean bIsDirectory
        (
            const TString& strToCheck
        ) = 0;

        virtual tCIDLib::TCard4 c4QueryVolumes
        (
            TKeyedHashSet<TVolumeInfo, TString, TStringKeyOps>& colVols
            , TKeyedHashSet<TVolFailureInfo, TString, TStringKeyOps>& colFails
        ) = 0;

        virtual tCIDLib::TVoid ReadFile
        (
            const TString& strToRead
            , tCIDLib::TCard4& c4BufSz_mbufToFill
            , THeapBuf& mbufToFill
        ) = 0;

        virtual tCIDLib::TVoid LoadFiles
        (
            const TString& strPath
            , const TString& strWildcard
            , TVector<TFindBuf>& colSubDirs
            , TVector<TFindBuf>& colFiles
        ) = 0;

        virtual tCIDLib::TVoid MakeDirectory
        (
            const TString& strBasePath
            , const TString& strNewDir
        ) = 0;

        virtual tCIDLib::TVoid QueryHostName
        (
            TString& strToFill
        ) = 0;

        virtual tCIDLib::TVoid RemoveSelected
        (
            const TString& strToRemove
        ) = 0;

        virtual tCIDLib::TVoid Rename
        (
            const TString& strOrgName
            , const TString& strNewName
            , const tCIDLib::TBoolean bIsScope
        ) = 0;

    protected :
        // --------------------------------------------------------------------
        // Hidden constructors
        // --------------------------------------------------------------------
        TRemBrowserServerBase();
        TRemBrowserServerBase(const TOrbObjId& ooidThis);

        // --------------------------------------------------------------------
        // Protected, inherited methods
        // --------------------------------------------------------------------
        tCIDLib::TVoid Dispatch
        (
            const  TString&      strMethodName
            ,      TOrbCmd&      orbcToDispatch
        )   override;

    private :
        // --------------------------------------------------------------------
        // Magic macros
        // --------------------------------------------------------------------
        RTTIDefs(TRemBrowserServerBase,TOrbServerBase)
};

#pragma CIDLIB_POPPACK

