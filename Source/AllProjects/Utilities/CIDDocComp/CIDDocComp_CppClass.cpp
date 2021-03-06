//
// FILE NAME: CIDDocComp_cppClass.Cpp
//
// AUTHOR: Dean Roddey
//
// CREATED: 05/30/2019
//
// COPYRIGHT: $_CIDLib_CopyRight_$
//
//  $_CIDLib_CopyRight2_$
//
// DESCRIPTION:
//
//  A derivative of the base page class, for C++ classes
//
// CAVEATS/GOTCHAS:
//
// LOG:
//
//  $_CIDLib_Log_$
//


// ----------------------------------------------------------------------------
//  Includes
// ----------------------------------------------------------------------------
#include    "CIDDocComp.hpp"


// ----------------------------------------------------------------------------
//  Local helper functions
// ----------------------------------------------------------------------------

//
//  Formats out a 'parameter pass-by' value, though it's also used for members,
//  constants, and return types.
//
static tCIDLib::TVoid
FormatAccType(          TTextOutStream&         strmTar
                , const TString&                strType
                , const tCIDDocComp::EParmPB    eType
                , const tCIDLib::TBoolean       bParmMode
                , const tCIDLib::TBoolean       bRetained = kCIDLib::False
                , const tCIDDocComp::EParmDirs  eDir = tCIDDocComp::EParmDirs::Count)
{
    strmTar << L"<span class=\"DeemphCode\">";

    // If the retained flag is set or the direction needs to be displayed, do that
    if (bRetained
    ||  ((eDir != tCIDDocComp::EParmDirs::Count)
    &&   ((eType == tCIDDocComp::EParmPB::Ref)
    ||    (eType == tCIDDocComp::EParmPB::Ptr)
    ||    (eType == tCIDDocComp::EParmPB::PtrC))))
    {
        strmTar << kCIDLib::chOpenBracket;
        if (bRetained)
            strmTar << L"retained";

        // If asked to show the direction, see if it's one of those types and do that
        if (eDir != tCIDDocComp::EParmDirs::Count)
        {
            if ((eType == tCIDDocComp::EParmPB::Ref)
            ||  (eType == tCIDDocComp::EParmPB::Ptr)
            ||  (eType == tCIDDocComp::EParmPB::PtrC))
            {
                if (bRetained)
                    strmTar << kCIDLib::chSpace;
                strmTar << eDir;
            }
        }
        strmTar << L"] ";
    }

    switch(eType)
    {
        case tCIDDocComp::EParmPB::CPtr :
            strmTar << L"const " << strType << L"*";
            break;

        case tCIDDocComp::EParmPB::CPtrC :
            strmTar << L"const " << strType << L"* const";
            break;

        case tCIDDocComp::EParmPB::CRef :
            strmTar << L"const " << strType << L"&";
            break;

        case tCIDDocComp::EParmPB::MRef :
            strmTar << strType << L"&&";
            break;

        case tCIDDocComp::EParmPB::Ref :
            strmTar << strType << L"&";
            break;

        case tCIDDocComp::EParmPB::Ptr :
            strmTar << strType << L"*";
            break;

        case tCIDDocComp::EParmPB::PtrC :
            strmTar << strType << L"* const";
            break;

        case tCIDDocComp::EParmPB::PtrCRef :
            strmTar << L"const " << strType << L"*&";
            break;

        case tCIDDocComp::EParmPB::PtrRef :
            strmTar << strType << L"*&";
            break;

        case tCIDDocComp::EParmPB::Val :
            // If in parameter mode, by value implies const value
            if (bParmMode)
                strmTar << L"const ";
            strmTar << strType;
            break;

        case tCIDDocComp::EParmPB::Void :
            strmTar << kCIDDocComp::strType_Void;
            break;

        default :
            break;
    };
    strmTar << L"</span>";
}


// ---------------------------------------------------------------------------
//   CLASS: TAliases
//  PREFIX: memg
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  TAliases: Public, non-virtual methods
// ---------------------------------------------------------------------------
tCIDLib::TVoid TAliases::Parse(const TXMLTreeElement& xtnodeSrc)
{
    // It's just a list of Alias elements
    xtnodeSrc.bForEach
    (
        [this](const TXMLTreeElement& xtnodeCur)
        {
            TAlias& aliasNew = m_colList.objAdd(TAlias());

            // If the 0th one is a description process that
            if (xtnodeCur.c4ChildCount())
                aliasNew.m_hnDesc.Parse(xtnodeCur.xtnodeChildAtAsElement(0));

            aliasNew.m_strName = xtnodeCur.strAttr(L"Name");
            aliasNew.m_strType = xtnodeCur.strAttr(L"Type");
            return kCIDLib::True;
        }
    );
}

tCIDLib::TVoid
TAliases::OutputContent(        TTextOutStream&         strmTar
                        , const tCIDDocComp::EVisTypes  eVisType) const
{
    // If we don't have any, do nothing
    if (m_colList.bIsEmpty())
        return;

    strmTar << L"<p><span class='SecHdr'>";
    // The visibility may not be used if it's in a namespace
    if (eVisType != tCIDDocComp::EVisTypes::Count)
        strmTar << eVisType;
    strmTar << L" Aliases</span></p>";

    m_colList.bForEach
    (
        [&strmTar](const TAlias& aliasCur)
        {
            strmTar << L"<pre>";
            facCIDDocComp.FormatDeemphText(strmTar, L"using ");
            facCIDDocComp.FormatEmphText(strmTar, aliasCur.m_strName);
            facCIDDocComp.FormatDeemphText(strmTar, L" = ", aliasCur.m_strType);
            strmTar << L"</pre>";

            //
            //  If any descriptive text, do that in a block quote, else just
            //  put out a break.
            //
            if (!aliasCur.m_hnDesc.bIsEmpty())
            {
                aliasCur.m_hnDesc.OutputHelpText(strmTar);
            }
             else
            {
                strmTar << L"<br/>";
            }
            return kCIDLib::True;
        }
    );
}



// ---------------------------------------------------------------------------
//   CLASS: TEnums
//  PREFIX: memg
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  TEnums: Public, non-virtual methods
// ---------------------------------------------------------------------------
tCIDLib::TVoid TEnums::Parse(const TXMLTreeElement& xtnodeSrc)
{
    xtnodeSrc.bForEach
    (
        [this](const TXMLTreeElement& xtnodeEnum)
        {
            TEnumDef& enumNew = m_colList.objAdd(TEnumDef());
            enumNew.m_strName = xtnodeEnum.strAttr(L"Name");

            // Now parse out this one as an enum value
            xtnodeEnum.bForEach
            (
                [this, &enumNew](const TXMLTreeElement& xtnodeVal)
                {
                    if (xtnodeVal.strQName() == kCIDDocComp::strXML_Desc)
                    {
                        enumNew.m_hnDesc.Parse(xtnodeVal);
                    }
                     else
                    {
                        // Has to be an enum value
                        TEnumVal& enumvNew = enumNew.m_colVals.objAdd(TEnumVal());
                        enumvNew.m_strName = xtnodeVal.strAttr(L"Name");
                        enumvNew.m_hnDesc.Parse(xtnodeVal);
                    }
                    return kCIDLib::True;
                }
            );
            return kCIDLib::True;
        }
    );
}

