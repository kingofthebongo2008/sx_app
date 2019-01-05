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
#include "sxCTSmartPtr.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread unit tests
#ifdef __sxBuildUTest

// Uses DirectX to test CompPtr policy
#include <d3d9.h>

//-----------------------------------------------------------------------------------------------------------
//
void UTestSmartPtrAssertPolicy()
{
	// Define a class fo internal unitest usage
	class sxCPointed
	{
	public:
		void Function() {}
		void ConstFunction() const {}
	};
		
	// Declares pointer type
	typedef sxCTAssertPtr<sxCPointed>::sxCType sxCPointedPtr;
	typedef sxCTAssertPtr<sxCPointed const>::sxCType sxCPointedConstPtr;

	// Comparison operators
	{
		// Object stack allocation
		sxCPointed oObject;

		sxCPointedPtr spObject;
		sxCPointedConstPtr spConstObject1(oObject);
		sxCPointedPtr spObject2(&oObject);

		// Comparison
		sxUTest(spObject == sxCPointedPtr());
		sxUTest(spObject == spObject);
		sxUTest(spConstObject1 == spObject2);
		sxUTest(spConstObject1 != spObject);
		sxUTest(spConstObject1 == &oObject);
		sxUTest(spObject != &oObject);
	}

	// Dereferencing
	{
		// Unassigned pointer
		sxCPointedPtr spObject;

		// Dereferencing should assert on unassigned pointers
		sxUTestAssert(spObject->Function());
		sxUTestAssert(*spObject);
		sxUTestAssert((sxCPointed&)spObject);

		// Dereferencing should succeed on assigned pointers
		sxCPointed oObject;
		spObject = oObject;
		spObject->Function();
		spObject->ConstFunction();

		// Even on const pointers
		sxCPointedConstPtr spConstObject(oObject);
		spConstObject->ConstFunction();
	}
}

// Register test
sxRegisterUTestCase(UTestSmartPtrAssertPolicy,
					sxERegisterSecondaryPriority::eHigh, "Smart pointer assert policy");

//-----------------------------------------------------------------------------------------------------------
//
void UTestSmartPtrIntrusivePolicy()
{
	// Define a class fo internal unitest usage
	class sxCPointed : public sxIIntrusiveCounter {};

	// Declares pointer type
	typedef sxCTIntrusivePtr<sxCPointed>::sxCType sxCPointedPtr;

	// Create 2 smart pointer
	sxCPointedPtr spObject1;
	sxCPointedPtr spObject2(sxNew(sxCPointed));

	// Only one smaptr pointer stores a refernece
	sxUTest(spObject2->GetRefCount() == 1);

	// Now the 2 smart pointers share a reference
	spObject1 = spObject2;
	sxUTest(spObject2->GetRefCount() == 2);

	// Release a reference
	spObject1 = NULL;
	sxUTest(spObject2->GetRefCount() == 1);

	// Add a reference using a new scope
	{
		sxCPointedPtr spObject3(spObject2);
		sxUTest(spObject2->GetRefCount() == 2);
	}
	sxUTest(spObject2->GetRefCount() == 1);

	// Release last reference
	spObject2 = NULL;
}

// Register test
sxRegisterUTestCase(UTestSmartPtrIntrusivePolicy,
					sxERegisterSecondaryPriority::eHigh, "Smart pointer intrusive policy");

//-----------------------------------------------------------------------------------------------------------
//
void UTestSmartPtrCOMPolicy()
{
	// Type of the smart pointer that'll be tested
	typedef sxCTComPtr<IUnknown>::sxCType sxIUnknowPtr;

	// Create a Com object, reference count equals to 1 on creation
	IUnknown* pUnkonwn = Direct3DCreate9(D3D_SDK_VERSION);

	// Create 2 smart pointer, only one smart pointer stores a reference so counter equals 2
	sxIUnknowPtr spObject1;
	sxIUnknowPtr spObject2(pUnkonwn);

	// Now the 2 smart pointers share a reference, couter equals 3
	spObject1 = spObject2;

	// Release a reference, couter equals 2
	spObject1 = NULL;

	// Add a reference using a new scope
	{
		sxIUnknowPtr spObject3(spObject2);
	}

	// Release last smart pointed reference counter equals 1
	spObject2 = NULL;

	// Now release the last reference that wasn't owned by a smart pointer
	sxUTest(pUnkonwn->Release() == 0);
}

// Register test
sxRegisterUTestCase(UTestSmartPtrCOMPolicy,
					sxERegisterSecondaryPriority::eHigh, "Smart pointer COM policy");

#endif // __sxBuildUTest
