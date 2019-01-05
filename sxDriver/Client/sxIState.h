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

#include "sxICommand.h"
#include "sxCStateCache.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver client state data object interface.
class sxIStateData : public sxICommandData
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxIStateData() {}

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxIStateData() {}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver client state object interface. Implement state cache binding functionalities. A internal command 
// object allows to to queue the server state object to the command buffer.
class sxIState : private sxINonCopyable
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxIState(sxEStateType::sxEValue a_eStateType, sxICommand& a_rCommand) :
		m_eStateType(a_eStateType),
		m_rCommand(a_rCommand)
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxIState() {}

	//-------------------------------------------------------------------------------------------------------
	// Get the state object enumerated value
	sxEStateType::sxEValue GetStateType() const {return m_eStateType;}

private:

	// _sxCCommandLocker is friend in order to access the internal command used by the state object
	friend _sxCCommandLocker;

	//----------------------------------------------------------------------------------------------------------
	// Internal command object getters
	sxICommand& GetCommand() {return m_rCommand;}
	sxICommand const& GetCommand() const {return m_rCommand;}

	//----------------------------------------------------------------------------------------------------------
	// Attributes

	// Enumerated state type
	sxEStateType::sxEValue m_eStateType;

	// Command object used by this state.
	sxICommand& m_rCommand;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// State wrapper object that instantiate the internal command object based on template arguments. This 
// command is used to queue the state server object into the command buffer once flushed from the cache.
template<typename t_StateData, sxEStateType::sxEValue t_eStateType, sxEFactoryObject::sxEValue t_eValue>
class sxCTStateWrapper : public sxIState
{
public:

	// Locker types definitions
	typedef sxCTReadOnlyLocker<t_StateData> sxCReadOnlyLocker;
	typedef sxCTReadWriteLocker<t_StateData> sxCReadWriteLocker;

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Initialize the internal command
	sxCTStateWrapper() :
		sxIState(t_eStateType, m_oCommand),
		m_oCommand()
	{
	}

private:

	// Internal command
	sxCTCommandWrapper<t_StateData, t_eValue> m_oCommand;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Default state registration

//-----------------------------------------------------------------------------------------------------------
// Default state construction function
template<typename t_State>
sxIState& _sxTStateDefaultConstructor() { return sxNew(t_State); }

//-----------------------------------------------------------------------------------------------------------
// Default state registration function
template<typename t_State, sxEStateType::sxEValue t_eStateType>
inline void _sxRegisterDefaultState(sxERegisteredOperation::EValue a_eOp)
{
	if(a_eOp == sxERegisteredOperation::eInitialize)
	{
		sxCStateCache::RegisterDefaultState(t_eStateType, &_sxTStateDefaultConstructor<t_State>);
	}
}

//-----------------------------------------------------------------------------------------------------------
// Default state registration macro.
#define sxRegisterDefaultState(x_State, x_eStateType)\
	sxRegisterFunction(	(_sxRegisterDefaultState<x_State, x_eStateType>),\
						sxERegisterPrimaryPriority::eNormal,\
						sxERegisterSecondaryPriority::eNormal)