tCIDLib::TVoid
TEnums::OutputContent(          TTextOutStream&         strmTar
                        , const tCIDDocComp::EVisTypes  eVisType) const
{
    // If we don't have any, do nothing
    if (m_colList.bIsEmpty())
        return;

    strmTar << L"<p><span class='SecHdr'>";
    // The visibility may not be used if it's in a namespace
    if (eVisType != tCIDDocComp::EVisTypes::Count)
        strmTar << eVisType;
    strmTar << L" Enumerations</span></p>";

    m_colList.bForEach
    (
        [&strmTar](const TEnumDef& enumCur)
        {
            strmTar << L"<div style=\"font-size : smaller;\">enum class <b>"
                    << enumCur.m_strName
                    << L"</b><br/>{<div class=\"ExpandItems\">";

            strmTar << L"<div>&nbsp;</div>";
            enumCur.m_colVals.bForEachI
            (
                [&strmTar](const TEnumVal& evalCur, const tCIDLib::TCard4 c4Index)
                {
                    // We put this in a tool tip
                    strmTar << L"<div class=\"ExpandItem\">";
                    if (c4Index)
                        strmTar << L",&nbsp;";
                    strmTar << evalCur.m_strName
                            << L"<div class=\"ExpandItemText\">";
                    evalCur.m_hnDesc.OutputHelpText(strmTar);
                    strmTar << L"</div></div>";
                    return kCIDLib::True;
                }
            );
            strmTar << L"</div>}</div><br/>";

            //
            //  If any descriptive text, do that in a block quote, else just
            //  put out a break.
            //
            if (!enumCur.m_hnDesc.bIsEmpty())
            {
                enumCur.m_hnDesc.OutputHelpText(strmTar);
            }
             else
            {
                strmTar << L"<br/>";
            }
            return kCIDLib::True;
        }
    );
}


// ---------------------------------------------------------------------------
//   CLASS: TMembers
//  PREFIX: memg
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  TMembers: Public, non-virtual methods
// ---------------------------------------------------------------------------
tCIDLib::TVoid TMembers::Parse(const TXMLTreeElement& xtnodeSrc)
{
    // It's just a list of Member elements
    xtnodeSrc.bForEach
    (
        [this](const TXMLTreeElement& xtnodeCur)
        {
            TMember& memberNew = m_colList.objAdd(TMember());

            // If the 0th one is a description process that
            if (xtnodeCur.c4ChildCount())
                memberNew.m_hnDesc.Parse(xtnodeCur.xtnodeChildAtAsElement(0));

            memberNew.m_eAccType = tCIDDocComp::eXlatEParmPB(xtnodeCur.strAttr(L"AccType"));
            memberNew.m_strName = xtnodeCur.strAttr(L"Name");
            memberNew.m_strType = xtnodeCur.strAttr(L"Type");
            return kCIDLib::True;
        }
    );
}

tCIDLib::TVoid
TMembers::OutputContent(        TTextOutStream&         strmTar
                        , const tCIDDocComp::EVisTypes  eVisType) const
{
    // If we don't have any, do nothing
    if (m_colList.bIsEmpty())
        return;

    strmTar << L"<p><span class='SecHdr'>"
            << tCIDDocComp::strXlatEVisTypes(eVisType)
            << L" Data Members</span></p>";

   m_colList.bForEach
    (
        [&strmTar](const TMember& memberCur)
        {
            strmTar << L"<pre>";
            FormatAccType(strmTar, memberCur.m_strType, memberCur.m_eAccType, kCIDLib::False);
            strmTar << kCIDLib::chSpace;
            facCIDDocComp.FormatEmphText(strmTar, memberCur.m_strName);
            strmTar << L"</pre>";

            //
            //  If any descriptive text, do that in a block quote, else just
            //  put out a break.
            //
            if (!memberCur.m_hnDesc.bIsEmpty())
            {
                memberCur.m_hnDesc.OutputHelpText(strmTar);
            }
             else
            {
                strmTar << L"<br/>";
            }
            return kCIDLib::True;
        }
    );
}


// ---------------------------------------------------------------------------
//   CLASS: TMethodParam
//  PREFIX: methp
// ---------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//  TMethodParam: Public, non-virtual methods
// ----------------------------------------------------------------------------
tCIDLib::TVoid TMethodParam::Parse(const TXMLTreeElement& xtnodeSrc)
{
    // The name and type we have to just take as is
    m_strName = xtnodeSrc.strAttr(L"Name");
    m_strType = xtnodeSrc.strAttr(L"Type");

    // The retained flag is defaulted so always present, but mostly just set to No
    m_bRetain = xtnodeSrc.xtattrNamed(L"Retained").bValueAs();

    // We can have an default value
    xtnodeSrc.bAttrExists(kCIDDocComp::strXML_DefValue, m_strDefVal);

    // Get the pass by and convert it
    m_ePassBy = tCIDDocComp::eXlatEParmPB(xtnodeSrc.strAttr(L"PBy"));
    if (!tCIDDocComp::bIsValidEnum(m_ePassBy))
        facCIDDocComp.AddErrorMsg(L"Invalid parameter pass-by type");

    // And the parameter direction
    m_eDir = tCIDDocComp::eXlatEParmDirs(xtnodeSrc.strAttr(L"Dir"));
    if (!tCIDDocComp::bIsValidEnum(m_eDir))
        facCIDDocComp.AddErrorMsg(L"Invalid parameter direction type");
}

tCIDLib::TVoid
TMethodParam::OutputContent(        TTextOutStream&     strmTar
                            , const tCIDLib::TBoolean   bIndent
                            , const tCIDLib::TCard4     c4Index) const
{
    // Format out the type info and its access and direction info
    FormatAccType(strmTar, m_strType, m_ePassBy, kCIDLib::True, m_bRetain, m_eDir);

    strmTar << L"<span class=\"EmphCode\"> " << m_strName
            << L"</span>";

    // if we have a default value, show that
    if (!m_strDefVal.bIsEmpty())
    {
        strmTar << L"</span>"
                << L"<span class=\"DeemphCode\"> = </span>"
                << L"<span class=\"EmphCode\">" << m_strDefVal
                << L"</span>";
    }
}


// ---------------------------------------------------------------------------
//   CLASS: TMethodVar
//  PREFIX: meth
// ---------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//  TMethodVar: Public, non-virtual methods
// ----------------------------------------------------------------------------
tCIDLib::TVoid
TMethodVar::Parse(  const   TXMLTreeElement&        xtnodeSrc
                    , const TString&                strRetType
                    , const tCIDDocComp::EParmPB    eRetBy
                    , const tCIDDocComp::EMethAttrs eMethAttrs
                    , const TString&                strTmplParms)
{
    // Process the attributes, whcih may or may not be present
    m_eAttrs = tCIDDocComp::EMethAttrs::None;

    if (xtnodeSrc.bAttrExists(L"Attrs", facCIDDocComp.m_strTmp1))
    {
        if (TStringTokenizer::bParseSpacedTokens(facCIDDocComp.m_strTmp1, facCIDDocComp.m_colTmp))
        {
            m_eAttrs = tCIDDocComp::EMethAttrs::None;
            tCIDLib::TBoolean bRes = facCIDDocComp.m_colTmp.bForEach
            (
                [this](const TString& strCur)
                {
                    const tCIDDocComp::EMethAttrs eAttr = tCIDDocComp::eXlatEMethAttrs(strCur);
                    if (eAttr == tCIDDocComp::EMethAttrs::None)
                        facCIDDocComp.AddErrorMsg(L"Invalid method attribute '%(1)'", strCur);

                    // It will be the no-attrs value if none are ever set
                    if (eAttr != tCIDDocComp::EMethAttrs::NoAttrs)
                        m_eAttrs |= eAttr;
                    return kCIDLib::True;
                }
            );
        }
        else
        {
            facCIDDocComp.AddErrorMsg(L"Failed to parse method attributes");
        }
    }

    // Add to these any method/method group level attributes
    m_eAttrs |= eMethAttrs;

    // Save the return info and template parms stuff we got
    m_eRetBy = eRetBy;
    m_strRetType = strRetType;
    m_strTmplParms = strTmplParms;

    //
    //  But they can override the return type (mostly for const vs. non-const
    //  of what's otherwise just a variation.
    //
    if (xtnodeSrc.bAttrExists(L"RetBy", facCIDDocComp.m_strTmp1))
    {
        m_eRetBy = tCIDDocComp::eXlatEParmPB(facCIDDocComp.m_strTmp1);
        if (m_eRetBy == tCIDDocComp::EParmPB::Count)
            facCIDDocComp.AddErrorMsg(L"Bad override of method return byte");
    }

	// If the return by isn't None, then we have to have a type
    if ((m_eRetBy != tCIDDocComp::EParmPB::None) && m_strRetType.bIsEmpty())
        facCIDDocComp.AddErrorMsg(L"No return type was indicated");

    // If we have any children, they are parameters of this variation
    if (xtnodeSrc.c4ChildCount())
    {
        xtnodeSrc.bForEach
        (
            [&](const TXMLTreeElement& xtnodeCur)
            {
                TMethodParam& mparamNew = m_colParams.objAdd(TMethodParam());
                mparamNew.Parse(xtnodeCur);
                return kCIDLib::True;
            }
        );
    }
}

