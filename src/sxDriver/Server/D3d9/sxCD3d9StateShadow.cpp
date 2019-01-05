//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//                                                                                                          |
//                              ShaderX7. Cross platform rendering thread. 2008.                            |
//              Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.         |
//                                                                                                          |
//  This program is free software. It is distributed in the hope that it will be useful, but without any    |
//  warranty, without even the implied warranty of merchantability or fitness for a particular purpose.     |
//                                                                                                          |
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#include "pch.h"
#include "sxCD3d9StateShadow.h"

//-----------------------------------------------------------------------------------------------------------
//
sxCD3d9StateShadow::sxCD3d9StateShadow() :
    m_vpServerStates(sxEStateType::eValuesCount),
    m_vpServerStateData(sxEStateType::eValuesCount)
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxCD3d9StateShadow::~sxCD3d9StateShadow()
{
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCD3d9StateShadow::SetState(  sxEStateType::sxEValue a_eStateType,
                                    sxIServerObject const& a_rState,
                                    sxICommandData const& a_rData)
{
    m_vpServerStates[a_eStateType] = &a_rState;
    m_vpServerStateData[a_eStateType] = &a_rData;
}

//-----------------------------------------------------------------------------------------------------------
//
sxIServerObject const& sxCD3d9StateShadow::GetState(sxEStateType::sxEValue a_eStateType) const
{
    return sxDereference(m_vpServerStates[a_eStateType]);
}

//-----------------------------------------------------------------------------------------------------------
//
sxICommandData const& sxCD3d9StateShadow::GetStateData(sxEStateType::sxEValue a_eStateType) const
{
    return sxDereference(m_vpServerStateData[a_eStateType]);
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCD3d9StateShadow::Release()
{
    // Release all references
    sxForEachEnum(eValue, sxEStateType)
    {
        m_vpServerStates[eValue] = NULL;
        m_vpServerStateData[eValue] = NULL;
    }
}
