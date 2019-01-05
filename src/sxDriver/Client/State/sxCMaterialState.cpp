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
#include "sxCMaterialState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Register sxCMaterialState default constructor
sxRegisterDefaultState(sxCMaterialState, sxEStateType::eMaterial);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCMaterialStateData implementation

//-------------------------------------------------------------------------------------------------------
//
sxCMaterialStateData::sxCMaterialStateData() :
    m_f4Color(0, 0, 0, 0),
    m_fAmbient(0),
    m_fDiffuse(0),
    m_fSpecular(0),
    m_fEmissive(0),
    m_fSpecularPower(0)
{
}

//-------------------------------------------------------------------------------------------------------
//
void sxCMaterialStateData::Clear()
{
    m_f4Color = sxCFloat4(0, 0, 0, 0);
    m_fAmbient = 0;
    m_fDiffuse = 0;
    m_fSpecular = 0;
    m_fEmissive = 0;
    m_fSpecularPower = 0;
}

//-------------------------------------------------------------------------------------------------------
//
void sxCMaterialStateData::Copy(sxICommandData const& a_rData)
{
    sxCMaterialStateData const& rData = (sxCMaterialStateData const&)a_rData;

    m_f4Color = rData.m_f4Color;

    m_fAmbient = rData.m_fAmbient;
    m_fDiffuse = rData.m_fDiffuse;
    m_fSpecular = rData.m_fSpecular;
    m_fEmissive = rData.m_fEmissive;
    m_fSpecularPower = rData.m_fSpecularPower;
}
