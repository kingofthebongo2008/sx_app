//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//                                                                                                          |
//                              ShaderX7. Cross platform rendering thread. 2008.                            |
//              Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.         |
//                                                                                                          |
//  This program is free software. It is distributed in the hope that it will be useful, but without any    |
//  warranty, without even the implied warranty of merchantability or fitness for a particular purpose.     |
//                                                                                                          |
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#include "pch.h"
#include "sxCMain.h"
#include "sxCWindow.h"
#include "sxCControls.h"
#include "sxCMesh.h"

#include "sxDriver/Client/sxCDevice.h"
#include "sxDriver/Client/sxCContext.h"

//-----------------------------------------------------------------------------------------------------------
//
sxCMain::sxCMain(sxChar const* a_szExecutableFilePath) :
    sxIMain(a_szExecutableFilePath),
    m_oGraphicApiSelector(sxEGraphicApi::eD3d9),
    m_rWindow(sxNew(sxCWindow)),
    m_rDevice(sxNew(sxCDevice)),
    m_rContext(sxNew(sxCContext, m_rDevice)),
    m_rGolemMesh(sxNew(sxCMesh, "Media/golem.bin")),
    m_rLightDummyMesh(sxNew(sxCMesh, "Media/sphere.bin"))
{
    // Get generic system info in order to warn the user in case of a single core hardawre
    SYSTEM_INFO oSystemInfo;
    GetSystemInfo(&oSystemInfo);
    if(oSystemInfo.dwNumberOfProcessors == 1)
    {
        sxLog(  "************************************************************\n"
                "WARNING: Single processor core detected.\n"
                "         Asynchronous buffering modes are not recommended.\n"
                "************************************************************");
    }
}

