//
// FILE NAME: CIDKernel_SafeCounter.hpp
//
// AUTHOR: Dean Roddey
//
// CREATED: 11/11/1996
//
// COPYRIGHT: Charmed Quark Systems, Ltd @ 2019
//
//  This software is copyrighted by 'Charmed Quark Systems, Ltd' and
//  the author (Dean Roddey.) It is licensed under the MIT Open Source
//  license:
//
//  https://opensource.org/licenses/MIT
//
// DESCRIPTION:
//
//  This is the header for the CIDKernel_SafeCounter.Cpp file, which
//  implements some simple, atomic counters (i.e. thread safe counters.)
//  There is one version for TInt4 values and one for TCard4 values.
//
//
// CAVEATS/GOTCHAS:
//
// LOG:
//
//  $_CIDLib_Log_$
//
#pragma once


#pragma CIDLIB_PACK(CIDLIBPACK)

// ---------------------------------------------------------------------------
//   CLASS: TKrnlSafeCard4Counter
//  PREFIX: scnt
// ---------------------------------------------------------------------------
class KRNLEXPORT TKrnlSafeCard4Counter
{
    public  :
        // -------------------------------------------------------------------
        //  Constructors and Destructor
        // -------------------------------------------------------------------
        TKrnlSafeCard4Counter();

        TKrnlSafeCard4Counter
        (
            const   tCIDLib::TCard4         c4InitVal
        );

        TKrnlSafeCard4Counter(const TKrnlSafeCard4Counter&) = delete;

        ~TKrnlSafeCard4Counter();


        // -------------------------------------------------------------------
        //  Public operators
        // -------------------------------------------------------------------
        TKrnlSafeCard4Counter& operator=(const TKrnlSafeCard4Counter&) = delete;


        // -------------------------------------------------------------------
        //  Public, non-virtual methods
        // -------------------------------------------------------------------
        tCIDLib::TCard4 c4AddTo
        (
            const   tCIDLib::TCard4         c4ToAdd
        );

        tCIDLib::TCard4 c4CompareAndExchange
        (
            const   tCIDLib::TCard4         c4New
            , const tCIDLib::TCard4         c4Compare
        );

        tCIDLib::TCard4 c4Exchange
        (
            const   tCIDLib::TCard4         c4New
        );

        tCIDLib::TCard4 c4Dec();

        tCIDLib::TCard4 c4Inc();

        tCIDLib::TCard4 c4SetValue
        (
            const   tCIDLib::TCard4         c4ToSet
        );

        tCIDLib::TCard4 c4SubFrom
        (
            const   tCIDLib::TCard4         c4ToSub
        );

        tCIDLib::TCard4 c4Value() const;


    private :
        // -------------------------------------------------------------------
        //  Private data members
        //
        //  m_c4Counter
        //      This is the counter member that is incremented and decremented.
        //
        //  m_pLockData
        //      This is the per-platform locking data for this counter. Its just an
        //      anonymous pointer here so that it can be anything required internally.
        // -------------------------------------------------------------------
        alignas(kCIDLib::c4CacheAlign) tCIDLib::TCard4  m_c4Counter;
        tCIDLib::TVoid*                                 m_pLockData;
};


// ---------------------------------------------------------------------------
//   CLASS: TKrnlSafeInt4Counter
//  PREFIX: scnt
// ---------------------------------------------------------------------------
class KRNLEXPORT TKrnlSafeInt4Counter
{
    public  :
        // -------------------------------------------------------------------
        //  Constructors and Destructor
        // -------------------------------------------------------------------
        TKrnlSafeInt4Counter();

        TKrnlSafeInt4Counter
        (
            const   tCIDLib::TInt4          i4InitVal
        );

        TKrnlSafeInt4Counter(const TKrnlSafeInt4Counter&) = delete;

        ~TKrnlSafeInt4Counter();


        // -------------------------------------------------------------------
        //  Public operators
        // -------------------------------------------------------------------
        TKrnlSafeInt4Counter& operator=(const TKrnlSafeInt4Counter&) = delete;


        // -------------------------------------------------------------------
        //  Public, non-virtual methods
        // -------------------------------------------------------------------
        tCIDLib::TInt4 i4AddTo
        (
            const   tCIDLib::TInt4          i4ToAdd
        );

        tCIDLib::TInt4 i4CompareAndExchange
        (
            const   tCIDLib::TInt4          i4New
            , const tCIDLib::TInt4          i4Compare
        );

        tCIDLib::TInt4 i4Exchange
        (
            const   tCIDLib::TInt4          i4New
        );

        tCIDLib::TInt4 i4Dec();

        tCIDLib::TInt4 i4Inc();

        tCIDLib::TInt4 i4SetValue
        (
            const   tCIDLib::TInt4          i4ToSet
        );

        tCIDLib::TInt4 i4SubFrom
        (
            const   tCIDLib::TInt4          i4ToSub
        );

        tCIDLib::TInt4 i4Value() const;


    private :
        // -------------------------------------------------------------------
        //  Private data members
        //
        //  m_pi4Counter
        //      This is the counter member that is incremented and decremented.
        //
        //  m_pLockData
        //      This is the per-platform locking data for this counter. Its just an
        //      anonymous pointer here so that it can be anything required internally.
        // -------------------------------------------------------------------
        alignas(kCIDLib::c4CacheAlign) tCIDLib::TInt4   m_i4Counter;
        tCIDLib::TVoid*                                 m_pLockData;
};

#pragma CIDLIB_POPPACK
