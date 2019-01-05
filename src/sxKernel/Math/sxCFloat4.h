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

#include "sxCFloat3.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vector of 4 floats
class sxCFloat4
{
public:

	//----------------------------------------------------------------------------------------------------------
	// Constructor
	sxCFloat4()
	{
	}

	//----------------------------------------------------------------------------------------------------------
	// Constructor
	sxCFloat4(sxFloat a_fX, sxFloat a_fY, sxFloat a_fZ, sxFloat a_fW) :
		m_fX(a_fX),
		m_fY(a_fY),
		m_fZ(a_fZ),
		m_fW(a_fW)
	{
	}

	//----------------------------------------------------------------------------------------------------------
	// Constructor, from a sxCFloat3
	sxCFloat4(sxCFloat3 const& a_rf3Value, sxFloat a_fW) :
		m_fX(a_rf3Value.m_fX),
		m_fY(a_rf3Value.m_fY),
		m_fZ(a_rf3Value.m_fZ),
		m_fW(a_fW)
	{
	}

	//----------------------------------------------------------------------------------------------------------
	// Copy constructor
	sxCFloat4(sxCFloat4 const& a_rf4Value) :
		m_fX(a_rf4Value.m_fX),
		m_fY(a_rf4Value.m_fY),
		m_fZ(a_rf4Value.m_fZ),
		m_fW(a_rf4Value.m_fW)
	{
	}

	//----------------------------------------------------------------------------------------------------------
	// Assignment operator
	void operator = (sxCFloat4 const& a_rf4Value)
	{
		m_fX = a_rf4Value.m_fX;
		m_fY = a_rf4Value.m_fY;
		m_fZ = a_rf4Value.m_fZ;
		m_fW = a_rf4Value.m_fW;
	}

	//----------------------------------------------------------------------------------------------------------
	// Comparison operator
	sxBool operator == (sxCFloat4 const& a_rf4Value)
	{
		return		m_fX == a_rf4Value.m_fX
				&&	m_fY == a_rf4Value.m_fY
				&&	m_fZ == a_rf4Value.m_fZ
				&&	m_fW == a_rf4Value.m_fW;
	}

	//----------------------------------------------------------------------------------------------------------
	// Comparison operator
	sxBool operator != (sxCFloat4 const& a_rf4Value)
	{
		return		m_fX != a_rf4Value.m_fX
				||	m_fY != a_rf4Value.m_fY
				||	m_fZ != a_rf4Value.m_fZ
				||	m_fW != a_rf4Value.m_fW;
	}

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// The 4 floats of the vector
	sxFloat m_fX;
	sxFloat m_fY;
	sxFloat m_fZ;
	sxFloat m_fW;
};