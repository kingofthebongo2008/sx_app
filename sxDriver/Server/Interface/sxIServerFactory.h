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

#include "sxDriver/Client/sxCDevice.h"
#include "sxDriver/Client/sxCFactorySelector.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Server object factory. A minimal factory used to create platform server objects matching a client one.
class sxIServerFactory
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxIServerFactory(sxEGraphicApi::sxEValue a_eApi);

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxIServerFactory();

	//-------------------------------------------------------------------------------------------------------
	// Pure function that instantiate a server object.
	virtual class sxIServerObject& Instantiate(sxEFactoryObject::sxEValue a_eValue) = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Factory wrapper object that implement shared code based on template arguments.
template <class t_Factory>
class sxITServerFactoryWrapper : protected sxIServerFactory, private sxITSingleton<t_Factory>
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxITServerFactoryWrapper(sxEGraphicApi::sxEValue a_eApi) :
		sxIServerFactory(a_eApi)
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxITServerFactoryWrapper()
	{
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Server factory registration

//-----------------------------------------------------------------------------------------------------------
// Server factory registration macro
#define sxRegisterServerFactory(x_tServerFactory)\
	sxRegisterSingleton(x_tServerFactory,	sxERegisterPrimaryPriority::eHighest,\
											sxERegisterSecondaryPriority::eNormal)
