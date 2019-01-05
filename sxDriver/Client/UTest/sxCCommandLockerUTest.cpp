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

#include "sxDriver/Client/sxCCommandLocker.h"
#include "sxDriver/Client/sxICommand.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// _sxCCommandLocker unit tests
#ifdef __sxBuildUTest

//-----------------------------------------------------------------------------------------------------------
//
void UTestCommandLocker()
{
	// Local command data
	class _sxCUTestCommandData : public sxICommandData
	{
		virtual void Clear() {}
		virtual void Copy(sxICommandData const& a_rData) {}
	} oData;

	// Inherit from _sxCCommandLocker in order to get access to acuire-relase functions
	class sxCUTestLocker : protected _sxCCommandLocker
	{
	public:
		//---------------------------------------------------------------------------------------------------
		// Constructor that implement all tests
		sxCUTestLocker(_sxCUTestCommandData& a_rData)
		{
			// Read-only lock
			sxUTestAssert(_sxCCommandLocker::ReleaseReadOnlyAccess(a_rData));
			_sxCCommandLocker::AcquireReadOnlyAccess(a_rData);
			_sxCCommandLocker::ReleaseReadOnlyAccess(a_rData);
			sxUTestAssert(_sxCCommandLocker::ReleaseReadOnlyAccess(a_rData));

			// Read-write lock
			sxUTestAssert(_sxCCommandLocker::ReleaseReadWriteAccess(a_rData));
			_sxCCommandLocker::AcquireReadWriteAccess(a_rData);
			sxUTestAssert(_sxCCommandLocker::AcquireReadWriteAccess(a_rData));
			_sxCCommandLocker::ReleaseReadWriteAccess(a_rData);
			sxUTestAssert(_sxCCommandLocker::ReleaseReadWriteAccess(a_rData));

			// Multiple read-write lock
			_sxCCommandLocker::AcquireReadOnlyAccess(a_rData);
			sxUTestAssert(_sxCCommandLocker::AcquireReadWriteAccess(a_rData));
			_sxCCommandLocker::AcquireReadOnlyAccess(a_rData);
			sxUTestAssert(_sxCCommandLocker::AcquireReadWriteAccess(a_rData));
			_sxCCommandLocker::ReleaseReadOnlyAccess(a_rData);
			_sxCCommandLocker::ReleaseReadOnlyAccess(a_rData);
			sxUTestAssert(_sxCCommandLocker::ReleaseReadWriteAccess(a_rData));
			_sxCCommandLocker::AcquireReadWriteAccess(a_rData);
			_sxCCommandLocker::ReleaseReadWriteAccess(a_rData);
		}
	} oLocker(oData);
}

// Register test
sxRegisterUTestCase(UTestCommandLocker, sxERegisterSecondaryPriority::eNormal, "Command locker");

#endif // __sxBuildUTest