tCIDLib::TVoid
TMethodVar::OutputContent(TTextOutStream& strmTar, const TString& strName) const
{
    // If a template do the
    if (tCIDLib::bAllBitsOn(m_eAttrs, tCIDDocComp::EMethAttrs::Template))
        strmTar << L"template&lt;" << m_strTmplParms << L"> ";

    // Do any that go before the type
    if (tCIDLib::bAllBitsOn(m_eAttrs, tCIDDocComp::EMethAttrs::Friend))
        strmTar << L"friend ";
    if (tCIDLib::bAllBitsOn(m_eAttrs, tCIDDocComp::EMethAttrs::ConstExpr))
        strmTar << L"constexpr ";
    if (tCIDLib::bAllBitsOn(m_eAttrs, tCIDDocComp::EMethAttrs::Explicit))
        strmTar << L"explicit ";
    if (tCIDLib::bAllBitsOn(m_eAttrs, tCIDDocComp::EMethAttrs::NoDiscard))
        strmTar << L"[[nodiscard]] ";
    if (tCIDLib::bAllBitsOn(m_eAttrs, tCIDDocComp::EMethAttrs::Static))
        strmTar << L"static ";
    if (tCIDLib::bAllBitsOn(m_eAttrs, tCIDDocComp::EMethAttrs::Virtual))
        strmTar << L"virtual ";

    //
    //  Do the return type. But check for a retry by of None, which is valid for
    //  constructors and things like conversion operators.
    //
    if (!m_strRetType.bIsEmpty())
    {
        //
        //  For return types, constants, and members, we can use the same code.
        //  We just use a local for that.
        //
        FormatAccType(strmTar, m_strRetType, m_eRetBy, kCIDLib::False);
        strmTar << kCIDLib::chSpace;
    }

    // The name, emphasized
    facCIDDocComp.FormatEmphText(strmTar, strName);

    //
    //  Decide if we want to wrap the parameters or put the whole thing on one
    //  line. If it's just a single paramater, then we always do one line. If it's
    //  two and none of them have default values, we do one line. Else we wrap.
    //
    //  So first check for default values. Break out as soon as we see one, which
    //  means the for each returns false, which we negate to true.
    //
    const tCIDLib::TBoolean bHaveDefVals = !m_colParams.bForEach
    (
        [&bHaveDefVals](const TMethodParam& methpCur)
        {
            return methpCur.m_strDefVal.bIsEmpty();
        }
    );
    const tCIDLib::TCard4 c4PCnt = m_colParams.c4ElemCount();
    const tCIDLib::TBoolean bWrapped
    (
        (c4PCnt > 2) || ((c4PCnt == 2) && bHaveDefVals)
    );
    if (bWrapped)
        strmTar << L"<br/>(<br/>    ";
    else
        strmTar << L"(";

    // Format each parameter, breaking after if needed
    m_colParams.bForEachI
    (
        [&](const TMethodParam& methpCur, const tCIDLib::TCard4 c4Index)
        {
            if (c4Index)
            {
                if (bWrapped)
                    strmTar << L"\n, ";
                else
                    strmTar << L", ";
            }
            methpCur.OutputContent(strmTar, bWrapped, c4Index);
            return kCIDLib::True;
        }
    );

    // Close this one of now, wrapping after the final parameter if needed
    if (bWrapped)
        strmTar << L"<br/>";
    strmTar << L")";

    //
    //  Do any attributes that go after the end. Order them such that if more than one
    //  are set (in any legal combination) they will make sense as displayed.
    //
    if (tCIDLib::bAllBitsOn(m_eAttrs, tCIDDocComp::EMethAttrs::Const))
        strmTar << L" const";
    if (tCIDLib::bAllBitsOn(m_eAttrs, tCIDDocComp::EMethAttrs::Override))
        strmTar << L" override";
    if (tCIDLib::bAllBitsOn(m_eAttrs, tCIDDocComp::EMethAttrs::Default))
        strmTar << L" = default";
    if (tCIDLib::bAllBitsOn(m_eAttrs, tCIDDocComp::EMethAttrs::Delete))
        strmTar << L" = delete";
    if (tCIDLib::bAllBitsOn(m_eAttrs, tCIDDocComp::EMethAttrs::NoExcept))
        strmTar << L" noexcept";
    if (tCIDLib::bAllBitsOn(m_eAttrs, tCIDDocComp::EMethAttrs::Pure))
        strmTar << L" = 0";
}



// ---------------------------------------------------------------------------
//   CLASS: TMethod
//  PREFIX: meth
// ---------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//  TMethod: Public, non-virtual methods
// ----------------------------------------------------------------------------

