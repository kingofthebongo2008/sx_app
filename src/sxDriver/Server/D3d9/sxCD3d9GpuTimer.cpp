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
#include "sxCD3d9GpuTimer.h"
#include "sxCD3d9Device.h"

//-------------------------------------------------------------------------------------------------------
//
sxCD3d9GpuTimer::sxCD3d9GpuTimer() :
    m_bIssued(false),
    m_bValid(false),
    m_bInsideBeginEnd(false),
    m_fElpasedTime(0)
{
}

//-------------------------------------------------------------------------------------------------------
//
sxCD3d9GpuTimer::~sxCD3d9GpuTimer()
{
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9GpuTimer::Begin()
{
    // Test entry condition
    sxAssert(m_bInsideBeginEnd == false, "Timer has already begin");

    // Flag that the timer has began
    m_bInsideBeginEnd = true;

    // Early out if queries aren't valid
    if(m_bValid == false)
    {
        return;
    }

    // If queries were already issued, then try to get the result
    if(m_bIssued != false)
    {
        sxUInt64 ui64DataBegin = 0, ui64DataEnd = 0, ui64DataFreq = 0;

        // Try to get the results
        if(     m_spTimeStampQueryFreq->GetData(&ui64DataFreq, sizeof(ui64DataFreq), 0) == S_OK
            &&  m_spTimeStampQueryBegin->GetData(&ui64DataBegin, sizeof(ui64DataBegin), 0) == S_OK
            &&  m_spTimeStampQueryEnd->GetData(&ui64DataEnd, sizeof(ui64DataEnd), 0) == S_OK)
        {
            // Compute delta time
            sxUInt64 ui64DeltaTime = ui64DataEnd - ui64DataBegin;

            // Compute elapsed time in seconds
            m_fElpasedTime = sxFloat(sxDouble(ui64DeltaTime) / sxDouble(ui64DataFreq));

            // The results are there, so asked for re-issuing again
            m_bIssued = false;
        }
    }
    
    // If queries aren't issued yet, then start issuing
    if(m_bIssued == false)
    {
        m_spTimeStampQueryFreq->Issue(D3DISSUE_END);
        m_spTimeStampQueryBegin->Issue(D3DISSUE_END);
    }
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9GpuTimer::End()
{
    // Test entry condition
    sxAssert(m_bInsideBeginEnd == true, "Timer Begin hasn't been called");

    // Flag that the timer has ended
    m_bInsideBeginEnd = false;

    // Early out if queries aren't valid
    if(m_bValid == false)
    {
        return;
    }
    
    // Should query be issued
    if(m_bIssued == false)
    {
        m_spTimeStampQueryEnd->Issue(D3DISSUE_END);

        // Every query is issued
        m_bIssued = true;
    }
}

//-------------------------------------------------------------------------------------------------------
//
sxFloat sxCD3d9GpuTimer::GetElapsedTime() const
{
    // Test entry condition
    sxAssert(IsValid(), "Cannot be called if timer isn't valid");

    return m_fElpasedTime;
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9GpuTimer::PreReset()
{
    // Test entry condition
    sxAssertNoText(m_bInsideBeginEnd == false);

    // Release queries
    m_spTimeStampQueryBegin = NULL;
    m_spTimeStampQueryEnd = NULL;
    m_spTimeStampQueryFreq = NULL;

    // Invalidate the timer
    m_bValid = false;
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9GpuTimer::PostResetSucceed()
{
    // Test entry condition
    sxAssertNoText(m_bValid == false && m_bInsideBeginEnd == false);

    //----------------
    // (Re)build queries

    // Fetch device
    IDirect3DDevice9& rDevice = sxCD3d9Device::GetInstance().GetD3dDevice();

    // Build "Begin" time stamp query
    HRESULT hRes = rDevice.CreateQuery( D3DQUERYTYPE_TIMESTAMP, sxGetD3d9SmartModifier(m_spTimeStampQueryBegin));
    if(FAILED(hRes) == true)
    {
        return;
    }

    // Build "End" time stamp query
    hRes = rDevice.CreateQuery( D3DQUERYTYPE_TIMESTAMP, sxGetD3d9SmartModifier(m_spTimeStampQueryEnd));
    if(FAILED(hRes) == true)
    {
        return;
    }

    // Build time stamp frequency query
    hRes = rDevice.CreateQuery( D3DQUERYTYPE_TIMESTAMPFREQ, sxGetD3d9SmartModifier(m_spTimeStampQueryFreq));
    if(FAILED(hRes) == true)
    {
        return;
    }

    // Every query is created, then timer is valid
    m_bValid = true;
}