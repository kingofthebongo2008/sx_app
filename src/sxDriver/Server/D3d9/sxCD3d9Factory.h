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

#include "sxDriver/Server/Interface/sxIServerFactory.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// D3d9 Server object factory.
class sxCD3d9Factory final : public sxITServerFactoryWrapper<sxCD3d9Factory>
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCD3d9Factory();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCD3d9Factory();

	//-------------------------------------------------------------------------------------------------------
	// Pure function  implementation that instantiate the server object.
	virtual class sxIServerObject& Instantiate(sxEFactoryObject::sxEValue a_eValue);
};