tCIDLib::TVoid
TMethod::Parse(const TXMLTreeElement& xtnodeSrc, const tCIDDocComp::EMethAttrs eGrpAttrs)
{
    m_strName = xtnodeSrc.strAttr(L"Name");
    m_strRetType = xtnodeSrc.strAttr(L"RetType");

    // If we have the a return by take that, else let's try to figure out a default
    m_eRetBy = tCIDDocComp::EParmPB::None;
    if (xtnodeSrc.bAttrExists(L"RetBy", facCIDDocComp.m_strTmp1))
    {
        m_eRetBy = tCIDDocComp::eXlatEParmPB(facCIDDocComp.m_strTmp1);

        //
        //  If no return type, then return by has to be none. If void then return by
        //  has to be void also. Else return by cannot be none or void.
        //
        if (m_strRetType.bIsEmpty())
        {
            if (m_eRetBy != tCIDDocComp::EParmPB::None)
                facCIDDocComp.AddErrorMsg(L"If no return type, return by has to be None");
        }
         else if ((m_strRetType == kCIDDocComp::strType_Void)
              &&  (m_eRetBy != tCIDDocComp::EParmPB::Void))
        {
            facCIDDocComp.AddErrorMsg(L"If no return type is void, return by must be");
        }
         else if ((m_eRetBy == tCIDDocComp::EParmPB::None)
              ||  (m_eRetBy == tCIDDocComp::EParmPB::Void))
        {
            facCIDDocComp.AddErrorMsg(L"Return by cannot be none/void if a return type is defined");
        }
    }
     else
    {
        if (m_strRetType.bIsEmpty())
        {
            if (tCIDLib::bAllBitsOn(m_eMethAttrs, tCIDDocComp::EMethAttrs::Ctor))
            {
                // Constructors have none
                m_eRetBy = tCIDDocComp::EParmPB::None;
            }
             else
            {
                // Go with void
                m_eRetBy = tCIDDocComp::EParmPB::Void;
                m_strRetType = kCIDDocComp::strType_Void;
            }
        }
         else
        {
            //
            //  If void, set the return by to void, else to value. These are the
            //  common scenarios we want to default.
            //
            if (m_strRetType == kCIDDocComp::strType_Void)
                m_eRetBy = tCIDDocComp::EParmPB::Void;
            else
                m_eRetBy = tCIDDocComp::EParmPB::Val;
        }
    }

    // First child is general description
    m_hnDescr.Parse(xtnodeSrc.xtnodeChildAtAsElement(0));

    // Process the ovearll method level attributes, which may or may not be present
    m_eMethAttrs = tCIDDocComp::EMethAttrs::None;
    if (xtnodeSrc.bAttrExists(L"Attrs", facCIDDocComp.m_strTmp1))
    {
        if (TStringTokenizer::bParseSpacedTokens(facCIDDocComp.m_strTmp1, facCIDDocComp.m_colTmp))
        {
            m_eMethAttrs = tCIDDocComp::EMethAttrs::None;
            tCIDLib::TBoolean bRes = facCIDDocComp.m_colTmp.bForEach
            (
                [this](const TString& strCur)
                {
                    const tCIDDocComp::EMethAttrs eAttr = tCIDDocComp::eXlatEMethAttrs(strCur);
                    if (eAttr == tCIDDocComp::EMethAttrs::None)
                        facCIDDocComp.AddErrorMsg(L"Invalid method attribute '%(1)'", strCur);

                    // It will be the no-attrs value if none are ever set
                    if (eAttr != tCIDDocComp::EMethAttrs::NoAttrs)
                        m_eMethAttrs |= eAttr;
                    return kCIDLib::True;
                }
            );
        }
        else
        {
            facCIDDocComp.AddErrorMsg(L"Failed to parse method attributes");
        }
    }

    // Add to these any group level attributes
    m_eMethAttrs |= eGrpAttrs;

    // If there are template parameters, get them
    xtnodeSrc.bAttrExists(L"TmplParms", m_strTmplParms);

    // Next we have a list of method variations
    xtnodeSrc.bForEach
    (
        [&](const TXMLTreeElement& xtnodeCur)
        {
            if (xtnodeCur.strQName() == kCIDDocComp::strXML_Desc)
            {
                // It's the general descriptive text for this method
                m_hnDescr.Parse(xtnodeCur);
            }
             else
            {
                // Has to be a method variation
                TMethodVar& mvarNew = m_colMethVars.objAdd(TMethodVar());
                mvarNew.Parse(xtnodeCur, m_strRetType, m_eRetBy, m_eMethAttrs, m_strTmplParms);
            }
            return kCIDLib::True;
        }
    );
}

tCIDLib::TVoid TMethod::OutputContent(TTextOutStream& strmTar) const
{
    //  Output all of the method variations, in a preformatted block.
    strmTar << L"<pre>";
    m_colMethVars.bForEach
    (
        [&](const TMethodVar& mvarCur)
        {
            // Put a break after each one
            mvarCur.OutputContent(strmTar, m_strName);
            strmTar << L"</br>";
            return kCIDLib::True;
        }
    );
    strmTar << L"</pre>";

    //
    //  Indent the general text and output that. Add a special div after it to
    //  create a little vertical spacing.
    //
    m_hnDescr.OutputHelpText(strmTar);
    strmTar << L"<div class=\"TrailingSpace\"></div>" << kCIDLib::NewLn;
}




// ---------------------------------------------------------------------------
//   CLASS: TMethodGrp
//  PREFIX: methg
// ---------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//  TMethodGrp: Public, non-virtual methods
// ----------------------------------------------------------------------------

// Indicate whether we have any information to display
tCIDLib::TBoolean TMethodGrp::bIsEmpty() const
{
    return  m_colMethods.bIsEmpty()
            && m_hnDescr.bIsEmpty()
            && m_fcolDefMethods.bIsEmpty()
            && m_fcolDelMethods.bIsEmpty();
}


tCIDLib::TVoid
TMethodGrp::Parse(const TXMLTreeElement& xtnodeSrc)
{
    ParseSpecMethods(xtnodeSrc, L"DefMethods", m_fcolDefMethods);
    ParseSpecMethods(xtnodeSrc, L"DelMethods", m_fcolDelMethods);

    xtnodeSrc.bForEach
    (
        [&](const TXMLTreeElement& xtnodeCur)
        {
            if (xtnodeCur.strQName() == kCIDDocComp::strXML_Desc)
            {
                m_hnDescr.Parse(xtnodeCur);
            }
             else if (xtnodeCur.strQName() == kCIDDocComp::strXML_GetSet)
            {
                ParseGetSet(xtnodeCur);
            }
             else if (xtnodeCur.strQName() == kCIDDocComp::strXML_Method)
            {
                TMethod& methNew = m_colMethods.objAdd(TMethod());
                methNew.Parse(xtnodeCur, m_eGrpAttrs);
            }
             else
            {
                CIDAssert2X
                (
                    L"Unknown child element of method group: %(1)", xtnodeSrc.strQName()
                );
            }
            return kCIDLib::True;
        }
    );
}


tCIDLib::TVoid TMethodGrp::OutputContent(TTextOutStream& strmTar) const
{
    // If there's overall group description text, then output that
    if (!m_hnDescr.bIsEmpty())
        m_hnDescr.OutputHelpText(strmTar, kCIDLib::False);

    // And then do the methods
    m_colMethods.bForEach
    (
        [this, &strmTar] (const TMethod& methCur)
        {
            methCur.OutputContent(strmTar);
            return kCIDLib::True;
        }
    );

    // Do any defaulted or deleted methods
    if (!m_fcolDefMethods.bIsEmpty())
    {
        strmTar << L"<pre>";
        m_fcolDefMethods.bForEach
        (
            [&strmTar, this](const tCIDDocComp::ESpecMeths eMethod)
            {
                OutputSpecMethod(strmTar, eMethod, kCIDLib::True);
                return kCIDLib::True;
            }
        );
        strmTar << L"</pre>"
                   L"<div class=\"HelpTextCont\">"
                   L"These methods get default implementations</div>";
    }

    if (!m_fcolDelMethods.bIsEmpty())
    {
        strmTar << L"<pre>";
        m_fcolDelMethods.bForEach
        (
            [&strmTar, this](const tCIDDocComp::ESpecMeths eMethod)
            {
                OutputSpecMethod(strmTar, eMethod, kCIDLib::False);
                return kCIDLib::True;
            }
        );
        strmTar << L"</pre>"
                   L"<div class=\"HelpTextCont\">"
                   L"These methods have been deleted and are not available</div>";
    }
}


