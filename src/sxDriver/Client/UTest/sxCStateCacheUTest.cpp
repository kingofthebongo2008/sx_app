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

#include "sxDriver/Client/sxCFactorySelector.h"
#include "sxDriver/Client/sxCCommandBuffer.h"
#include "sxDriver/Client/sxCContext.h"
#include "sxDriver/Client/sxCDevice.h"
#include "sxDriver/Client/State/sxCRenderTargetState.h"
#include "sxDriver/Client/Command/UTest/sxCUTestCommand.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// State cache unit tests
#ifdef __sxBuildUTest

//-----------------------------------------------------------------------------------------------------------
//
void UTestStateCacheBind()
{
    sxCFactoryRaiiSelector oRaiiSelector(sxEGraphicApi::eUTest);

    // Instantiate a device
    sxCDevice oDevice;

    // Instantiate a context of that device
    sxCContext oContext(oDevice);

    // Instantiate a state. UTest specialization of the render target state implements specific tests.
    // Declaration is done inside the command buffer scope in order to stress command destruction policy.
    sxCRenderTargetState oState;

    // Command used to check build and dispatch coherency
    sxCUTestCheckCommand oCheckCommand;

    // Iterate through all modes
    sxForEachEnum(eMode, sxEBufferingMode)
    {
        // Change command buffer
        oContext.SetBufferingMode(eMode);

        // Do a few loops
        sxForEachUInt(uiFrame, 10)
        {
            // Enter Begin
            oContext.Begin();

            // Bind it for the first time, it should replace existing one in the cache
            sxUTest(oContext.Bind(oState));

            // Bind it again without any lock, it doesn't update the cahce
            sxUTest(oContext.Bind(oState) == false);

            // Bind the default state back
            sxUTest(oContext.Reset(sxEStateType::eRenderTarget));

            // Re-bind it, it should replace default one in the cache
            sxUTest(oContext.Bind(oState));

            // Lock for read only
            {
                sxCRenderTargetState::sxCReadOnlyLocker oLocker(oState);
            }

            // Bind it again, it doesn't update the cache either
            sxUTest(oContext.Bind(oState) == false);

            // Lock for read write now
            {
                sxCRenderTargetState::sxCReadWriteLocker oLocker(oState);
            }

            // Bind it again, it updates the cache as the state was modified
            sxUTest(oContext.Bind(oState));

            // Bind the default state back
            sxUTest(oContext.Reset(sxEStateType::eRenderTarget));

            // Lock for read write while the state isn't in the cache
            {
                sxCRenderTargetState::sxCReadWriteLocker oLocker(oState);
            }

            // Bind it again, it updates the cache as the state was modified
            sxUTest(oContext.Bind(oState));

            // Bind the default state back
            sxUTest(oContext.Reset(sxEStateType::eRenderTarget));

            // End
            oContext.End();
        }
    }
}

// Register test
sxRegisterUTestCase(UTestStateCacheBind,
                    sxERegisterSecondaryPriority::eBelowNormal,
                    "State cache bind");


