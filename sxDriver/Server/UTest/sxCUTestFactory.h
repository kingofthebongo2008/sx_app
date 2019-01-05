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

#ifdef __sxBuildUTest

#include "sxDriver/Server/Interface/sxIServerFactory.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UTest Server object factory.
class sxCUTestFactory : public sxITServerFactoryWrapper<sxCUTestFactory>
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCUTestFactory();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCUTestFactory();

	//-------------------------------------------------------------------------------------------------------
	// Pure function  implementation that instantiate the server object.
	virtual class sxIServerObject& Instantiate(sxEFactoryObject::sxEValue a_eValue);
};

#endif // __sxBuildUTest