// ----------------------------------------------------------------------------
//  TMethodGrp: Public, non-virtual methods
// ----------------------------------------------------------------------------
tCIDLib::TVoid
TMethodGrp::OutputSpecMethod(       TTextOutStream&         strmTar
                            , const tCIDDocComp::ESpecMeths eMeth
                            , const tCIDLib::TBoolean       bDefault) const
{
    const TString& strClass = facCIDDocComp.strCurClass();
    const TString& strPrefix = facCIDDocComp.strCurClassPref();
    switch(eMeth)
    {
        case tCIDDocComp::ESpecMeths::DefCtor :
            facCIDDocComp.FormatEmphText(strmTar, strClass);
            facCIDDocComp.FormatDeemphText(strmTar, L"()");
            break;

        case tCIDDocComp::ESpecMeths::CopyCtor :
            facCIDDocComp.FormatEmphText(strmTar, strClass);
            strmTar << L"(";
            facCIDDocComp.FormatDeemphText(strmTar, L"const ", strClass);
            strmTar << L"& ";
            facCIDDocComp.FormatEmphText(strmTar, strPrefix, L"Src");
            strmTar << L")";
            break;

        case tCIDDocComp::ESpecMeths::MoveCtor :
            facCIDDocComp.FormatEmphText(strmTar, strClass);
            strmTar << L"(";
            facCIDDocComp.FormatEmphText(strmTar, strClass);
            strmTar << L"&&)";
            break;

        case tCIDDocComp::ESpecMeths::Dtor :
            strmTar << L"~";
            facCIDDocComp.FormatEmphText(strmTar, strClass);
            facCIDDocComp.FormatDeemphText(strmTar, L"()");
            break;

        case tCIDDocComp::ESpecMeths::AssignOp :
            facCIDDocComp.FormatDeemphText(strmTar, strClass, L"& ");
            facCIDDocComp.FormatEmphText(strmTar,  L"operator=");
            strmTar << L"(";
            facCIDDocComp.FormatDeemphText(strmTar, L"const ", strClass, L"& ");
            facCIDDocComp.FormatEmphText(strmTar, strPrefix, L"Src");
            strmTar << L")";
            break;

        case tCIDDocComp::ESpecMeths::MoveOp :
            facCIDDocComp.FormatDeemphText(strmTar, strClass, L"& ");
            facCIDDocComp.FormatEmphText(strmTar,  L"operator=");
            strmTar << L"(";
            facCIDDocComp.FormatDeemphText(strmTar, strClass, L"&& ");
            facCIDDocComp.FormatEmphText(strmTar, strPrefix, L"Src");
            strmTar << L")";
            break;

        case tCIDDocComp::ESpecMeths::NewOp :
            facCIDDocComp.FormatDeemphText(strmTar, L"tCIDLib::TVoid* ");
            facCIDDocComp.FormatEmphText(strmTar,  L"operator new");
            strmTar << L"(";
            facCIDDocComp.FormatDeemphText(strmTar,  L"const size_t ");
            facCIDDocComp.FormatEmphText(strmTar,  L"size");
            strmTar << L")";
            break;

        default :
            facCIDDocComp.AddErrorMsg(L"Unknown special method type");
            break;
    };

    // And finally add the default or deleted bit
    if (bDefault)
        strmTar << L" = default";
    else
        strmTar << L" = delete";

    strmTar << L"<br/>";
}


//
//  We handle the special case getter/setting definition and spit out methods of
//  a semi-fixed form.
//
tCIDLib::TVoid TMethodGrp::ParseGetSet(const TXMLTreeElement& xtnodeSrc)
{
    const TString& strName = xtnodeSrc.strAttr(L"Name");
    const TString& strRetType = xtnodeSrc.strAttr(L"RetType");

    // If not set, we assume a by value return type, which is very common
    tCIDDocComp::EParmPB eRetBy = tCIDDocComp::EParmPB::Val;
    if (xtnodeSrc.bAttrExists(L"RetBy", facCIDDocComp.m_strTmp1))
        eRetBy = tCIDDocComp::eXlatEParmPB(facCIDDocComp.m_strTmp1);

    // We must have a return type and return by cannot be None for this scenario
    if (strRetType.bIsEmpty() || (eRetBy == tCIDDocComp::EParmPB::None))
    {
        facCIDDocComp.AddErrorMsg(L"A getter/setter must have a return type");
        return;
    }

    // Translate the get/set directions supported
    const tCIDDocComp::EGSDirs eDirs = tCIDDocComp::eXlatEGSDirs(xtnodeSrc.strAttr(L"Dir"));
    if (eDirs == tCIDDocComp::EGSDirs::Count)
    {
        facCIDDocComp.AddErrorMsg(L"Unknown getter/setter direction type");
        return;
    }

    // This much is the same either way
    TMethod& methGet = m_colMethods.objAdd(TMethod());
    methGet.m_strName = strName;

    //
    //  If it has any descriptive text, then process that. Else, generate a default
    //  one.
    //
    if (xtnodeSrc.c4ChildCount())
    {
        //
        //  The help text is a direct child of us, and this guy parses help text starting
        //  with the chidren of the passed node.
        //
        methGet.m_hnDescr.Parse(xtnodeSrc);
    }
     else
    {
        if (eDirs == tCIDDocComp::EGSDirs::Get)
            facCIDDocComp.m_strTmp1 = L"Returns the current value of the m_";
        else if (eDirs == tCIDDocComp::EGSDirs::Set)
            facCIDDocComp.m_strTmp1 = L"Sets the current value of the ";
        else
            facCIDDocComp.m_strTmp1 = L"Get or set the current value of the m_";
        facCIDDocComp.m_strTmp1 += strName;
        facCIDDocComp.m_strTmp1 += L" member";
        methGet.m_hnDescr.SetToText(facCIDDocComp.m_strTmp1.pszBuffer());
    }

    // Then add method variations for the directions supported
    if ((eDirs == tCIDDocComp::EGSDirs::Both)
    ||  (eDirs == tCIDDocComp::EGSDirs::Get))
    {
        TMethodVar& mvarNew = methGet.m_colMethVars.objAdd(TMethodVar());
        mvarNew.m_eRetBy = eRetBy;
        mvarNew.m_strRetType = strRetType;
        mvarNew.m_eAttrs = tCIDDocComp::EMethAttrs::Const;
    }

    if ((eDirs == tCIDDocComp::EGSDirs::Both)
    ||  (eDirs == tCIDDocComp::EGSDirs::Set))
    {
        TMethodVar& mvarNew = methGet.m_colMethVars.objAdd(TMethodVar());
        mvarNew.m_eRetBy = eRetBy;
        mvarNew.m_strRetType = strRetType;
        mvarNew.m_eAttrs = tCIDDocComp::EMethAttrs::None;
        TMethodParam& mparmNew = mvarNew.m_colParams.objAdd(TMethodParam());
        mparmNew.m_ePassBy = eRetBy;
        mparmNew.m_strType = strRetType;

        //
        //  We need a name, which requires the type prefix. WE basically take chars
        //  from the method type until we hit an upper-case letter.
        //
        const tCIDLib::TCard4 c4Count = strName.c4Length();
        for (tCIDLib::TCard4 c4Index = 0; c4Index < c4Count; c4Index++)
        {
            if (TRawStr::bIsDigit(strName[c4Index])
            ||  TRawStr::bIsLower(strName[c4Index]))
            {
                mparmNew.m_strName += strName[c4Index];
            }
             else
            {
                break;
            }
        }
        mparmNew.m_strName += L"ToSet";
    }
}


tCIDLib::TVoid
TMethodGrp::ParseSpecMethods(const  TXMLTreeElement&            xtnodePar
                            , const  TString&                   strAttr
                            ,       tCIDDocComp::TSpecMeths&    fcolLoad)
{
    // Not all of them define these, so just return if not found
    if (!xtnodePar.bAttrExists(strAttr, facCIDDocComp.m_strTmp1))
        return;

    if (!TStringTokenizer::bParseSpacedTokens(facCIDDocComp.m_strTmp1, facCIDDocComp.m_colTmp))
    {
        facCIDDocComp.AddErrorMsg
        (
            L"Can't parse special methods for %(1)", facCIDDocComp.strCurClass()
        );
    }

    fcolLoad.RemoveAll();
    facCIDDocComp.m_colTmp.bForEach
    (
        [this, &fcolLoad](const TString& strCur)
        {
            const tCIDDocComp::ESpecMeths eMeth = tCIDDocComp::eXlatESpecMeths(strCur);
            if (eMeth == tCIDDocComp::ESpecMeths::Count)
            {
                facCIDDocComp.AddErrorMsg
                (
                    L"Bad special method for class %(1)", facCIDDocComp.strCurClass()
                );
                return kCIDLib::False;
            }

            // If it's an empty list, it'll just be the none value
            if (eMeth != tCIDDocComp::ESpecMeths::None)
                fcolLoad.c4AddElement(eMeth);
            return kCIDLib::True;
        }
    );
}




