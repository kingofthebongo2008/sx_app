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
// Material state command data.
class sxCMaterialStateData : public sxIStateData
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor.
	sxCMaterialStateData();

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual clear function
	virtual void Clear();

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual copy function
	virtual void Copy(sxICommandData const& a_rData);

	//-------------------------------------------------------------------------------------------------------
	// Material color setter/getter
	void SetColor(sxCFloat4 const& a_f4Value) {m_f4Color = a_f4Value;}
	sxCFloat4 const& GetColor() const {return m_f4Color;}

	//-------------------------------------------------------------------------------------------------------
	// Ambient lighting coefficient setter/getter
	void SetAmbient(sxFloat a_fValue) {m_fAmbient = a_fValue;}
	sxFloat GetAmbient() const {return m_fAmbient;}

	//-------------------------------------------------------------------------------------------------------
	// Diffuse lighting coefficient setter/getter
	void SetDiffuse(sxFloat a_fValue) {m_fDiffuse = a_fValue;}
	sxFloat GetDiffuse() const {return m_fDiffuse;}

	//-------------------------------------------------------------------------------------------------------
	// Specular lighting coefficient setter/getter
	void SetSpecular(sxFloat a_fValue) {m_fSpecular = a_fValue;}
	sxFloat GetSpecular() const {return m_fSpecular;}

	//-------------------------------------------------------------------------------------------------------
	// Emissive lighting coefficient setter/getter
	void SetEmissive(sxFloat a_fValue) {m_fEmissive = a_fValue;}
	sxFloat GetEmissive() const {return m_fEmissive;}

	//-------------------------------------------------------------------------------------------------------
	// Specular power setter/getter
	void SetSpecularPower(sxFloat a_fValue) {m_fSpecularPower = a_fValue;}
	sxFloat GetSpecularPower() const {return m_fSpecularPower;}

private:

	// Material color
	sxCFloat4 m_f4Color;

	// Ambient lighting coefficient
	sxFloat m_fAmbient;

	// Diffuse lighting coefficient
	sxFloat m_fDiffuse;

	// Specular lighting coefficient
	sxFloat m_fSpecular;

	// Emissive lighting coefficient
	sxFloat m_fEmissive;

	// Specular power
	sxFloat m_fSpecularPower;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type of the sxCMaterialState object built base on the state wrapper helper
typedef sxCTStateWrapper<	sxCMaterialStateData,
							sxEStateType::eMaterial,
							sxEFactoryObject::eMaterialState> sxCMaterialState;
