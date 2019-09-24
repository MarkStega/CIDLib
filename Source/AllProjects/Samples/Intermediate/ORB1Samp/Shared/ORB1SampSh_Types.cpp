// ----------------------------------------------------------------------------
//  FILE: ORB1SampSh_Types.cpp
//  DATE: Tue, Sep 24 17:51:16 2019 -0400
//
//  This file was generated by the Charmed Quark CIDIDL compiler. Do not make
//  changes by hand, because they will be lost if the file is regenerated.
// ----------------------------------------------------------------------------

#include "ORB1SampSh_.hpp"



static TEnumMap::TEnumValItem aeitemValues_EStatus[4] = 
{
    {  tCIDLib::TInt4(tORB1SampSh::EStatus::Divorced), 0, 0,  { L"", L"", L"", L"Divorced", L"EStatus::Divorced", L"" } }
  , {  tCIDLib::TInt4(tORB1SampSh::EStatus::Married), 0, 0,  { L"", L"", L"", L"Married", L"EStatus::Married", L"" } }
  , {  tCIDLib::TInt4(tORB1SampSh::EStatus::Single), 0, 0,  { L"", L"", L"", L"Single", L"EStatus::Single", L"" } }
  , {  tCIDLib::TInt4(tORB1SampSh::EStatus::Widowed), 0, 0,  { L"", L"", L"", L"Widowed", L"EStatus::Widowed", L"" } }

};

static TEnumMap emapEStatus
(
     L"EStatus"
     , 4
     , kCIDLib::False
     , aeitemValues_EStatus
     , nullptr
     , tCIDLib::TCard4(tORB1SampSh::EStatus::Count)
);

const TString& tORB1SampSh::strXlatEStatus(const tORB1SampSh::EStatus eVal, const tCIDLib::TBoolean bThrowIfNot)
{
    return emapEStatus.strMapEnumVal(tCIDLib::TCard4(eVal), TEnumMap::ETextVals::BaseName, bThrowIfNot);
}

tORB1SampSh::EStatus tORB1SampSh::eXlatEStatus(const TString& strVal, const tCIDLib::TBoolean bThrowIfNot)
{
    return tORB1SampSh::EStatus(emapEStatus.i4MapEnumText(strVal, TEnumMap::ETextVals::BaseName, bThrowIfNot));
}

TTextOutStream& operator<<(TTextOutStream& strmTar, const tORB1SampSh::EStatus eVal)
{
    strmTar << emapEStatus.strMapEnumVal(tCIDLib::TCard4(eVal), TEnumMap::ETextVals::BaseName, kCIDLib::False);
    return strmTar;
}
TBinOutStream& operator<<(TBinOutStream& strmTar, const tORB1SampSh::EStatus eVal)
{
    strmTar.WriteEnum(tCIDLib::TCard4(eVal));
    return strmTar;
}
TBinInStream& operator>>(TBinInStream& strmSrc, tORB1SampSh::EStatus& eVal)
{
    eVal = tORB1SampSh::EStatus(strmSrc.c4ReadEnum());
    return strmSrc;
}
tCIDLib::TVoid TBinInStream_ReadArray(TBinInStream& strmSrc, tORB1SampSh::EStatus* const aeList, const tCIDLib::TCard4 c4Count)
{
    tCIDLib::TCard4 c4Cur;
    for (tCIDLib::TCard4 c4Index = 0; c4Index < c4Count; c4Index++)
    {
        strmSrc >> c4Cur;
        aeList[c4Index] = tORB1SampSh::EStatus(c4Cur);
    }
}
tCIDLib::TVoid TBinOutStream_WriteArray(TBinOutStream& strmTar, const tORB1SampSh::EStatus* const aeList, const tCIDLib::TCard4 c4Count)
{
    for (tCIDLib::TCard4 c4Index = 0; c4Index < c4Count; c4Index++)
        strmTar << tCIDLib::TCard4(aeList[c4Index]);
}
tCIDLib::TBoolean tORB1SampSh::bIsValidEnum(const tORB1SampSh::EStatus eVal)
{
    return emapEStatus.bIsValidEnum(tCIDLib::TCard4(eVal));

}