// ---------------------------------------------------------------------------
//   CLASS: TMemberGrp
//  PREFIX: memg
// ---------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//  TMemberGrp: Constructors and Destructor
// ----------------------------------------------------------------------------
TMemberGrp::TMemberGrp(const tCIDDocComp::EVisTypes eVisType) :

    m_eVisType(eVisType)
    , m_methgCtors(tCIDDocComp::EMethAttrs::Ctor)
    , m_methgNVirtMethods()
    , m_methgOperators(tCIDDocComp::EMethAttrs::Operator)
    , m_methgOverMethods(tCIDDocComp::EMethAttrs::Override)
    , m_methgStatMethods(tCIDDocComp::EMethAttrs::Static)
    , m_methgVirtMethods(tCIDDocComp::EMethAttrs::Virtual)
{
}


// ----------------------------------------------------------------------------
//  TMemberGrp: Public, non-virtual methods
// ----------------------------------------------------------------------------
tCIDLib::TBoolean TMemberGrp::bIsEmpty() const
{
    return
    (
        m_memgAliases.bIsEmpty()
        && m_memgEnums.bIsEmpty()
        && m_memgMembers.bIsEmpty()
        && m_methgCtors.bIsEmpty()
        && m_methgNVirtMethods.bIsEmpty()
        && m_methgOperators.bIsEmpty()
        && m_methgOverMethods.bIsEmpty()
        && m_methgStatMethods.bIsEmpty()
        && m_methgVirtMethods.bIsEmpty()
    );
}

//
//  We parse all of the members for one of the visibility scopes. We don't care
//  which it is, since they all are the same format. The caller knows.
//
tCIDLib::TVoid TMemberGrp::Parse(const TXMLTreeElement& xtnodeGrp)
{
    xtnodeGrp.bForEach
    (
        [&](const TXMLTreeElement& xtnodeElem)
        {
            const TString& strName = xtnodeElem.strQName();
            if (strName == L"Aliases")
            {
                m_memgAliases.Parse(xtnodeElem);
            }
             else if (strName == L"Constants")
            {
            }
             else if (strName == L"Ctors")
            {
                m_methgCtors.Parse(xtnodeElem);
            }
             else if (strName == L"Enums")
            {
                m_memgEnums.Parse(xtnodeElem);
            }
             else if (strName == L"NVirtMeths")
            {
                m_methgNVirtMethods.Parse(xtnodeElem);
            }
             else if (strName == L"Operators")
            {
                m_methgOperators.Parse(xtnodeElem);
            }
             else if (strName == L"OverMeths")
            {
                m_methgOverMethods.Parse(xtnodeElem);
            }
             else if (strName == L"StaticMeths")
            {
                m_methgStatMethods.Parse(xtnodeElem);
            }
            else if (strName == L"VirtMeths")
            {
                m_methgVirtMethods.Parse(xtnodeElem);
            }
             else if (strName == L"Members")
            {
                m_memgMembers.Parse(xtnodeElem);
            }
             else
            {
                facCIDDocComp.AddErrorMsg(L"Unknown member group element: %(1)", strName);
            }
            return kCIDLib::True;
        }
    );
}

tCIDLib::TVoid TMemberGrp::OutputContent(TTextOutStream& strmTar) const
{
    // Do the aliases first if any
    m_memgAliases.OutputContent(strmTar, m_eVisType);

    // Then enums
    m_memgEnums.OutputContent(strmTar, m_eVisType);

    // Do the various method groups
    if (!m_methgStatMethods.bIsEmpty())
    {
        strmTar << L"<p><span class='SecHdr'>"
                << tCIDDocComp::strXlatEVisTypes(m_eVisType)
                << L" Static Methods</span></p>";
        TStreamIndentJan janIndent(&strmTar, 4);
        m_methgStatMethods.OutputContent(strmTar);
    }

    if (!m_methgCtors.bIsEmpty())
    {
        strmTar << L"<p><span class='SecHdr'>"
                << tCIDDocComp::strXlatEVisTypes(m_eVisType)
                << L" Constructors and Destructor</span></p>";
        TStreamIndentJan janIndent(&strmTar, 4);
        m_methgCtors.OutputContent(strmTar);
    }

    if (!m_methgOperators.bIsEmpty())
    {
        strmTar << L"<p><span class='SecHdr'>"
                << tCIDDocComp::strXlatEVisTypes(m_eVisType)
                << L" Operators</span></p>";
        TStreamIndentJan janIndent(&strmTar, 4);
        m_methgOperators.OutputContent(strmTar);
    }

    if (!m_methgVirtMethods.bIsEmpty())
    {
        strmTar << L"<p><span class='SecHdr'>"
                << tCIDDocComp::strXlatEVisTypes(m_eVisType)
                << L" Virtual Methods</span></p>";
        TStreamIndentJan janIndent(&strmTar, 4);
        m_methgVirtMethods.OutputContent(strmTar);
    }

    if (!m_methgOverMethods.bIsEmpty())
    {
        strmTar << L"<p><span class='SecHdr'>"
                << tCIDDocComp::strXlatEVisTypes(m_eVisType)
                << L" Overrides</span></p>";
        TStreamIndentJan janIndent(&strmTar, 4);
        m_methgOverMethods.OutputContent(strmTar);
    }

    if (!m_methgNVirtMethods.bIsEmpty())
    {
        strmTar << L"<p><span class='SecHdr'>"
                << tCIDDocComp::strXlatEVisTypes(m_eVisType)
                << L" Non-Virtual Methods</span></p>";
        TStreamIndentJan janIndent(&strmTar, 4);
        m_methgNVirtMethods.OutputContent(strmTar);
    }

    // If we have any members, do those
    m_memgMembers.OutputContent(strmTar, m_eVisType);
}





// ---------------------------------------------------------------------------
//   CLASS: TCppClassPage
//  PREFIX: pg
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  TCppClassPage: Constructors and Destructor
// ---------------------------------------------------------------------------
TCppClassPage::TCppClassPage(   const   TString&    strExtTitle
                                , const TString&    strParSrcDir
                                , const TString&    strParTopic
                                , const TString&    strFileName) :

    TBasePage
    (
        strExtTitle
        , strParSrcDir
        , strParTopic
        , strFileName
        , tCIDDocComp::EPageTypes::CppClass
    )
    , m_memgProtected(tCIDDocComp::EVisTypes::Protected)
    , m_memgPrivate(tCIDDocComp::EVisTypes::Private)
    , m_memgPublic(tCIDDocComp::EVisTypes::Public)
{
    //
    //  If we have not faulted in the static definitions of the common mixin
    //  methods, then let's do that now.
    //
    if (!s_bStatsDone)
        LoadMixinMethods();
}


// ---------------------------------------------------------------------------
//  TCppClassPage: Private, static members
// ---------------------------------------------------------------------------
tCIDLib::TBoolean TCppClassPage::s_bStatsDone = kCIDLib::False;
TMethod TCppClassPage::s_methDuplicate;
TMethod TCppClassPage::s_methFormatTo;
TMethod TCppClassPage::s_methStreamFrom;
TMethod TCppClassPage::s_methStreamTo;


