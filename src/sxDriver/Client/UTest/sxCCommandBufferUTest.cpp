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
#include "sxDriver/Client/Command/UTest/sxCUTestCommand.h"
#include "sxDriver/Server/Interface/sxIServerCommand.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Command buffer unit tests
#ifdef __sxBuildUTest

//-----------------------------------------------------------------------------------------------------------
//
void UTestCommandBufferModes()
{
    // Select the UTest factory
    sxCFactoryRaiiSelector oRaiiSelector(sxEGraphicApi::eUTest);

    // Instantiate a device
    sxCDevice oDevice;

    // Iterate through all modes
    sxForEachEnum(eMode, sxEBufferingMode)
    {
        // Construct a command buffer to run on the device
        sxCCommandBuffer oCommandBuffer(oDevice, eMode);

        // Change mode dynamically
        sxForEachEnum(eNewMode, sxEBufferingMode)
        {
            // Simulate many frames
            sxForEachUInt(i, 10)
            {
                oCommandBuffer.Begin();
                oCommandBuffer.End();
            }

            // Change mode
            oCommandBuffer.SetBufferingMode(eNewMode);

            // Changing modes should fail inside a begin end
            oCommandBuffer.Begin();
            sxUTestAssert(oCommandBuffer.SetBufferingMode(eNewMode));
            oCommandBuffer.End();
        }
    }
}

// Register test
sxRegisterUTestCase(UTestCommandBufferModes, sxERegisterSecondaryPriority::eNormal, "Command buffer modes");

//-----------------------------------------------------------------------------------------------------------
//
void UTestCommandBufferBeginEnd()
{
    // Select the UTest factory
    sxCFactoryRaiiSelector oRaiiSelector(sxEGraphicApi::eUTest);

    // Instantiate a device
    sxCDevice oDevice;

    // Iterate through all modes
    sxForEachEnum(eMode, sxEBufferingMode)
    {
        // Construct a command buffer to run on the device
        sxCCommandBuffer oCommandBuffer(oDevice, eMode);

        // Test begin/end pairs and assertions
        oCommandBuffer.Begin();
        sxUTestAssert(oCommandBuffer.Begin());
        oCommandBuffer.End();
        sxUTestAssert(oCommandBuffer.End());

        // Do a few fast loops
        sxForEachUInt(i, 100)
        {
            oCommandBuffer.Begin();
            oCommandBuffer.End();
        }

        // Do a few slow loops
        sxForEachUInt(i, 100)
        {
            oCommandBuffer.Begin();
            sxCThread::Sleep(1);
            oCommandBuffer.End();
        }
    }
}

// Register test
sxRegisterUTestCase(UTestCommandBufferBeginEnd, sxERegisterSecondaryPriority::eNormal, "Command buffer begin-end");

//-----------------------------------------------------------------------------------------------------------
//
void UTestCommandBufferQueue()
{
    // Test queuing behavior through the use of a fake server command object

    // Select the UTest factory
    sxCFactoryRaiiSelector oRaiiSelector(sxEGraphicApi::eUTest);

    // Instantiate a device
    sxCDevice oDevice;

    // Local command data class
    class _sxCUTestCommandData : public sxICommandData
    {
        virtual void Clear() {}
        virtual void Copy(sxICommandData const& a_rData) {}
    };
        
    // Local server command class
    class _sxCUTestServerCommand : public sxIServerCommand
    {
    private:
        virtual void Dispatch(  sxICommandData const& a_rData,
                                class sxIServerDevice& a_rDevice)
        {
        }
    } ;

    // Locker holder to simulate a driver object
    class _sxCTUTestDriverHolder : public sxCReadOnlyLockerHolder
    {
    public:
        _sxCTUTestDriverHolder( sxIServerCommand& a_rServerCommand,
                                sxICommandData const& a_CommandData) :
            sxCReadOnlyLockerHolder(a_rServerCommand, a_CommandData)
        {
        }
    } oDriverHolder(sxNew(_sxCUTestServerCommand), sxNew(_sxCUTestCommandData));

    // Construct a command buffer
    class sxCUTestCommandBuffer : public sxCCommandBuffer
    {
    public:
        sxCUTestCommandBuffer(sxCDevice& a_rDevice, _sxCTUTestDriverHolder& a_rDriverHolder) :
            sxCCommandBuffer(a_rDevice)
        {
            // Iterate through all modes
            sxForEachEnum(eMode, sxEBufferingMode)
            {
                // Change command buffer
                SetBufferingMode(eMode);

                // Do a few loops
                sxForEachUInt(uiFrame, 10)
                {
                    // Queue outside of begin end pair must fail
                    sxUTestAssert(Queue(a_rDriverHolder));

                    // Begin queuing
                    Begin();

                    // Now it should be valid
                    Queue(a_rDriverHolder);

                    // End queuing
                    End();
                }
            }
        }
    } oUTestCommandBuffer(oDevice, oDriverHolder);
}

// Register test
sxRegisterUTestCase(UTestCommandBufferQueue, sxERegisterSecondaryPriority::eNormal, "Command buffer queue");

