//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//                                                                                                          |
//                      Guillaume Blanc. ELB (Etranges Libellules). Lyon FRANCE. 2008.                      |
//                              ShaderX7. Cross platform rendering thread                                   |
//                                                                                                          |
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#include "pch.h"

#ifdef __sxBuildUTest

#include "sxCUTestRenderTargetState.h"
#include "sxDriver/Client/State/sxCRenderTargetState.h"
#include "sxDriver/Server/UTest/Command/sxCUTestServerCommand.h"

//-----------------------------------------------------------------------------------------------------------
//
void sxCUTestRenderTargetState::Build(  sxICommandData const& a_rData,
                                        class sxIServerDevice& a_rDevice)
{
    sxCRenderTargetStateData const& rData = (sxCRenderTargetStateData const&)a_rData;

    m_uiBuiltValue = rData.GetWidth();
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCUTestRenderTargetState::Dispatch(   sxICommandData const& a_rData,
                                            class sxIServerDevice& a_rDevice)
{
    sxCRenderTargetStateData const& rData = (sxCRenderTargetStateData const&)a_rData;

    sxUTest(m_uiBuiltValue == rData.GetWidth());
    sxCUTestCheckServerCommand::SetValueToTest(m_uiBuiltValue);
}

#endif // __sxBuildUTest
