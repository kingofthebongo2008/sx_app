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
// Vector of 3 floats
class sxCFloat3
{
public:

	//----------------------------------------------------------------------------------------------------------
	// Constructor
	sxCFloat3()
	{
	}

	//----------------------------------------------------------------------------------------------------------
	// Constructor
	sxCFloat3(sxFloat a_fX, sxFloat a_fY, sxFloat a_fZ) :
		m_fX(a_fX),
		m_fY(a_fY),
		m_fZ(a_fZ)
	{
	}

	//----------------------------------------------------------------------------------------------------------
	// Copy constructor
	sxCFloat3(sxCFloat3 const& a_rf3Value) :
		m_fX(a_rf3Value.m_fX),
		m_fY(a_rf3Value.m_fY),
		m_fZ(a_rf3Value.m_fZ)
	{
	}

	//----------------------------------------------------------------------------------------------------------
	// Assignment operator
	void operator = (sxCFloat3 const& a_rf3Value)
	{
		m_fX = a_rf3Value.m_fX;
		m_fY = a_rf3Value.m_fY;
		m_fZ = a_rf3Value.m_fZ;
	}

	//----------------------------------------------------------------------------------------------------------
	// Comparison operator
	sxBool operator == (sxCFloat3 const& a_rf3Value)
	{
		return		m_fX == a_rf3Value.m_fX
				&&	m_fY == a_rf3Value.m_fY
				&&	m_fZ == a_rf3Value.m_fZ;
	}

	//----------------------------------------------------------------------------------------------------------
	// Comparison operator
	sxBool operator != (sxCFloat3 const& a_rf3Value)
	{
		return		m_fX != a_rf3Value.m_fX
				||	m_fY != a_rf3Value.m_fY
				||	m_fZ != a_rf3Value.m_fZ;
	}

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// The 3 floats of the vector
	sxFloat m_fX;
	sxFloat m_fY;
	sxFloat m_fZ;
};