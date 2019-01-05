//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//																											|
//								ShaderX7. Cross platform rendering thread. 2008.							|
//				Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.			|
//																											|
//	This program is free software. It is distributed in the hope that it will be useful, but without any	|
//	warranty, without even the implied warranty of merchantability or fitness for a particular purpose.		|
//																											|
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unit test implementation when __sxBuildUTest isn't defined
#ifndef __sxBuildUTest

// Helper macro to activate deactivate code portions according to __sxBuildUTest
#define sxIfBuildUTest(...)
#define sxIfNotBuildUTest(...) __VA_ARGS__

#else // __sxBuildUTest

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unit test implementation when __sxBuildUTest is defined

//-----------------------------------------------------------------------------------------------------------
// Helper macro to activate deactivate code portions according to __sxBuildUTest
#define sxIfBuildUTest(...) __VA_ARGS__
#define sxIfNotBuildUTest(...)

//-----------------------------------------------------------------------------------------------------------
// Test that expression it true
#define sxUTest(x_Test)\
do{\
	if((x_Test) == false)\
	{\
		throw _sxCUTestFailureException();\
	}\
}while(false)



//-----------------------------------------------------------------------------------------------------------
// Test that expression produces an assertion
#ifdef __sxBuildAssert

#define sxUTestAssert(x_Test)\
do{\
	try\
	{\
		sxNsAssertion::sxCUTestAssertionEnabler oEnabler;\
		x_Test;\
	}\
	catch(sxNsAssertion::sxCUTestAssertionException&)\
	{\
		/* Assertion was produced so everything's fine*/\
		break;\
	}\
	/* No assertion produced so report the failure*/\
	throw _sxCUTestFailureException();\
}while(false)

#else // __sxBuildAssert

#define sxUTestAssert(...)

#endif // __sxBuildAssert

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unit test manager
class sxCUTestManager : public sxITSingleton<sxCUTestManager>
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCUTestManager();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCUTestManager();

	//-------------------------------------------------------------------------------------------------------
	// Start registered function
	void Start(sxERegisteredOperation::EValue a_eOp);

	//-------------------------------------------------------------------------------------------------------
	// Finish registered function
	void Finish(sxERegisteredOperation::EValue a_eOp);

private:

	// sxCUTestCase is friend in order to increment test counts
	friend class sxCUTestCase;

	//-------------------------------------------------------------------------------------------------------
	// Notify that a test case has ended, wheter it was failed or succeeded
	void NotifyEndedTest(sxCUTestCase& a_rTestCase, sxBool a_bSucceded);

	// Number of succeeded tests
	sxUInt m_uiSuccedeedTestCount;

	// Number of failed tests
	sxUInt m_uiFailedTestCount;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unit test registration object
class sxCUTestCase : private sxIRegisteredObject
{
public:

	// Test case function definition
	typedef void (*sxPfnUTestFunction)();

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Takes the test case function as argument
	sxCUTestCase(	sxPfnUTestFunction a_pFunction,
					sxChar const* a_szName,
					sxERegisterSecondaryPriority::EValue a_eSecondaryPriority);

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	~sxCUTestCase();

	//-------------------------------------------------------------------------------------------------------
	// Get test name
	sxChar const* GetName() const {return m_szName;}

private:

	//-------------------------------------------------------------------------------------------------------
	// Internal execution function
	virtual void operator()(sxERegisteredOperation::EValue a_eOp);

	// Test case function
	sxPfnUTestFunction m_pFunction;

	// UTes4t case name
	sxChar const* m_szName;
};

//-----------------------------------------------------------------------------------------------------------
// Unit test case registration macro
#define sxRegisterUTestCase(x_UTestFunction, x_eSecondaryPriority, x_UtestName)\
	sxRegisterObject(sxCUTestCase, x_UTestFunction, x_UtestName, x_eSecondaryPriority)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unit test failure exception
class _sxCUTestFailureException : public exception {};

#endif // __sxBuildUTest
