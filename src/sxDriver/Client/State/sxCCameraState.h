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
// Camera state data.
class sxCCameraStateData final : public sxIStateData
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor.
	sxCCameraStateData();

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual clear function
	virtual void Clear();

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual copy function
	virtual void Copy(sxICommandData const& a_rData);

	void SetEye(sxCFloat3 const& a_f3Value) {m_f3Eye = a_f3Value;}
	sxCFloat3 const& GetEye() const {return m_f3Eye;}

	void SetAt(sxCFloat3 const& a_f3Value) {m_f3At = a_f3Value;}
	sxCFloat3 const& GetAt() const {return m_f3At;}
	
	void SetUp(sxCFloat3 const& a_f3Value) {m_f3Up = a_f3Value;}
	sxCFloat3 const& GetUp() const {return m_f3Up;}

	void SetFocalAngle(sxFloat a_fValue) {m_fFocalAngle = a_fValue;}
	sxFloat GetFocalAngle() const {return m_fFocalAngle;}

	void SetRatio(sxFloat a_fValue) {m_fRatio = a_fValue;}
	sxFloat GetRatio() const {return m_fRatio;}

	void SetNear(sxFloat a_fValue) {m_fNear = a_fValue;}
	sxFloat GetNear() const {return m_fNear;}

	void SetFar(sxFloat a_fValue) {m_fFar = a_fValue;}
	sxFloat GetFar() const {return m_fFar;}

private:

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// Camera position
	sxCFloat3 m_f3Eye;

	// Camera look at position
	sxCFloat3 m_f3At;

	// Camera up direction
	sxCFloat3 m_f3Up;

	// Camera focal angle
	sxFloat m_fFocalAngle;

	// Width/Height ratio
	sxFloat m_fRatio;

	// Near and far plane distance
	sxFloat m_fNear, m_fFar;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type of the sxCCameraState object built base on the state wrapper helper
typedef sxCTStateWrapper<	sxCCameraStateData,
							sxEStateType::eCamera,
							sxEFactoryObject::eCameraState> sxCCameraState;