//-----------------------------------------------------------------------------------------------------------
//
void UTestCommandBufferAcquireOnQueue()
{
    // Select the UTest factory
    sxCFactoryRaiiSelector oRaiiSelector(sxEGraphicApi::eUTest);

    // Instantiate a device
    sxCDevice oDevice;

    // Instantiate a context of that device
    sxCContext oContext(oDevice);

    // Instantiate a command. This is done inside the command buffer scope in order to stress command 
    // destruction policy
    sxCUTestCommand oCommand;

    // Pointer used to test CommandData instantiations
    sxICommandData* pLastData = NULL;

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

            // Discarded read-write lock after queuing
            {
                sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand, true);
                sxUTest(oLocker.GetData().GetValue() == 0);

                // Initialize Last data
                pLastData = &oLocker.GetData();
            }

            // Read-only lock before queuing
            {
                sxCUTestCommand::sxCReadOnlyLocker oLocker(oCommand);
                sxUTest(oLocker.GetData().GetValue() == 0);
                sxUTest(pLastData == &oLocker.GetData());
            }

            // Read-write lock before queuing
            {
                sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand);
                oLocker.GetData().SetValue(uiFrame + 17);
                sxUTest(pLastData == &oLocker.GetData());
            }

            // Read-only lock before queuing,after a write
            {
                sxCUTestCommand::sxCReadOnlyLocker oLocker(oCommand);
                sxUTest(oLocker.GetData().GetValue() == uiFrame + 17);
                sxUTest(pLastData == &oLocker.GetData());
            }

            // Queue the command
            oContext.Queue(oCommand);

            // Read-only lock after queuing
            {
                sxCUTestCommand::sxCReadOnlyLocker oLocker(oCommand);
                sxUTest(oLocker.GetData().GetValue() == uiFrame + 17);
                sxUTest(pLastData == &oLocker.GetData());
            }

            // Un-discarded read-write lock after queuing
            {
                sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand, false);
                sxUTest(oLocker.GetData().GetValue() == uiFrame + 17);
                oLocker.GetData().SetValue(uiFrame + 71);

                // Data is modified after being queued, so it is duplicated
                sxUTest(eMode == sxEBufferingMode::eNoBuffering
                        ||  pLastData != &oLocker.GetData());
                pLastData = &oLocker.GetData();
            }

            // Un-discarded read-write lock again after an un-discarded write
            {
                sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand, false);
                sxUTest(oLocker.GetData().GetValue() == uiFrame + 71);
                sxUTest(pLastData == &oLocker.GetData());
            }

            // Queue the command again
            oContext.Queue(oCommand);

            // Discarded read-write lock again after un-discarded write
            {
                sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand, true);
                sxUTest(oLocker.GetData().GetValue() == 0);

                // Data is modified after being queued, so it is duplicated
                sxUTest(eMode == sxEBufferingMode::eNoBuffering
                        ||  pLastData != &oLocker.GetData());
            }
            // End
            oContext.End();
        }
    }
}

// Register test
sxRegisterUTestCase(UTestCommandBufferAcquireOnQueue,
                    sxERegisterSecondaryPriority::eNormal,
                    "Command buffer acquire on queue");

//-----------------------------------------------------------------------------------------------------------
//
void UTestCommandBufferDispatch()
{
    sxCFactoryRaiiSelector oRaiiSelector(sxEGraphicApi::eUTest);

    // Instantiate a device
    sxCDevice oDevice;

    // Instantiate a context of that device
    sxCContext oContext(oDevice);

    // Instantiate a command. This is done inside the command buffer scope in order to stress command 
    // destruction policy
    sxCUTestCommand oCommand;

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

            // Discarded read-write lock after queuing
            {
                sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand, true);
                sxUTest(oLocker.GetData().GetValue() == 0);
            }

            // Read-only lock before queuing
            {
                sxCUTestCommand::sxCReadOnlyLocker oLocker(oCommand);
                sxUTest(oLocker.GetData().GetValue() == 0);
            }

            // Read-write lock before queuing
            {
                sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand);
                oLocker.GetData().SetValue(uiFrame + 17);
            }

            // Read-only lock before queuing,after a write
            {
                sxCUTestCommand::sxCReadOnlyLocker oLocker(oCommand);
                sxUTest(oLocker.GetData().GetValue() == uiFrame + 17);
            }

            // Queue the command
            oContext.Queue(oCommand);

            // Lock and queue the command that'll check oCommand results
            {
                sxCUTestCommand::sxCReadWriteLocker oCheckLocker(oCheckCommand);
                oCheckLocker.GetData().SetValue(uiFrame + 17);
            }
            oContext.Queue(oCheckCommand);

            // Read-only lock after queuing
            {
                sxCUTestCommand::sxCReadOnlyLocker oLocker(oCommand);
                sxUTest(oLocker.GetData().GetValue() == uiFrame + 17);
            }

            // Un-discarded read-write lock after queuing
            {
                sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand, false);
                sxUTest(oLocker.GetData().GetValue() == uiFrame + 17);
                oLocker.GetData().SetValue(uiFrame + 71);
            }

            // Un-discarded read-write lock again after an un-discarded write
            {
                sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand, false);
                sxUTest(oLocker.GetData().GetValue() == uiFrame + 71);
            }

            // Queue the command again
            oContext.Queue(oCommand);

            // Lock and queue the command that'll check oCommand results
            {
                sxCUTestCommand::sxCReadWriteLocker oCheckLocker(oCheckCommand);
                oCheckLocker.GetData().SetValue(uiFrame + 71);
            }
            oContext.Queue(oCheckCommand);

            // Discarded read-write lock again after un-discarded write
            {
                sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand, true);
                sxUTest(oLocker.GetData().GetValue() == 0);
            }

            // Reset check command also
            sxCUTestCommand::sxCReadWriteLocker oLocker(oCheckCommand, true);

            // End
            oContext.End();
        }
    }
}

// Register test
sxRegisterUTestCase(UTestCommandBufferDispatch,
                    sxERegisterSecondaryPriority::eNormal,
                    "Command buffer dispatch");

#endif // __sxBuildUTest
