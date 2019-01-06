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

#include "sxDriver/Client/sxIState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Transform state command data. Embeds render target sizes and clear color. This clear color information
// might not be a real-world case but rather demonstrates the one-to-many abstraction design.
class sxCTransformStateData final : public sxIStateData
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor.
	sxCTransformStateData();

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual clear function
	virtual void Clear();

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual copy function
	virtual void Copy(sxICommandData const& a_rData);

	void SetRight(sxCFloat3 const& a_rf3Value) {m_f3Right = a_rf3Value;}
	sxCFloat3 const& GetRight() const {return m_f3Right;}

	void SetUp(sxCFloat3 const& a_rf3Value) {m_f3Up = a_rf3Value;}
	sxCFloat3 const& GetUp() const {return m_f3Up;}

	void SetAt(sxCFloat3 const& a_rf3Value) {m_f3At = a_rf3Value;}
	sxCFloat3 const& GetAt() const {return m_f3At;}

	void SetPos(sxCFloat3 const& a_rf3Value) {m_f3Pos = a_rf3Value;}
	sxCFloat3 const& GetPos() const {return m_f3Pos;}
	
private:

	// The 4 vector that compose the affine transformation matrix
	sxCFloat3 m_f3Right;
	sxCFloat3 m_f3Up;
	sxCFloat3 m_f3At;
	sxCFloat3 m_f3Pos;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type of the sxCTransformState object built base on the state wrapper helper
typedef sxCTStateWrapper<	sxCTransformStateData,
							sxEStateType::eTransform,
							sxEFactoryObject::eTransformState> sxCTransformState;
