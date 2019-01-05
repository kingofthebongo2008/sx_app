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
// Registration primary priority enumeration
struct sxERegisterPrimaryPriority
{
	enum EValue
	{
		eReserved,	// Reserved for memory management tasks

		eHighest,
		eVeryHigh,
		eHigh,
		eAboveNormal,
		eNormal,
		eBelowNormal,
		eVeryLow,
		eLowest,

#ifdef __sxBuildUTest
		eUnitTest,	// Unit tests execution
#endif // __sxBuildUTest
	};
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Registration secondary priority enumeration, ie the priority inside a primary layer
struct sxERegisterSecondaryPriority
{
	enum EValue
	{
		eHighest,
		eVeryHigh,
		eHigh,
		eAboveNormal,
		eNormal,
		eBelowNormal,
		eVeryLow,
		eLowest,
	};
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Registered object available operation
struct sxERegisteredOperation
{
	enum EValue
	{
		eInitialize,
		eDestroy
	};
};
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Registered object interface. Any derived class can use sxRegisterObject macro to be registered.
class sxIRegisteredObject : private sxINonCopyable
{
public:

	//----------------------------------------------------------------------------------------------------------
	// Construtor. Initialize object priority
	sxIRegisteredObject(sxERegisterPrimaryPriority::EValue a_ePrimaryPriority,
						sxERegisterSecondaryPriority::EValue a_eSecondaryPriority);

	//----------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxIRegisteredObject();

private:

	// sxCRegistrer is friend in order to call sxIRegisteredObject::operator (sxERegisteredOperation::EValue a_eOp)
	friend class sxCRegistrer;

	//-------------------------------------------------------------------------------------------------------
	// Pure virtual operator to overide in order to catch registration operations
	virtual void operator()(sxERegisteredOperation::EValue a_eOp) = 0;

	//-------------------------------------------------------------------------------------------------------
	// Comparator for priority sorting
	static sxBool IsLessPrioritary(	sxIRegisteredObject const* a_pLeft, 
									sxIRegisteredObject const* a_pRight);

	// Stores the priority used for the sorting operation
	sxERegisterPrimaryPriority::EValue m_ePrimaryPriority;

	// Priority inside primary layer
	sxERegisterSecondaryPriority::EValue m_eSecondaryPriority;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Macro used to register any sxIRegisteredObject derived class
#define sxRegisterObject(x_tRegisterClass, ...)\
	static x_tRegisterClass sxConcat(_sxoRegisteredObject, __COUNTER__)(__VA_ARGS__)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function registered object implementation.
class sxCRegisteredFunction : public sxIRegisteredObject
{
public:

	// Callback function type
	typedef void (*sxPfnRegisteredFunction)(sxERegisteredOperation::EValue a_eOp);

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCRegisteredFunction(	sxPfnRegisteredFunction a_pFunction,
							sxERegisterPrimaryPriority::EValue a_ePrimaryPriority,
							sxERegisterSecondaryPriority::EValue a_eSecondaryPriority);

private:

	//-------------------------------------------------------------------------------------------------------
	// Overide in order to catch registration operations
	virtual void operator()(sxERegisteredOperation::EValue a_eOp);

	// Callback function
	sxPfnRegisteredFunction m_pFunction;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Macro used to register any sxCRegisteredFunction class and derived class
#define sxRegisterFunction(x_oFunctor, x_ePriority, x_iLayerPriority)\
	sxRegisterObject(sxCRegisteredFunction, x_oFunctor, x_ePriority, x_iLayerPriority)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCRegistrer object. This static class manages registered objects.
class sxCRegistrer
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Call register object initialization operation starting with the highest prioritary one
	static void CallInitialize();

	//-------------------------------------------------------------------------------------------------------
	// Call register object destruction operation starting with the less prioritary one
	static void CallDestroy();

private:

	// sxIRegisteredObject is friend in order to perform ::RegisterObject and ::UnregisterObject calls
	friend class sxIRegisteredObject;

	//-------------------------------------------------------------------------------------------------------
	// Register a sxIRegisteredObject
	static void RegisterObject(sxIRegisteredObject& a_rObject);

	//-------------------------------------------------------------------------------------------------------
	// Uneegister a sxIRegisteredObject
	static void UnregisterObject(sxIRegisteredObject& a_rObject);

	// Collection type of registered objects
	typedef list<sxIRegisteredObject*> sxCRegisteredList;

	// Registered object collection. Uses a pointer such that no static construction is required.
	static sxCRegisteredList* m_plRegisteredObjects;
};
