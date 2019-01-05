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
// Driver server object interface
class sxIServerObject : public sxIIntrusiveCounter, private sxINonCopyable
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxIServerObject() {}

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxIServerObject() {}
};

// Type of a server object smart pointer
typedef sxCTIntrusivePtr<sxIServerObject>::sxCType sxIServerObjectPtr;
