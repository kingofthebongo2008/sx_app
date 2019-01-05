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
#include "sxCTime.h"

// Need window performance counter inclusion
#include <windows.h>

class sxCWindowsPerformanceCounter : public sxITSingleton<sxCWindowsPerformanceCounter>
{
public:
    //----------------------------------------------------------------------------------------------------------
    // Constructor. Initialize performance counter frequency and initial time.
    sxCWindowsPerformanceCounter()
    {
        // Get counter frequency
        sxInt64 i64Frequency;
        sxValidateNoText(QueryPerformanceFrequency((LARGE_INTEGER*) &i64Frequency) != 0);

        // Compute inverse frequency in seconds (forcing double precision)
        m_dInvNsFrequency = 1.0 / sxDouble(i64Frequency);

        // Get initial counter value
        sxValidateNoText(QueryPerformanceCounter((LARGE_INTEGER*) &m_i64InitialCounter) != 0);
    }

    //----------------------------------------------------------------------------------------------------------
    // Destructor.
    ~sxCWindowsPerformanceCounter()
    {
    }

    //----------------------------------------------------------------------------------------------------------
    // Compute current time since application boot
    sxFloat ComputeApplicationTime() const
    {
        // GetCurrent counter
        sxInt64 i64CurrentCounter;
        sxValidateNoText(QueryPerformanceCounter((LARGE_INTEGER*) &i64CurrentCounter) != 0);

        // Compute delta counter value
        sxDouble const dDeltaCounter = sxDouble(i64CurrentCounter - m_i64InitialCounter);

        // Compute delta time
        sxDouble const dDeltaTime = dDeltaCounter * m_dInvNsFrequency;

        // Return a sxFloat result
        return sxFloat(dDeltaTime);
    }

protected:

    // Performance counter frequency
    sxDouble m_dInvNsFrequency;

    // Initial performance counter
    sxInt64 m_i64InitialCounter;
};

//-----------------------------------------------------------------------------------------------------------
// Register performance counter such as it is initialized during application boot
sxRegisterSingleton(sxCWindowsPerformanceCounter,
                    sxERegisterPrimaryPriority::eHighest,
                    sxERegisterSecondaryPriority::eHighest);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCTime implementation

//-----------------------------------------------------------------------------------------------------------
//
sxCTime sxCTime::GetApplicationTime()
{
    // Compute time using the windows performance counter
    sxFloat fTimeInSeconds = sxCWindowsPerformanceCounter::GetInstance().ComputeApplicationTime();

    // Build a sxCTime object
    return sxCTime(fTimeInSeconds);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCFpsCounter implementation

//-----------------------------------------------------------------------------------------------------------
//
sxCFpsCounter::sxCFpsCounter(sxFloat a_fMeanTime) :
    m_fCumulatedTime(0),
    m_fMeanTime(a_fMeanTime),
    m_uiFrameCount(0),
    m_uiFramePerSecond(0)
{}

//-----------------------------------------------------------------------------------------------------------
//
void sxCFpsCounter::Update(sxFloat a_fFrameTime)
{
    // Incrment internal values
    m_fCumulatedTime += a_fFrameTime;
    m_uiFrameCount++;

    // Has the mean time elapsed ?
    if(m_fCumulatedTime > m_fMeanTime)
    {
        // Compute fps now
        m_uiFramePerSecond = sxUInt(sxFloat(m_uiFrameCount) / m_fCumulatedTime);

        // Reset counters
        m_uiFrameCount = 1;
        m_fCumulatedTime = a_fFrameTime;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCFpsAutoCounter implementation

//-----------------------------------------------------------------------------------------------------------
//
sxCFpsAutoCounter::sxCFpsAutoCounter(sxFloat a_fMeanTime) :
    sxCFpsCounter(a_fMeanTime),
    m_fLastTime(0)
{
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCFpsAutoCounter::Update()
{
    // Get the current time
    sxFloat fCurrentTime = sxCTime::GetApplicationTime().GetSeconds();

    // Compute elapsed time since last update
    sxFloat fDeltaTime = fCurrentTime - m_fLastTime;

    // Update counter
    sxCFpsCounter::Update(fDeltaTime);

    // Copy last time
    m_fLastTime = fCurrentTime;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread unit tests
#ifdef __sxBuildUTest

//-----------------------------------------------------------------------------------------------------------
//
void UTestTime()
{
    sxCTime const oBeginTime = sxCTime::GetApplicationTime();
    sxCTime oZeroTime;

    sxUTest(oZeroTime.GetSeconds() == 0.f);
    sxUTest(oBeginTime.GetSeconds() > 0.f);
    sxUTest(oBeginTime.GetSeconds() < sxCTime::GetApplicationTime().GetSeconds());
}

// Register test
sxRegisterUTestCase(UTestTime, sxERegisterSecondaryPriority::eHigh, "Time");

//-----------------------------------------------------------------------------------------------------------
//
void UTestFpsCounter()
{
    sxFloat const fMeanTime = 3.f;
    sxFloat const fFrameTime = 0.01f;
    sxUInt const uiRequiredLoops = sxUInt(fMeanTime / fFrameTime);
    sxUInt const uiEstimatedFps = sxUInt(1.f/ fFrameTime);

    // Initialize a counter with fMeanTime mean time
    sxCFpsCounter oCounter(fMeanTime);

    // Update counter with fFrameTime
    sxForEachUInt(i, uiRequiredLoops + 1)
    {
        oCounter.Update(fFrameTime);
    }

    sxUTest(oCounter.GetFramePerSecond() == uiEstimatedFps);

    // Do a second loop, update counter with fFrameTime
    sxForEachUInt(i, uiRequiredLoops)
    {
        oCounter.Update(fFrameTime);
    }

    sxUTest(oCounter.GetFramePerSecond() == uiEstimatedFps);
}

// Register test
sxRegisterUTestCase(UTestFpsCounter, sxERegisterSecondaryPriority::eHigh, "FpsCounter");

#endif // __sxBuildUTest
