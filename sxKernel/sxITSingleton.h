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
// Singleton object template interface. This implementation does not instantiate the object, but rather 
// asserts for unique instantiation and allows to get the only instantiated object reference using 
// ::GetInstance function.
template<typename t_Object>
class sxITSingleton : private sxINonCopyable
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Asserts for unique instantiation 
	sxITSingleton()
	{
		sxAssert(ms_pSingletonObject == NULL, "Object's already instantiated");
		ms_pSingletonObject = this;
	}

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxITSingleton()
	{
		sxAssert(ms_pSingletonObject != NULL, "Object isn't instantiated");
		ms_pSingletonObject = NULL;
	}

	//-------------------------------------------------------------------------------------------------------
	// Instance getter function. Asserts that object's instantiated
	static t_Object& GetInstance()
	{
		sxAssert(ms_pSingletonObject != NULL, "Object isn't instantiated");
		return *static_cast<t_Object*>(ms_pSingletonObject);
	}

	//-------------------------------------------------------------------------------------------------------
	// Test if singleton instance is valid ie : allocated once
	static sxBool IsInstanceValid()
	{
		return (ms_pSingletonObject != NULL);
	}
private:

	// This singleton object instance
	static sxITSingleton<t_Object>* ms_pSingletonObject;
};

// Singleton static member initialization
template<typename t_Object>
sxITSingleton<t_Object>* sxITSingleton<t_Object>::ms_pSingletonObject = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Singleton registered object interface.
template<typename t_Object>
class sxCTRegisteredSingleton : private sxIRegisteredObject
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Declare registration priority
	sxCTRegisteredSingleton(sxERegisterPrimaryPriority::EValue a_ePriority,
							sxERegisterSecondaryPriority::EValue a_eSecondaryPriority) :
		sxIRegisteredObject(a_ePriority, a_eSecondaryPriority)
	{
	}

private:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Declare registration priority
	virtual void operator()(sxERegisteredOperation::EValue a_eOp)
	{
		// Singleton object reference
		static t_Object* pObject = NULL;

		if(a_eOp == sxERegisteredOperation::eInitialize)
		{
			sxAssertNoText(pObject == NULL);
			
			// Allocate singleton
			pObject = &sxNew(t_Object);
		}
		else if(a_eOp == sxERegisteredOperation::eDestroy)
		{
			sxAssertNoText(pObject != NULL);

			// Delete it
			sxDelete(*pObject);
		}
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper macro to instantiate a singleton at application boot time
#define sxRegisterSingleton(x_Type, x_ePrimaryPriority, x_eSecondaryPriority)\
	sxRegisterObject(sxCTRegisteredSingleton<x_Type>, x_ePrimaryPriority, x_eSecondaryPriority)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Singleton function registered object implementation.
template<typename x_tSingleton>
class sxCTRegisteredSingletonFunction : public sxIRegisteredObject
{
public:

	// Callback function type
	typedef void (x_tSingleton::*sxPfnRegisteredFunction)(sxERegisteredOperation::EValue a_eOp);

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCTRegisteredSingletonFunction(sxPfnRegisteredFunction a_pFunction,
									sxERegisterPrimaryPriority::EValue a_ePrimaryPriority,
									sxERegisterSecondaryPriority::EValue a_eSecondaryPriority) :
		sxIRegisteredObject(a_ePrimaryPriority, a_eSecondaryPriority),
		m_pFunction(a_pFunction)
	{
		sxAssert(m_pFunction != NULL, "Registered function cannot be NULL");
	}

private:

	//-------------------------------------------------------------------------------------------------------
	// Overide in order to catch registration operations
	virtual void operator()(sxERegisteredOperation::EValue a_eOp)
	{
		// Call singleton registered function
		(sxITSingleton<x_tSingleton>::GetInstance().*m_pFunction)(a_eOp);
	}

	// Callback function
	sxPfnRegisteredFunction m_pFunction;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Macro used to register any sxCRegisteredFunction class and derived class
#define sxRegisteredSingletonFunction(x_tSingleton, x_oFunction, x_ePrimaryPriority, x_eSecondaryPriority)\
	sxRegisterObject(	sxCTRegisteredSingletonFunction<x_tSingleton>,\
						x_oFunction,\
						x_ePrimaryPriority,\
						x_eSecondaryPriority)