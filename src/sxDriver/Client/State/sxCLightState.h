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

class sxCLight
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCLight();

	//-------------------------------------------------------------------------------------------------------
	// Light color setter/getter
	void SetColor(sxCFloat3 const& a_f3Value) {m_f3Color = a_f3Value;}
	sxCFloat3 const& GetColor() const {return m_f3Color;}

	//-------------------------------------------------------------------------------------------------------
	// Light position setter/getter
	void SetPosition(sxCFloat3 const& a_f3Value) {m_f3Position = a_f3Value;}
	sxCFloat3 const& GetPosition() const {return m_f3Position;}

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
	// Attenuation lighting coefficient setter/getter
	void SetAttenuation(sxFloat a_fValue) {m_fAttenuation = a_fValue;}
	sxFloat GetAttenuation() const {return m_fAttenuation;}

private:

	// Light color
	sxCFloat3 m_f3Color;

	// Light position
	sxCFloat3 m_f3Position;

	// Ambient lighting coefficient
	sxFloat m_fAmbient;

	// Diffuse lighting coefficient
	sxFloat m_fDiffuse;

	// Specular lighting coefficient
	sxFloat m_fSpecular;

	// Attenuation coefficient
	sxFloat m_fAttenuation;
};
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Light state command data.
class sxCLightStateData final: public sxIStateData
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor.
	sxCLightStateData();

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual clear function
	virtual void Clear();

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual copy function
	virtual void Copy(sxICommandData const& a_rData);

	// Type of the light collection managed by the state
	typedef vector<sxCLight> sxCLightVector;

	void AddLight(sxCLight const& a_rLight) {m_vLights.push_back(a_rLight);}
	sxCLightVector& GetLights() {return m_vLights;}
	sxCLightVector const& GetLights() const {return m_vLights;}

private:

	// Type of the light collection managed by the state
	typedef vector<sxCLight> sxCLightVector;
	sxCLightVector m_vLights;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type of the sxCLightState object built base on the state wrapper helper
typedef sxCTStateWrapper<	sxCLightStateData,
							sxEStateType::eLight,
							sxEFactoryObject::eLightState> sxCLightState;
