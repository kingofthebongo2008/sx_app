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
#include "sxCCondition.h"
#include "sxCThread.h"

//-----------------------------------------------------------------------------------------------------------
//
sxCCondition::sxCCondition() :
    m_hGateSemaphore(CreateSemaphore(NULL, 1, 1, NULL)),
    m_hQueueSemaphore(CreateSemaphore(NULL, 0, INT_MAX, NULL)),
    m_uiGone(0),
    m_uiBlocked(0),
    m_uiWaiting(0)
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxCCondition::~sxCCondition()
{
    CloseHandle(m_hGateSemaphore);
    CloseHandle(m_hQueueSemaphore);
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCCondition::NotifyOne()
{
    sxUInt uiSignals = 0;

    {
        sxCMutexLocker oLock(m_oMutex);

        if(m_uiWaiting != 0)
        {
            if(m_uiBlocked == 0)
            {
                return;
            }

            ++m_uiWaiting;
            --m_uiBlocked;
            uiSignals = 1;
        }
        else
        {
            sxValidateNoText(WaitForSingleObject(m_hGateSemaphore, INFINITE) == WAIT_OBJECT_0);
            if(m_uiBlocked > m_uiGone)
            {
                if(m_uiGone != 0)
                {
                    m_uiBlocked -= m_uiGone;
                    m_uiGone = 0;
                }
                uiSignals = m_uiWaiting = 1;
                --m_uiBlocked;
            }
            else
            {
                sxValidateNoText(ReleaseSemaphore(m_hGateSemaphore, 1, NULL) != 0);
            }
        }
    }

    if(uiSignals != 0)
    {
        sxValidateNoText(ReleaseSemaphore(m_hQueueSemaphore, uiSignals, NULL) != 0);
    }
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCCondition::NotifyAll()
{
    sxUInt uiSignals = 0;

    {
        sxCMutexLocker oLock(m_oMutex);

        if(m_uiWaiting != 0)
        {
            if(m_uiBlocked == 0)
            {
                return;
            }

            m_uiWaiting += (uiSignals = m_uiBlocked);
            m_uiBlocked = 0;
        }
        else
        {
            sxValidateNoText(WaitForSingleObject(m_hGateSemaphore, INFINITE) == WAIT_OBJECT_0);

            if(m_uiBlocked > m_uiGone)
            {
                if(m_uiGone != 0)
                {
                    m_uiBlocked -= m_uiGone;
                    m_uiGone = 0;
                }
                uiSignals = m_uiWaiting = m_uiBlocked;
                m_uiBlocked = 0;
            }
            else
            {
                sxValidateNoText(ReleaseSemaphore(m_hGateSemaphore, 1, NULL) != 0);
            }
        }
    }

    if(uiSignals != 0)
    {
        sxValidateNoText(ReleaseSemaphore(m_hQueueSemaphore, uiSignals, NULL) != 0);
    }
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCCondition::EnterWait()
{
    sxValidateNoText(WaitForSingleObject(m_hGateSemaphore, INFINITE) == WAIT_OBJECT_0);
    ++m_uiBlocked;
    sxValidateNoText(ReleaseSemaphore(m_hGateSemaphore, 1, NULL) != 0);
}

//-----------------------------------------------------------------------------------------------------------
//
sxBool sxCCondition::Wait(sxUInt a_uiMilliSeconds)
{
    sxUInt uiWasWaiting;
    sxUInt uiWasGone;
    
    sxBool bRet = WaitForSingleObject(m_hQueueSemaphore, a_uiMilliSeconds) == WAIT_OBJECT_0;

    {
        sxCMutexLocker oLock(m_oMutex);

        uiWasWaiting = m_uiWaiting;
        uiWasGone = m_uiGone;

        if(uiWasWaiting != 0)
        {
            if(bRet == false)
            {
                if(m_uiBlocked != 0)
                {
                    --m_uiBlocked;
                }
                else
                {
                    ++m_uiGone; 
                }
            }
            if(--m_uiWaiting == 0)
            {
                if(m_uiBlocked != 0)
                {
                    sxValidateNoText(ReleaseSemaphore(m_hGateSemaphore, 1, NULL) != 0);
                    uiWasWaiting = 0;
                }
                else if(m_uiGone != 0)
                {
                    m_uiGone = 0;
                }
            }
        }
        else if(++m_uiGone == (UINT_MAX / 2))
        {
            sxValidateNoText(WaitForSingleObject(m_hGateSemaphore, INFINITE) == WAIT_OBJECT_0);
            m_uiBlocked -= m_uiGone;
            sxValidateNoText(ReleaseSemaphore(m_hGateSemaphore, 1, NULL) != 0);

            m_uiGone = 0;
        }
    }

    if(1 == uiWasWaiting)
    {
        for (; uiWasGone; --uiWasGone)
        {
            sxValidateNoText(WaitForSingleObject(m_hQueueSemaphore, INFINITE) == WAIT_OBJECT_0);
        }
        sxValidateNoText(ReleaseSemaphore(m_hGateSemaphore, 1, NULL) != 0);
    }

    return bRet;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Condition unit tests
#ifdef __sxBuildUTest

//-----------------------------------------------------------------------------------------------------------
// Helper class that increment a shared value inside a mutex scope
class _sxCUTestConditionTest
{
public:
    _sxCUTestConditionTest() :
        m_uiSharedValue(0),
        m_uiExecute(0),
        m_bExit(false)
    {
    }

    sxBool m_bExit;
    sxUInt m_uiSharedValue;
    sxUInt m_uiExecute;

    sxCMutex m_oMutex;
    sxCCondition m_oCondition;
};

//-----------------------------------------------------------------------------------------------------------
//
void _UTestConditionThreadFunc(void* a_pvArgument)
{
    _sxCUTestConditionTest& rConditionTest = *(_sxCUTestConditionTest*)a_pvArgument;

    // Do everything inside lock
    sxCMutexLocker oLock(rConditionTest.m_oMutex);

    // Loop until exit's asked
    while(true)
    {
        // Wait for something to do
        while(      rConditionTest.m_uiExecute == 0
                &&  rConditionTest.m_bExit != true)
        {
            rConditionTest.m_oCondition.Wait(oLock);
        }

        // Should exit now
        if(rConditionTest.m_bExit == true)
        {
            return;
        }

        // Should execute now
        if(rConditionTest.m_uiExecute != 0)
        {
            rConditionTest.m_uiExecute--;
            rConditionTest.m_uiSharedValue++;

            // Notify everyboy that an execution has finished
            rConditionTest.m_oCondition.NotifyAll();
        }
    }
}

//-----------------------------------------------------------------------------------------------------------
//
void UTestCondition()
{
    // Test basic single threaded condition
    {
        sxBool bCond = false;

        sxCMutex oMutex;
        sxCCondition oCondition;

        sxCMutexLocker oLock(oMutex);
        while(bCond == false)
        {
            sxBool bTimedWaitRes = oCondition.TimedWait(oLock, 1);

            // Should fail has no notify occurs
            sxUTest(bTimedWaitRes == false);

            // Validate condition
            bCond = true;
        }
    }

    // Test threaded notify waits
    {
        _sxCUTestConditionTest oConditionTest;

        // Create threads
        sxCThreadPtr aspThread[3] =
        {
            sxCThread::Create("UTest condition", _UTestConditionThreadFunc, &oConditionTest),
            sxCThread::Create("UTest condition", _UTestConditionThreadFunc, &oConditionTest),
            sxCThread::Create("UTest condition", _UTestConditionThreadFunc, &oConditionTest),
        };

        // Ask for execution using notify one
        {
            sxCMutexLocker oLock(oConditionTest.m_oMutex);

            // Initial value's 0
            sxUTest(oConditionTest.m_uiSharedValue == 0);

            oConditionTest.m_uiExecute++;
            oConditionTest.m_oCondition.NotifyOne();

            // m_uiSharedValue still equals 0 as mutex isn't unlocked
            sxUTest(oConditionTest.m_uiSharedValue == 0);
        }

        // Wait execution ends
        {
            sxCMutexLocker oLock(oConditionTest.m_oMutex);
            while(oConditionTest.m_uiExecute != 0)
            {
                oConditionTest.m_oCondition.Wait(oLock);
            }

            // Now m_uiSharedValue is incremented
            sxUTest(oConditionTest.m_uiSharedValue == 1);
        }

        // Ask again for more  executions using notify all
        {
            sxCMutexLocker oLock(oConditionTest.m_oMutex);
            oConditionTest.m_uiExecute += sxArraySize(aspThread);
            oConditionTest.m_oCondition.NotifyAll();

            // m_uiSharedValue still equals 1 as mutex isn't unlocked
            sxUTest(oConditionTest.m_uiSharedValue == 1);
        }

        // Wait execution ends
        {
            sxCMutexLocker oLock(oConditionTest.m_oMutex);
            while(oConditionTest.m_uiExecute != 0)
            {
                oConditionTest.m_oCondition.Wait(oLock);
            }

            // Now m_uiSharedValue is incremented sxArraySize(aspThread) time
            sxUTest(oConditionTest.m_uiSharedValue == 1 + sxArraySize(aspThread));
        }

        // Ask for exit using notify all
        {
            sxCMutexLocker oLock(oConditionTest.m_oMutex);
            oConditionTest.m_bExit = true;
            oConditionTest.m_oCondition.NotifyAll();
        }

        // Join threads
        sxForEachUInt(i, sxArraySize(aspThread))
        {
            aspThread[i]->Join();
        }
    }
}

// Register test
sxRegisterUTestCase(UTestCondition, sxERegisterSecondaryPriority::eAboveNormal, "Condition");

#endif // __sxBuildUTest