// ---------------------------------------------------------------------------
//  TCppClassPage: Private, inherited methods
// ---------------------------------------------------------------------------
tCIDLib::TBoolean TCppClassPage::bFlagOn(const tCIDDocComp::EClsFlags eCheck) const
{
    return tCIDLib::bAllBitsOn(m_eFlags, eCheck);
}


tCIDLib::TVoid
TCppClassPage::Parse(           TTopic&             topicParent
                        , const TXMLTreeElement&    xtnodeClass)
{
    // Push us onto the context stack for this scope
    TCtxStackJan janStack(*this);

    // First we need to get out our class name, parent class, and prefix
    m_strClass = xtnodeClass.strAttr(L"Class");
    m_strParClass = xtnodeClass.strAttr(L"Parent");
    m_strPrefix = xtnodeClass.strAttr(L"Prefix");

    // Get the boolean flags out and parse them
    const TString& strFlags = xtnodeClass.strAttr(L"Flags");
    if (!TStringTokenizer::bParseSpacedTokens(strFlags, facCIDDocComp.m_colTmp))
        facCIDDocComp.AddErrorMsg(L"Failed to parse class flags");

    // Translate any flags and set them on our flags member
    m_eFlags = tCIDDocComp::EClsFlags::None;
    facCIDDocComp.m_colTmp.bForEach
    (
        [this](const TString& strCur)
        {
            const tCIDDocComp::EClsFlags eFlag = tCIDDocComp::eXlatEClsFlags(strCur);
            if (eFlag == tCIDDocComp::EClsFlags::None)
                facCIDDocComp.AddErrorMsg(L"Invalid class flag '%(1) for %(2)", strCur, m_strClass);

            // It could be the no-flags value
            if (eFlag != tCIDDocComp::EClsFlags::NoFlags)
                m_eFlags |= eFlag;
            return kCIDLib::True;
        }
    );

    // We may have template params
    tCIDLib::TCard4 c4At;
    const TXMLTreeElement* pxtnodeTPs
    (
        xtnodeClass.pxtnodeFindElement(L"TmplParams", 0, c4At)
    );
    if (pxtnodeTPs)
    {
        // Add each one to our template parms collection
        pxtnodeTPs->bForEach
        (
            [this](const TXMLTreeElement& xtnodePT)
            {
                this->m_colTmplParams.objAdd
                (
                    TKeyValuePair
                    (
                        xtnodePT.strAttr(L"Name"), xtnodePT.strAttr(L"DefVal")
                    )
                );
                return kCIDLib::True;
            }
        );
    }
     else
    {
        // If  the template class flag is on, then this is bad
        if (tCIDLib::bAllBitsOn(m_eFlags, tCIDDocComp::EClsFlags::Template))
            facCIDDocComp.AddErrorMsg(L"Template flag is set, but no template parms provided");
    }

    const TXMLTreeElement& xtnodeDesc = xtnodeClass.xtnodeFindElement
    (
        kCIDDocComp::strXML_Desc, 0, c4At
    );
    m_hnDesc.Parse(xtnodeDesc);

    //
    //  Next we have optional public, protected and private members stuff. They
    //  have the same content, so we call a helper with each one.
    //
    const TXMLTreeElement* pxtnodeGrp;
    pxtnodeGrp = xtnodeClass.pxtnodeFindElement(L"PubIntf", 0, c4At);
    if (pxtnodeGrp)
        m_memgPublic.Parse(*pxtnodeGrp);

    pxtnodeGrp = xtnodeClass.pxtnodeFindElement(L"ProtIntf", 0, c4At);
    if (pxtnodeGrp)
        m_memgProtected.Parse(*pxtnodeGrp);

    pxtnodeGrp = xtnodeClass.pxtnodeFindElement(L"PrivIntf", 0, c4At);
    if (pxtnodeGrp)
        m_memgPrivate.Parse(*pxtnodeGrp);

    //
    //  If we implement any of the standard mixins, spit out the pre-fab method
    //  definitions that we previously set up.
    //
    if (bFlagOn(tCIDDocComp::EClsFlags::Dup))
        m_memgPublic.m_methgOverMethods.m_colMethods.objAdd(s_methDuplicate);

    if (bFlagOn(tCIDDocComp::EClsFlags::Fmt))
        m_memgProtected.m_methgOverMethods.m_colMethods.objAdd(s_methFormatTo);

    if (bFlagOn(tCIDDocComp::EClsFlags::BStrm))
    {
        m_memgProtected.m_methgOverMethods.m_colMethods.objAdd(s_methStreamFrom);
        m_memgProtected.m_methgOverMethods.m_colMethods.objAdd(s_methStreamTo);
    }
}


tCIDLib::TVoid TCppClassPage::OutputContent(TTextOutStream& strmTar) const
{
    //
    //  Push our class name into the source stack. During output it us just used
    //  to keep up with the current class.
    //
    TCtxStackJan janStack(*this);

    // The class name is the title
    strmTar << L"<p><span class='PageHdr'> Class: "
            << m_strClass
            << L"</span></p>";

    // We have a little table of key info at the top
    strmTar << L"<blockquote><table class='CppClsHdrTable'>";
    strmTar << L"<tr><td>Parent Class:</td><td>"
            << m_strParClass
            << L"</td></tr>";

    static const tCIDDocComp::EClsFlags eMixins
    (
        tCIDDocComp::EClsFlags::Dup
        | tCIDDocComp::EClsFlags::Fmt
        | tCIDDocComp::EClsFlags::BStrm
    );

    if (tCIDLib::bAnyBitsOn(m_eFlags, eMixins))
    {
        strmTar << L"<tr><td>Std Mixins:</td><td>";
        if (bFlagOn(tCIDDocComp::EClsFlags::Dup))
            strmTar << L"MDuplicable ";

        if (bFlagOn(tCIDDocComp::EClsFlags::Fmt))
            strmTar << L"MFormattable ";

        if (bFlagOn(tCIDDocComp::EClsFlags::BStrm))
            strmTar << L"MStreamable ";

        strmTar << L"</td></tr>";
    }

    if (!m_strPrefix.bIsEmpty())
    {
        strmTar << L"<tr><td>Prefix:</td><td>"
                << m_strPrefix << L"</td></tr>";
    }

    strmTar << L"</table></blockquote><Br/>";

    //
    //  Before the descriptive text, if this is a template, we want to output
    //  the full signature with template parameters.
    //
    if (!m_colTmplParams.bIsEmpty())
    {
        const tCIDLib::TBoolean bWrap(m_colTmplParams.c4ElemCount() > 2);
        strmTar << L"<pre>template class&lt;";
        if (bWrap)
            strmTar << kCIDLib::NewLn;
        m_colTmplParams.bForEachI
        (
            [&strmTar, bWrap]
            (const TKeyValuePair& kvalCur, const tCIDLib::TCard4 c4Index)
            {
                if (bWrap)
                    strmTar << L"    ";
                if (c4Index)
                    strmTar << L", ";
                strmTar << L"typename " << kvalCur.strKey();
                if (!kvalCur.strValue().bIsEmpty())
                    strmTar << L" = " << kvalCur.strValue();
                return kCIDLib::True;
            }
        );
        if (bWrap)
            strmTar << kCIDLib::NewLn;
        strmTar << L"> <b>" << m_strClass << L"</b></pre>";
    }

    // Next should be the descriptive text.
    m_hnDesc.OutputHelpText(strmTar, kCIDLib::False);

    // Do a little divider
    strmTar << L"<div class=\"HorzDivCont\"><div class=\"HorzDiv\"></div></div>";

    //  Output any non-empty member groups
    if (!m_memgPublic.bIsEmpty())
        m_memgPublic.OutputContent(strmTar);
    if (!m_memgProtected.bIsEmpty())
        m_memgProtected.OutputContent(strmTar);
    if (!m_memgPrivate.bIsEmpty())
        m_memgPrivate.OutputContent(strmTar);
}


