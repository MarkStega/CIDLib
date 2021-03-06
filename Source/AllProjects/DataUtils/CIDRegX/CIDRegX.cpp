//
// FILE NAME: CIDRegX.cpp
//
// AUTHOR: Dean Roddey
//
// CREATED: 03/21/1999
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
//  This is the main file of the facility. All we need to do in this case is
//  provide the lazy faulting in method for the facility object.
//
// CAVEATS/GOTCHAS:
//
// LOG:
//
//  $_CIDLib_Log_$
//

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include    "CIDRegX_.hpp"


//
//  This is the lazy eval method for the facility object.
//
TFacCIDRegX& facCIDRegX()
{
    static TFacCIDRegX* pfacThis = new TFacCIDRegX();
    return *pfacThis;
}