//-----------------------------------------------------------------------------------------------------------
//
sxCMain::~sxCMain()
{
    sxDelete(m_rGolemMesh);
    sxDelete(m_rLightDummyMesh);
    sxDelete(m_rContext);
    sxDelete(m_rDevice);
    sxDelete(m_rWindow);
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCMain::EnterLoop()
{
    sxLog("Entering main loop");

    // Initialize render target (width, height, ...)
    SetupRenderTarget();

    // Initialize the material object (color, coefficient, ...)
    SetupMaterial();

    // Initialize camera (position, focal, ...)
    SetupCamera();
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCMain::ExitLoop()
{
    sxLog("Exiting main loop");
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCMain::Update()
{
    //----------------------------
    // Handle buffering mode changes

    // Get desired mode
    sxEBufferingMode::sxEValue eBufferingMode = sxCControls::GetInstance().GetBufferingMode();

    // Compare with the current and change if required
    if(eBufferingMode != m_rContext.GetBufferingMode())
    {
        // Change it
        m_rContext.SetBufferingMode(eBufferingMode);
    }

    //-------
    // Update

    // Introduce a delay in the main thread loop
    sxCThread::Sleep(sxCControls::GetInstance().GetMainThreadDelay());

    // Compute time
    UpdateTime();

    // Set light colors and positions
    UpdateLights(m_oCurrentTime);

    // Update scene values
    UpdateScene(m_oCurrentTime);

    // Render the scene
    Render();
}

//-------------------------------------------------------------------------------------------------------
//
void sxCMain::UpdateScene(sxCTime const& a_rTime)
{
    // Lock the data, previous values can be discarded as everything will be overwritten
    sxCTransformState::sxCReadWriteLocker oLocker(m_oTransformState, true);

    // Fetch the transform state data 
    sxCTransformStateData& rData = oLocker.GetData();

    // Build a rotation matrix
    //      |  cos(A)  0   sin(A)  0 |
    // M =  |  0       1   0       0 |
    //      | -sin(A)  0   cos(A)  0 |
    //      |  0       0   0       1 |
    
    sxFloat const fSin = sinf(a_rTime.GetSeconds());
    sxFloat const fCos = cosf(a_rTime.GetSeconds());

    rData.SetRight(sxCFloat3(fCos, 0, -fSin));
    rData.SetUp(sxCFloat3(0, 1, 0));
    rData.SetAt(sxCFloat3(fSin, 0, fCos));

    // Translate the scene down a bit
    rData.SetPos(sxCFloat3(0, -5, 0));
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCMain::UpdateLights(sxCTime const& a_rTime)
{
    // Get the number of lights from the control object
    sxUInt const uiLightCount = sxCControls::GetInstance().GetLightCount();

    // Get the maximum number of lights from the control object
    sxUInt const uiMaxLightCount = sxCControls::GetInstance().GetMaxLightCount();

    // Radius of the sphere the light are moving on
    sxFloat const fRadius = 8.f;

    // Lock the data, previous values can be discarded as default values doesn't matter there
    sxCLightState::sxCReadWriteLocker oLocker(m_oLightState);

    // Fetch the light state data 
    sxCLightStateData& rData = oLocker.GetData();

    // Iterate uiLightCount times and add a light
    sxForEachUInt(uiLight, uiLightCount)
    {
        // Build a light
        sxCLight oLight;

        //-------------------------
        // Color is fixed per light
        oLight.SetColor(sxCFloat3(  fmodf(sxFloat(uiLight * 9) / sxFloat(uiMaxLightCount), 1.f),
                                    1.f - fmodf((sxFloat(uiLight * 9) / sxFloat(uiMaxLightCount)), 1.f),
                                    fmodf(2.f * sxFloat(uiLight) / sxFloat(uiMaxLightCount), 1.f)));

        oLight.SetAmbient(0.1f);
        oLight.SetDiffuse(1.f);
        oLight.SetSpecular(1.f);
        oLight.SetAttenuation(0.2f);

        //---------------------------------------------------------
        // Compute light position base on the time and light number
        sxFloat const fTime = a_rTime.GetSeconds();

        // Get a perturbation integer value
        sxInt const iPerturb = -sxInt(uiMaxLightCount / 2) + sxInt(uiLight);
        sxFloat const fScalar = fTime * sxFloat((iPerturb == 0)? 1 : iPerturb);

        // Build a position
        sxCFloat3 const f3Pos(cosf(fScalar * 0.04f), cosf(sxFloat(iPerturb)), sinf(fScalar * -0.1f));

        // Normalize such that all the lights are on a sphere
        sxFloat const fInvLen = fRadius / sqrtf(f3Pos.m_fX * f3Pos.m_fX + f3Pos.m_fY * f3Pos.m_fY + f3Pos.m_fZ * f3Pos.m_fZ);
        sxCFloat3 const f3NormPos(f3Pos.m_fX * fInvLen, f3Pos.m_fY * fInvLen, f3Pos.m_fZ * fInvLen);

        // Update light position
        oLight.SetPosition(f3NormPos);

        // Add the light to the data
        rData.AddLight(oLight);
    }
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCMain::UpdateTime()
{
    // Update last time
    m_oLastTime = m_oCurrentTime;

    // Compute new time
    m_oCurrentTime = sxCTime::GetApplicationTime();

    // Deduce delta time
    m_oDeltaTime = sxCTime(m_oCurrentTime.GetSeconds() - m_oLastTime.GetSeconds());
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCMain::Render()
{
    // Begin the rendering context. Begin can fail if the server device isn't ready.
    if(m_rContext.Begin())
    {
        //------------------
        // Prepare rendering

        // Bind the render target we want to draw in
        m_rContext.Bind(m_oRenderTargetState);

        // Bind camera
        m_rContext.Bind(m_oCameraState);

        //-------
        // Render

        // Render the scene, ie : characters
        RenderScene();

        // Render dummy light object
        RenderDummyLighs();

        //--------------
        // Ends rendering

        // Finally queue the swap command
        m_rContext.Queue(m_oSwapCommand);

        // End of the rendering context
        m_rContext.End();
    }
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCMain::RenderScene()
{
    // Bind lights
    m_rContext.Bind(m_oLightState);

    // Bind material
    m_rContext.Bind(m_oMaterialState);

    // Bind transform
    m_rContext.Bind(m_oTransformState);

    // Render the foreground mesh
    m_rGolemMesh.Render(m_rContext);
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCMain::RenderDummyLighs()
{
    // Early out if dummy lights are disabled
    if(sxCControls::GetInstance().IsDummyLightRenderingEnabled() == false)
    {
        return;
    }

    //----------------------------------------------------------------
    // Disable lights in order to use the emissive material value only
    // This is done by binding the default light state, using the state cache reset function
    m_rContext.Reset(sxEStateType::eLight);

    //----------------------------------------------------------
    // Lock the data for reading in order to access light values
    sxCLightState::sxCReadOnlyLocker oLocker(m_oLightState);

    // Fetch the light state data 
    sxCLightStateData const& rData = oLocker.GetData();

    // Fetch light vector
    sxCLightStateData::sxCLightVector const& rvLights = rData.GetLights();

    //--------------------------------
    // Iterate through the light vector and display every light
    sxForEachC(oLightIterator, rvLights, sxCLightStateData::sxCLightVector)
    {
        // Fetch light
        sxCLight const& rLight = *oLightIterator;

        //--------------------------------------------------
        // Set the transform according to the light position
        {
            // Lock the data, previous values can be discarded as everything will be overwritten
            sxCTransformState::sxCReadWriteLocker oLocker(m_oDummyTransformState, true);

            // Fetch the transform state data 
            sxCTransformStateData& rData = oLocker.GetData();

            // Get light position
            sxCFloat3 const& rf3Position = rLight.GetPosition();

            // Set the transform to the light position
            rData.SetPos(rf3Position);

            // Scale light objects down
            sxFloat const fScaleFactor = 0.3f;
            rData.SetRight(sxCFloat3( fScaleFactor, 0, 0));
            rData.SetUp(sxCFloat3( 0, fScaleFactor, 0));
            rData.SetAt(sxCFloat3( 0, 0, fScaleFactor));
        }

        // Bind the modified transform state
        m_rContext.Bind(m_oDummyTransformState);

        //--------------------------------------------------
        // Set the material according to the light color
        {
            // Lock the data, previous values can be discarded as everything will be overwritten
            sxCMaterialState::sxCReadWriteLocker oLocker(m_oDummyMaterialState);

            // Fetch the material state data 
            sxCMaterialStateData& rData = oLocker.GetData();

            // Get light color
            sxCFloat3 const& rf3Color = rLight.GetColor();

            // Sets color information, use emissive only
            rData.SetColor(sxCFloat4(rf3Color, 1.f));
            rData.SetAmbient(0);
            rData.SetDiffuse(0);
            rData.SetSpecular(0);
            rData.SetEmissive(1);
            rData.SetSpecularPower(0);
        }

        // Bind the modified material state
        m_rContext.Bind(m_oDummyMaterialState);

        //---------------------
        // Draw the light dummy
        m_rLightDummyMesh.Render(m_rContext);
    }
}

//-------------------------------------------------------------------------------------------------------
//
void sxCMain::SetupMaterial()
{
    // Lock the data, previous values can be discarded as everything will be overwritten
    sxCMaterialState::sxCReadWriteLocker oLocker(m_oMaterialState);

    // Fetch the material state data 
    sxCMaterialStateData& rData = oLocker.GetData();

    // Sets color information
    rData.SetColor(sxCFloat4(1.f, 1.f, 1.f, 1.f));
    rData.SetAmbient(0.1f);
    rData.SetDiffuse(0.7f);
    rData.SetSpecular(0.7f);
    rData.SetEmissive(0.f);
    rData.SetSpecularPower(10.f);
}

//-------------------------------------------------------------------------------------------------------
//
void sxCMain::SetupRenderTarget()
{
    // Lock the data but don't discard previous values
    sxCRenderTargetState::sxCReadWriteLocker oLocker(m_oRenderTargetState, false);

    // Fetch the render target state data 
    sxCRenderTargetStateData& rData = oLocker.GetData();

    // Sets color information
    rData.SetColor(sxCFloat4(0, 127, 127, 0));
}

//-------------------------------------------------------------------------------------------------------
//
void sxCMain::SetupCamera()
{
    // Lock the data but don't discard previous values
    sxCCameraState::sxCReadWriteLocker oLocker(m_oCameraState, false);

    // Fetch the camera state data 
    sxCCameraStateData& rData = oLocker.GetData();

    // Initialize view information
    rData.SetEye(sxCFloat3(0, 0, -10));
    rData.SetAt(sxCFloat3(0, 0, 0));
    rData.SetUp(sxCFloat3(0, 1, 0));

    // Initialize projection information
    rData.SetFocalAngle(90);
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCMain::Resize(sxUInt a_uiWidth, sxUInt a_uiHeight)
{
    // Lock the data but don't discard previous values
    sxCRenderTargetState::sxCReadWriteLocker oLocker(m_oRenderTargetState, false);

    // Fetch the render target state data 
    sxCRenderTargetStateData& rData = oLocker.GetData();

    // Update size information
    rData.SetWidth(a_uiWidth);
    rData.SetHeight(a_uiHeight);
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCMain::Loop()
{
    // Do pre loop code
    EnterLoop();

    sxBool bLoop = true;

    // Loop until exit is asked
    while( true == bLoop)
    {
        sxBool bGotMsg;
        MSG oMessage;

        if(m_rWindow.IsActive() == true)
        {
            // The PeekMessage function dispatches incoming sent messages, checks the thread message queue 
            // for a posted message, and retrieves the message (if any exist). 
            bGotMsg = (PeekMessage(&oMessage, NULL, 0, 0, PM_REMOVE | PM_NOYIELD) != FALSE);
        }
        else
        {
            // Unlike GetMessage, the PeekMessage function does not wait for a message to be posted before 
            // returning. So GetMessage is used when the application does not have the focus. 
            bGotMsg = (GetMessage(&oMessage, NULL, 0, 0) >= 0);
        }

        // Handle messages
        if(bGotMsg == true)
        {
            // Detect a Quit message to stop the loop
            if(oMessage.message == WM_QUIT)
            {
                // Exit the while loop
                bLoop = false;
            }

            // Translate and dispatch messages
            TranslateMessage(&oMessage);
            DispatchMessage(&oMessage);
        }
        else
        {
            // Do the per frame update
            Update();
        }
    }

    // Do post loop code
    ExitLoop();
}

//-----------------------------------------------------------------------------------------------------------
//
void* sxCMain::GetApplicationHandle()
{
    return m_rWindow.GetDisplayHandle();
}
