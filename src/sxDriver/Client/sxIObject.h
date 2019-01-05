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

#include "sxDriver/Client/sxCFactorySelector.h"
#include "sxDriver/Server/Interface/sxIServerObject.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver client object interface
class sxIObject : public sxINonCopyable
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxIObject(sxEFactoryObject::sxEValue a_eValue);

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxIObject();

protected:

	//-------------------------------------------------------------------------------------------------------
	// Allow access to the server object in the protected scope
	sxIServerObject& GetServerObject() const {return m_spServerObject;}

private:

	// Server object smart pointer. Used to hold a reference as long as the client object exists.
	sxIServerObjectPtr m_spServerObject;
};