// ---------------------------------------------------------------------------
//  TCppClassPage: Private, non-virtual methods
// ---------------------------------------------------------------------------

//
//  Manually gen up definitions for some methods that we automatically figure out
//  when to document based on class config.
//
tCIDLib::TVoid TCppClassPage::LoadMixinMethods()
{
    if (s_bStatsDone)
        return;

    s_bStatsDone = kCIDLib::True;

    // Do MDuplicable interface
    {
        s_methDuplicate.m_strName = L"pobjDuplicate";
        s_methDuplicate.m_hnDescr.SetToText
        (
            L"Allocates a duplicate of this object and returns the newly allocated "
            L"object. The caller is responsible for releasing the object at some point. "
            "See the MDuplicable mixin class."
        );
        TMethodVar& mvarDup = s_methDuplicate.m_colMethVars.objAdd(TMethodVar());

        mvarDup.m_strRetType = L"TObject";
        mvarDup.m_eRetBy = tCIDDocComp::EParmPB::Ptr;
        mvarDup.m_eAttrs = tCIDDocComp::EMethAttrs::Override | tCIDDocComp::EMethAttrs::NoDiscard;
    }

    // Do Formatable interface
    {
        s_methFormatTo.m_strName = L"FormatTo";
        s_methFormatTo.m_hnDescr.SetToText
        (
            L"Formats this object to the passed text output stream. It is up to "
            L"the class to determine the appropriate formatting scheme. See the "
            L"MFormattable mixin class"
        );
        TMethodVar& mvarDup = s_methFormatTo.m_colMethVars.objAdd(TMethodVar());
        mvarDup.m_strRetType = L"tCIDLib::TVoid";
        mvarDup.m_eRetBy = tCIDDocComp::EParmPB::Void;
        TMethodParam& methpNew = mvarDup.m_colParams.objAdd(TMethodParam());
        methpNew.m_eDir = tCIDDocComp::EParmDirs::In;
        methpNew.m_ePassBy = tCIDDocComp::EParmPB::Ref;
        methpNew.m_strName = L"strmTar";
        methpNew.m_strType = L"TTextOutStream";
        mvarDup.m_eAttrs = tCIDDocComp::EMethAttrs::Override;
    }

    // Do binary streaming interface
    {
        s_methStreamFrom.m_strName = L"StreamFrom";
        s_methStreamFrom.m_hnDescr.SetToText
        (
            L"Streams into this object the context previously streamed out by a "
            L"call to StreamTo by an object of this type. This is not called "
            L"directly, it's used by a global >> operator that is defined in "
            L"terms of the MStreamable mixin class."
        );
        TMethodVar& mvarCur = s_methStreamFrom.m_colMethVars.objAdd(TMethodVar());
        mvarCur.m_strRetType = L"tCIDLib::TVoid";
        mvarCur.m_eRetBy = tCIDDocComp::EParmPB::Void;
        TMethodParam& methpNew = mvarCur.m_colParams.objAdd(TMethodParam());
        methpNew.m_eDir = tCIDDocComp::EParmDirs::In;
        methpNew.m_ePassBy = tCIDDocComp::EParmPB::Ref;
        methpNew.m_strName = L"strmSrc";
        methpNew.m_strType = L"TBinInStream";
        mvarCur.m_eAttrs = tCIDDocComp::EMethAttrs::Override;
    }

    {
        s_methStreamTo.m_strName = L"StreamTo";
        s_methStreamTo.m_hnDescr.SetToText
        (
            L"Streams out this object to the passed binary output stream, flattening "
            L"it out for later reading back in via a call to StreamFrom. This is not "
            L"called directly, it is used by a global << operator defined in terms "
            L"of the MStreamable mixin class."
        );
        TMethodVar& mvarCur = s_methStreamTo.m_colMethVars.objAdd(TMethodVar());
        mvarCur.m_strRetType = L"tCIDLib::TVoid";
        mvarCur.m_eRetBy = tCIDDocComp::EParmPB::Void;
        TMethodParam& methpNew = mvarCur.m_colParams.objAdd(TMethodParam());
        methpNew.m_eDir = tCIDDocComp::EParmDirs::In;
        methpNew.m_ePassBy = tCIDDocComp::EParmPB::Ref;
        methpNew.m_strName = L"strmTar";
        methpNew.m_strType = L"TBinOutStream";
        mvarCur.m_eAttrs = tCIDDocComp::EMethAttrs::Override;
    }
}



// ---------------------------------------------------------------------------
//   CLASS: TNamespacePage
//  PREFIX: pg
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  TNamespacePage: Constructors and Destructor
// ---------------------------------------------------------------------------
TNamespacePage::TNamespacePage(  const   TString&   strExtTitle
                                , const TString&    strParSrcDir
                                , const TString&    strParTopic
                                , const TString&    strFileName) :

    TBasePage
    (
        strExtTitle
        , strParSrcDir
        , strParTopic
        , strFileName
        , tCIDDocComp::EPageTypes::Namespace
    )
{

}


// ---------------------------------------------------------------------------
//  TNamespacePage: Private, inherited methods
// ---------------------------------------------------------------------------
tCIDLib::TVoid
TNamespacePage::Parse(TTopic& topicParent, const TXMLTreeElement& xtnodeNS)
{
    // Push us onto the context stack for this scope
    TCtxStackJan janStack(*this);

    // First we need to get out our namespace name
    m_strName = xtnodeNS.strAttr(L"Name");

    tCIDLib::TCard4 c4At;
    const TXMLTreeElement& xtnodeDesc = xtnodeNS.xtnodeFindElement
    (
        kCIDDocComp::strXML_Desc, 0, c4At
    );
    m_hnDesc.Parse(xtnodeDesc);

    xtnodeNS.bForEach
    (
        [&](const TXMLTreeElement& xtnodeCur)
        {
            const TString& strName = xtnodeCur.strQName();
            if (strName == L"Aliases")
            {
                m_memgAliases.Parse(xtnodeCur);
            }
             else if (strName == L"Constants")
            {
            }
             else if (strName == L"Enums")
            {
                m_memgEnums.Parse(xtnodeCur);
            }
             else if (strName == L"Method")
            {
                TMethod& methNew = m_colMethods.objAdd(TMethod());
                methNew.Parse(xtnodeCur, tCIDDocComp::EMethAttrs::NoAttrs);
            }
            return kCIDLib::True;
        }
    );
}


tCIDLib::TVoid TNamespacePage::OutputContent(TTextOutStream& strmTar) const
{
    //  Push ourself into the source stack
    TCtxStackJan janStack(*this);

    // The namespace name is the title
    strmTar << L"<p><span class='PageHdr'> Namespace: "
            << m_strName << L"</span></p>";

    m_hnDesc.OutputHelpText(strmTar, kCIDLib::False);

    // Do a little divider
    strmTar << L"<div class=\"HorzDivCont\"><div class=\"HorzDiv\"></div></div>";

    // Do the aliases first if any
    m_memgAliases.OutputContent(strmTar, tCIDDocComp::EVisTypes::Count);

    // Then enums
    m_memgEnums.OutputContent(strmTar, tCIDDocComp::EVisTypes::Count);

    // And any methods we have
    if (!m_colMethods.bIsEmpty())
    {
        strmTar << L"<p><span class='SecHdr'>Namespace Methods</span></p>";
        TStreamIndentJan janIndent(&strmTar, 4);
        m_colMethods.bForEach
        (
            [this, &strmTar] (const TMethod& methCur)
            {
                methCur.OutputContent(strmTar);
                return kCIDLib::True;
            }
        );
    }
}
