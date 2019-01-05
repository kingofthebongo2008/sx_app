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
#include "sxRegistration.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxIRegisteredObject implementation

//-----------------------------------------------------------------------------------------------------------
//
sxIRegisteredObject::sxIRegisteredObject(	sxERegisterPrimaryPriority::EValue a_ePrimaryPriority,
											sxERegisterSecondaryPriority::EValue a_eSecondaryPriority) :
	m_ePrimaryPriority(a_ePrimaryPriority),
	m_eSecondaryPriority(a_eSecondaryPriority)
{
	sxCRegistrer::RegisterObject(rThis);
}
	
//-----------------------------------------------------------------------------------------------------------
//
sxIRegisteredObject::~sxIRegisteredObject()
{
	sxCRegistrer::UnregisterObject(rThis);
}

//-----------------------------------------------------------------------------------------------------------
//
sxBool sxIRegisteredObject::IsLessPrioritary(	sxIRegisteredObject const* a_pLeft,
												sxIRegisteredObject const* a_pRight)
{
	// The highest priority is the lower integer value
	return		(a_pLeft->m_ePrimaryPriority < a_pRight->m_ePrimaryPriority)
			||	(	(a_pLeft->m_ePrimaryPriority == a_pRight->m_ePrimaryPriority)
					&& (a_pLeft->m_eSecondaryPriority < a_pRight->m_eSecondaryPriority));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCRegisteredFunction implementation

//-----------------------------------------------------------------------------------------------------------
//
sxCRegisteredFunction::sxCRegisteredFunction(	sxPfnRegisteredFunction a_pFunction,
												sxERegisterPrimaryPriority::EValue a_ePrimaryPriority,
												sxERegisterSecondaryPriority::EValue a_eSecondaryPriority) :
	sxIRegisteredObject(a_ePrimaryPriority, a_eSecondaryPriority),
	m_pFunction(a_pFunction)
{
	sxAssert(m_pFunction != NULL, "Registered function cannot be NULL");
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCRegisteredFunction::operator()(sxERegisteredOperation::EValue a_eOp)
{
	// Call registered function
	(*m_pFunction)(a_eOp);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCRegistrer implementation

// List of registered objects
sxCRegistrer::sxCRegisteredList* sxCRegistrer::m_plRegisteredObjects = NULL;

//-----------------------------------------------------------------------------------------------------------
//
void sxCRegistrer::RegisterObject(sxIRegisteredObject& a_rObject)
{
	// Alocate the list if it's the first object registration
	if(m_plRegisteredObjects == NULL)
	{
		m_plRegisteredObjects = &sxNew(sxCRegisteredList);
	}

	// Add that object to the list
	m_plRegisteredObjects->push_front(&a_rObject);
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCRegistrer::UnregisterObject(sxIRegisteredObject& a_rObject)
{
	// Calling this function means at least one object was registered
	sxAssertNoText(m_plRegisteredObjects != NULL);

	// Remove the object from the list
	m_plRegisteredObjects->remove(&a_rObject);

	// Is list has been emptied, then delete it
	if(m_plRegisteredObjects->size() == 0)
	{
		sxDelete(*m_plRegisteredObjects);
	}
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCRegistrer::CallInitialize()
{
	// m_plRegisteredObjects is unallocated if no object's registered
	if(m_plRegisteredObjects == NULL)
	{
		return;
	}

	// First sort the list
	m_plRegisteredObjects->sort(sxIRegisteredObject::IsLessPrioritary);

	// Iterate and call initialize execution
	sxForEachCallFunctor(	(*m_plRegisteredObjects),
							bind( mem_fn(&sxIRegisteredObject::operator()), _1, sxERegisteredOperation::eInitialize));
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCRegistrer::CallDestroy()
{
	// m_plRegisteredObjects is unallocated if no object's registered
	if(m_plRegisteredObjects == NULL)
	{
		return;
	}

	// Iterate and call destroy execution
	sxForEachReverseCallFunctor((*m_plRegisteredObjects),
								bind( mem_fn(&sxIRegisteredObject::operator()), _1, sxERegisteredOperation::eDestroy));
}
