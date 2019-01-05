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

#include "sxDriver/Client/sxICommand.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UTest command data. Used to be compared with sxCUTestCheckCommandData when both are queued sequentially.
class sxCUTestCommandData : public sxICommandData
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Initialize internal value to 0
	sxCUTestCommandData() :
		m_uiValue(0)
	{}

	//-------------------------------------------------------------------------------------------------------
	// Value setter and getter
	void SetValue(sxUInt a_uiValue) {m_uiValue = a_uiValue;}
	sxUInt GetValue() const {return m_uiValue;}

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual clear function
	virtual void Clear() {m_uiValue = 0;}

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual copy function
	virtual void Copy(sxICommandData const& a_rData)
	{
		// Fetch the source data
		sxCUTestCommandData const& rData = (sxCUTestCommandData const&)a_rData;

		// Copy it
		m_uiValue = rData.m_uiValue;
	}
	
private:

	// Value hold by the data to be compared with the sxCUTestCheckCommandData one
	sxUInt m_uiValue;
};

// Type of the sxCUTestCommand object built base on the command wrapper helper
typedef sxCTCommandWrapper<sxCUTestCommandData, sxEFactoryObject::eUTestCommand> sxCUTestCommand;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UTest check command data. Used to be compared with sxCUTestCommandData when both are queued sequentially.
class sxCUTestCheckCommandData : public sxICommandData
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Constructor. Initialize internal value to 0
	sxCUTestCheckCommandData() :
		m_uiValue(0)
	{}

	//-------------------------------------------------------------------------------------------------------
	// Value setter and getter
	void SetValue(sxUInt a_uiValue) {m_uiValue = a_uiValue;}
	sxUInt GetValue() const {return m_uiValue;}

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual clear function
	virtual void Clear() {m_uiValue = 0;}

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual copy function
	virtual void Copy(sxICommandData const& a_rData)
	{
		sxCUTestCheckCommandData const& rData = (sxCUTestCheckCommandData const&)a_rData;
		m_uiValue = rData.m_uiValue;
	}
	
private:

	// Value hold by the data to be compared with the sxCUTestCommandData one
	sxUInt m_uiValue;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type of the sxCUTestCheckCommand object built base on the command wrapper helper
typedef sxCTCommandWrapper<sxCUTestCheckCommandData, sxEFactoryObject::eUTestCheckCommand> sxCUTestCheckCommand;

#endif // __sxBuildUTest
