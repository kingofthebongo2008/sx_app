//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//																											|
//								ShaderX7. Cross platform rendering thread. 2008.							|
//				Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.			|
//																											|
//	This program is free software. It is distributed in the hope that it will be useful, but without any	|
//	warranty, without even the implied warranty of merchantability or fitness for a particular purpose.		|
//																											|
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#include "sxKernel/sxPcHeader.h"
#include "sxCUTestCommand.h"
#include "sxDriver/Client/sxCFactorySelector.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxICommand unit tests
#ifdef __sxBuildUTest

//-----------------------------------------------------------------------------------------------------------
//
void UTestCommandLocking()
{
	sxCFactoryRaiiSelector oRaiiSelector(sxEGraphicApi::eUTest);

	sxCUTestCommand oCommand;

	// Simple read-only lock
	{
		sxCUTestCommand::sxCReadOnlyLocker oLocker(oCommand);
	}

	// Simple read-write lock
	{
		sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand);
	}

	// Multiple read-only locks
	{
		sxCUTestCommand::sxCReadOnlyLocker oLocker1(oCommand);
		sxCUTestCommand::sxCReadOnlyLocker oLocker2(oCommand);
		sxCUTestCommand::sxCReadOnlyLocker oLocker3(oCommand);
		sxCUTestCommand::sxCReadOnlyLocker oLocker4(oCommand);
	}

	// Mixed read-only and read-write locks
	{
		sxCUTestCommand::sxCReadWriteLocker oLocker1(oCommand);
		sxUTestAssert(sxCUTestCommand::sxCReadWriteLocker oLocker2(oCommand));
		sxUTestAssert(sxCUTestCommand::sxCReadOnlyLocker oLocker3(oCommand));
	}
}

// Register test
sxRegisterUTestCase(UTestCommandLocking, sxERegisterSecondaryPriority::eNormal, "Command locking");

//-----------------------------------------------------------------------------------------------------------
//
void UTestCommandDataIntegrity()
{
	sxCFactoryRaiiSelector oRaiiSelector(sxEGraphicApi::eUTest);
	sxCUTestCommand oCommand;

	// Simple read
	{
		sxCUTestCommand::sxCReadOnlyLocker oLocker(oCommand);
		sxCUTestCommandData const& rData = oLocker.GetData();

		// Test default value
		sxUTest(rData.GetValue() == 0);
	}

	// Simple write then read
	{
		// Modify the data
		{
			sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand);
			sxCUTestCommandData& rData = oLocker.GetData();

			// Test default value
			sxUTest(rData.GetValue() == 0);
			rData.SetValue(12);
			sxUTest(rData.GetValue() == 12);
		}

		// Ensure the data is modified when accessed with reads
		{
			sxCUTestCommand::sxCReadOnlyLocker oLocker(oCommand);
			sxCUTestCommandData const& rData = oLocker.GetData();

			// Test default value
			sxUTest(rData.GetValue() == 12);
		}
	}

	// If the data is modified again, then its value is reseted to default as "Discard" is the default 
	// behavior
	{
		sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand);
		sxCUTestCommandData& rData = oLocker.GetData();

		// Test default value
		sxUTest(rData.GetValue() == 0);
		rData.SetValue(12);
		sxUTest(rData.GetValue() == 12);
	}

	// If the data is modified again, then its value is not reseted if "Discard" flag is false
	{
		sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand, false);
		sxCUTestCommandData& rData = oLocker.GetData();

		// Test default value
		sxUTest(rData.GetValue() == 12);
		rData.SetValue(21);
		sxUTest(rData.GetValue() == 21);
	}

	// Even if data is acquired somewhere in the driver for read-only
	{
		sxCUTestCommand::sxCReadOnlyLocker oReadLocker(oCommand);
		sxCUTestCommand::sxCReadWriteLocker oLocker(oCommand, false);
		sxCUTestCommandData& rData = oLocker.GetData();

		// Test default value
		sxUTest(rData.GetValue() == 21);
		rData.SetValue(12);
		sxUTest(rData.GetValue() == 12);
	}
}

// Register test
sxRegisterUTestCase(UTestCommandDataIntegrity, sxERegisterSecondaryPriority::eNormal, "Command data integrity");

#endif // __sxBuildUTest