//-----------------------------------------------------------------------------------------------------------
//
void UTestStateCacheFlushDispatch()
{
    sxCFactoryRaiiSelector oRaiiSelector(sxEGraphicApi::eUTest);

    // Instantiate a device
    sxCDevice oDevice;

    // Instantiate a context of that device
    sxCContext oContext(oDevice);

    // Instantiate a state. UTest specialization of the render target state implements specific tests.
    // Declaration is done inside the command buffer scope in order to stress command destruction policy.
    sxCRenderTargetState oState;

    // Command used to check build and dispatch coherency
    sxCUTestCheckCommand oCheckCommand;

    // Iterate through all modes
    sxForEachEnum(eMode, sxEBufferingMode)
    {
        // Change command buffer
        oContext.SetBufferingMode(eMode);

        // Pointer used to test sxCRenderTargetStateData instantiations
        sxCRenderTargetStateData* pLastData = NULL;

        // Do a few loops
        sxForEachUInt(uiFrame, 10)
        {
            // Enter Begin
            oContext.Begin();

            // Discarded read-write lock after queuing
            {
                sxCRenderTargetState::sxCReadWriteLocker oLocker(oState, true);
                sxUTest(oLocker.GetData().GetWidth() == 0);
                sxUTest(    pLastData == NULL
                        ||  pLastData == &oLocker.GetData());
                pLastData = &oLocker.GetData();
            }

            // Read-only lock before queuing
            {
                sxCRenderTargetState::sxCReadOnlyLocker oLocker(oState);
                sxUTest(pLastData == &oLocker.GetData());
                sxUTest(oLocker.GetData().GetWidth() == 0);
            }

            // Read-write lock before queuing
            {
                sxCRenderTargetState::sxCReadWriteLocker oLocker(oState);
                sxUTest(pLastData == &oLocker.GetData());
                oLocker.GetData().SetWidth(uiFrame + 17);
            }

            // Read-only lock before queuing,after a write
            {
                sxCRenderTargetState::sxCReadOnlyLocker oLocker(oState);
                sxUTest(pLastData == &oLocker.GetData());
                sxUTest(oLocker.GetData().GetWidth() == uiFrame + 17);
            }

            // Bind the state
            sxUTest(oContext.Bind(oState));

            // Lock and queue the command that'll check oState results
            {
                sxCRenderTargetState::sxCReadWriteLocker oCheckLocker(oCheckCommand);
                oCheckLocker.GetData().SetWidth(uiFrame + 17);
            }
            oContext.Queue(oCheckCommand);

            // Read-only lock after queuing
            {
                sxCRenderTargetState::sxCReadOnlyLocker oLocker(oState);
                sxUTest(pLastData == &oLocker.GetData());
                sxUTest(oLocker.GetData().GetWidth() == uiFrame + 17);
            }

            // Unbind the state
            sxUTest(oContext.Reset(sxEStateType::eRenderTarget));

            // Un-discarded read-write lock after queuing
            {
                sxCRenderTargetState::sxCReadWriteLocker oLocker(oState, false);
                sxUTest(    pLastData == &oLocker.GetData()
                        ||  eMode != sxEBufferingMode::eNoBuffering);
                pLastData = &oLocker.GetData();
                sxUTest(oLocker.GetData().GetWidth() == uiFrame + 17);
                oLocker.GetData().SetWidth(uiFrame + 71);
            }

            // Un-discarded read-write lock again after an un-discarded write
            {
                sxCRenderTargetState::sxCReadWriteLocker oLocker(oState, false);
                sxUTest(pLastData == &oLocker.GetData());
                sxUTest(oLocker.GetData().GetWidth() == uiFrame + 71);
                pLastData = &oLocker.GetData();
            }

            // Bind the state
            sxUTest(oContext.Bind(oState));
            sxUTest(oContext.Bind(oState) == false);

            // Lock and queue the command that'll check oState results
            {
                sxCRenderTargetState::sxCReadWriteLocker oCheckLocker(oCheckCommand);
                oCheckLocker.GetData().SetWidth(uiFrame + 71);
            }
            oContext.Queue(oCheckCommand);

            // Discarded read-write lock again after un-discarded write
            {
                sxCRenderTargetState::sxCReadWriteLocker oLocker(oState, true);
                sxUTest(oLocker.GetData().GetWidth() == 0);
                sxUTest(pLastData != &oLocker.GetData());
                pLastData = &oLocker.GetData();
            }

            // Unbind the state after the state was modified
            sxUTest(oContext.Reset(sxEStateType::eRenderTarget));

            // Discarded read-write lock again after the unbind
            {
                sxCRenderTargetState::sxCReadWriteLocker oLocker(oState, true);
                sxUTest(oLocker.GetData().GetWidth() == 0);
                sxUTest(    pLastData == &oLocker.GetData()
                        ||  eMode == sxEBufferingMode::eNoBuffering);
                pLastData = &oLocker.GetData();
            }

            // Reset check command also
            sxCRenderTargetState::sxCReadWriteLocker oLocker(oCheckCommand, true);

            // End
            oContext.End();
        }
    }
}

// Register test
sxRegisterUTestCase(UTestStateCacheFlushDispatch,
                    sxERegisterSecondaryPriority::eBelowNormal,
                    "State cache flush dispatch");

#endif // __sxBuildUTest
