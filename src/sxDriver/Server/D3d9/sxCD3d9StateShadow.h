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

#include "sxDriver/Client/sxEStateType.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// D3d9 Server state shadow. Stores the server states that are bound when a command occurs. This is not the 
// real right solution as it should reather be implemented in the server interface side.
class sxCD3d9StateShadow
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCD3d9StateShadow();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCD3d9StateShadow();

	//-------------------------------------------------------------------------------------------------------
	// Set a state and it associated data in the shadow
	void SetState(	sxEStateType::sxEValue a_eStateType,
					class sxIServerObject const& a_rState,
					class sxICommandData const& a_rData);

	//-------------------------------------------------------------------------------------------------------
	// Get a_eStateType state in the shadow
	class sxIServerObject const& GetState(sxEStateType::sxEValue a_eStateType) const;

	//-------------------------------------------------------------------------------------------------------
	// Get a_eStateType state data in the shadow
	class sxICommandData const& GetStateData(sxEStateType::sxEValue a_eStateType) const;

	//-------------------------------------------------------------------------------------------------------
	// Release all states and data
	void Release();

private:

	// Vector of states
	typedef vector<class sxIServerObject const*> sxCServerObjectVector;
	sxCServerObjectVector m_vpServerStates;

	// Vector of state data
	typedef vector<class sxICommandData const*> sxCCommandDataVector;
	sxCCommandDataVector m_vpServerStateData;
};
