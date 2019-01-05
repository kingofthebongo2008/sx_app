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

#ifdef __sxBuildUTest

#include "sxUnitTest.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCUTestManager implementation

// Register UTest manager singleton object
sxRegisterSingleton(sxCUTestManager,
					sxERegisterPrimaryPriority::eNormal,
					sxERegisterSecondaryPriority::eNormal);

// Register UTest manager Start function
sxRegisteredSingletonFunction(	sxCUTestManager,
								&sxCUTestManager::Start,
								sxERegisterPrimaryPriority::eUnitTest,
								sxERegisterSecondaryPriority::eHighest);

// Register UTest manager Finish function
sxRegisteredSingletonFunction(	sxCUTestManager,
								&sxCUTestManager::Finish,
								sxERegisterPrimaryPriority::eUnitTest,
								sxERegisterSecondaryPriority::eLowest);

//-----------------------------------------------------------------------------------------------------------
//
sxCUTestManager::sxCUTestManager() :
	m_uiSuccedeedTestCount(0),
	m_uiFailedTestCount(0)
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxCUTestManager::~sxCUTestManager()
{
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCUTestManager::Start(sxERegisteredOperation::EValue a_eOp)
{
	if(a_eOp == sxERegisteredOperation::eInitialize)
	{
		sxLog("Starting unit tests");
	}
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCUTestManager::Finish(sxERegisteredOperation::EValue a_eOp)
{
	if(a_eOp == sxERegisteredOperation::eInitialize)
	{
		if(m_uiFailedTestCount == 0)
		{
			sxLog(	"Ending unit tests with success, %d test were executed",
					m_uiSuccedeedTestCount);
		}
		else
		{
			sxLog(	"Ending unit tests with %d failure(s), %d test were executed",
					m_uiFailedTestCount,
					m_uiSuccedeedTestCount + m_uiFailedTestCount);
		}
		// Jump a line
		sxLog("");
	}
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCUTestManager::NotifyEndedTest(sxCUTestCase& a_rTestCase, sxBool a_bSucceded)
{
	// Log test result
	sxLog(	" - test %d (%s) : %s", 
			m_uiSuccedeedTestCount + m_uiFailedTestCount,
			a_rTestCase.GetName(),
			a_bSucceded? "OK" : "FAILED");

	// Increment test counts
	(a_bSucceded == true)? m_uiSuccedeedTestCount++ : m_uiFailedTestCount++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCUTestCase implementation

//-----------------------------------------------------------------------------------------------------------
//
sxCUTestCase::sxCUTestCase(	sxPfnUTestFunction a_pFunction,
							sxChar const* a_szName,
							sxERegisterSecondaryPriority::EValue a_eSecondaryPriority) :
	sxIRegisteredObject(sxERegisterPrimaryPriority::eUnitTest, a_eSecondaryPriority),
	m_pFunction(a_pFunction),
	m_szName(a_szName)
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxCUTestCase::~sxCUTestCase()
{
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCUTestCase::operator()(sxERegisteredOperation::EValue a_eOp)
{
	// Only execute test case on sxERegisteredOperation::eInitialize
	if(a_eOp == sxERegisteredOperation::eInitialize)
	{
		// Has the test succeeded
		sxBool bSucceed = true;

		// Execute the test inside a try/catch as any failure will be reported with an exception throw
		try
		{
			(*m_pFunction)();
		}
		catch(_sxCUTestFailureException&)
		{
			// Failure
			bSucceed = false;
		}

		// Notify unit test manager of the test case execution result
		sxCUTestManager::GetInstance().NotifyEndedTest(rThis, bSucceed);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//. Self tests
void UTestTest()
{
	// Simplest test
	sxUTest(true);

	// Standard nnumeric tests
	sxUTest(0 == 0);
	sxUTest(0.f == 0.f);
	sxUTest(0 != 1);
	sxUTest(0.f != 1.f);

	// Assertions
	sxAssert(true, "Internal unit test assertion");
	sxUTestAssert(sxAssert(false, "Internal unit test assertion"));

	sxValidate(true, "Internal unit test validation");
	sxUTestAssert(sxValidate(false, "Internal unit test validation"));}

// Register test
sxRegisterUTestCase(UTestTest, sxERegisterSecondaryPriority::eVeryHigh, "Unit test framework self tests");

#endif // __sxBuildUTest