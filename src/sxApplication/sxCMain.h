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

#include "sxKernel/sxIMain.h"
#include "sxDriver/Client/sxCFactorySelector.h"
#include "sxDriver/Client/State/sxCRenderTargetState.h"
#include "sxDriver/Client/State/sxCLightState.h"
#include "sxDriver/Client/State/sxCMaterialState.h"
#include "sxDriver/Client/State/sxCCameraState.h"
#include "sxDriver/Client/State/sxCTransformState.h"
#include "sxDriver/Client/Command/sxCSwapCommand.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application main object. Creates application window and handles the loop.
class sxCMain : private sxIMain
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCMain(sxChar const* a_szExecutableFilePath);

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCMain();

	//-------------------------------------------------------------------------------------------------------
	// Main loop function. Does not exit until application ends.
	void Loop();

	//-------------------------------------------------------------------------------------------------------
	// Ge the Os specific application handle
	virtual void* GetApplicationHandle();

	//-------------------------------------------------------------------------------------------------------
	// Resizing notification
	virtual void Resize(sxUInt a_uiWidth, sxUInt a_uiHeight);

protected:

	//-------------------------------------------------------------------------------------------------------
	// Loop entering function. Called once before entering the main loop.
	void EnterLoop();

	//-------------------------------------------------------------------------------------------------------
	// Loop exiting function. Called once after existing the main loop.
	void ExitLoop();

	//-------------------------------------------------------------------------------------------------------
	// Frame update function. Called once per main loop per frame.
	void Update();

private:

	//-------------------------------------------------------------------------------------------------------
	// Update timer values
	void UpdateTime();
	
	//-------------------------------------------------------------------------------------------------------
	// Update scene values
	void UpdateScene(sxCTime const& a_rTime);

	//-------------------------------------------------------------------------------------------------------
	// Update light values
	void UpdateLights(sxCTime const& a_rTime);

	//-------------------------------------------------------------------------------------------------------
	// Frame render function.
	void Render();

	//-------------------------------------------------------------------------------------------------------
	// Render the scene
	void RenderScene();

	//-------------------------------------------------------------------------------------------------------
	// Render lights dummies as small spheres
	void RenderDummyLighs();

	//-------------------------------------------------------------------------------------------------------
	// Set material values
	void SetupMaterial();

	//-------------------------------------------------------------------------------------------------------
	// Set material values
	void SetupRenderTarget();

	//-------------------------------------------------------------------------------------------------------
	// Set camera values
	void SetupCamera();

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// Factory selector
	sxCFactoryRaiiSelector m_oGraphicApiSelector;

	// Render target state object
	sxCRenderTargetState m_oRenderTargetState;

	// Light state object
	sxCLightState m_oLightState;

	// Light state object
	sxCMaterialState m_oMaterialState;

	// Dummy lights transform state
	sxCTransformState m_oDummyTransformState;

	// Dummy lights transform state
	sxCMaterialState m_oDummyMaterialState;
	
	// Camera state
	sxCCameraState m_oCameraState;

	// Transform state
	sxCTransformState m_oTransformState;

	// Swap command object
	sxCSwapCommand m_oSwapCommand;

	// Timer objects
	sxCTime m_oCurrentTime;
	sxCTime m_oLastTime;
	sxCTime m_oDeltaTime;

	// Window object
	class sxCWindow& m_rWindow;

	// Driver Device object
	class sxCDevice& m_rDevice;

	// Driver Context
	class sxCContext& m_rContext;

	// Character mesh object
	class sxCMesh& m_rGolemMesh;

	// Light dummy mesh object
	class sxCMesh& m_rLightDummyMesh;
};