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
#include "sxCControls.h"

//-----------------------------------------------------------------------------------------------------------
// Register sxCDriverStatistics object
sxRegisterSingleton(sxCControls,    sxERegisterPrimaryPriority::eNormal,
                                    sxERegisterSecondaryPriority::eNormal);

//-----------------------------------------------------------------------------------------------------------
//
sxCControls::sxCControls() :
    m_iaLightCount(5),
    m_iaBufferingMode(sxEBufferingMode::eTripleBuffer),
    m_iaRenderDummyLights(1),
    m_iaMainThreadDelay(0)
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxCControls::~sxCControls()
{
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCControls::Reset()
{
    m_iaLightCount = 5;
    m_iaBufferingMode = sxEBufferingMode::eTripleBuffer;
    m_iaRenderDummyLights = 1;
    m_iaMainThreadDelay = 0;
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCControls::SetBufferingMode(sxEBufferingMode::sxEValue a_eValue)
{
    m_iaBufferingMode = a_eValue;
}

//-----------------------------------------------------------------------------------------------------------
//
sxEBufferingMode::sxEValue sxCControls::GetBufferingMode() const
{
    return sxEBufferingMode::sxEValue(m_iaBufferingMode.GetValue());
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCControls::IncMainThreadDelay()
{
    if(m_iaMainThreadDelay < 100)
    {
        m_iaMainThreadDelay++;
    }
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCControls::DecMainThreadDelay()
{
    if(m_iaMainThreadDelay != 0)
    {
        m_iaMainThreadDelay--;
    }
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCControls::Control(sxUInt a_uiKey, sxBool a_bKeyUp)
{
    if(a_bKeyUp == false)
    {
        switch(a_uiKey)
        {
            //--------------
            // Light control
            case VK_ADD:
            case VK_OEM_PLUS:
            {
                SetLightCount(sxMin(GetMaxLightCount(), GetLightCount() + 1));
                break;
            }
            case VK_SUBTRACT:
            case '6': // key -
            {
                SetLightCount(sxMax(1u, GetLightCount() - 1));
                break;
            }
            // Main thread delay
            case 'A':
            {
                IncMainThreadDelay();
                break;
            }
            case 'Z':
            {
                DecMainThreadDelay();
                break;
            }
            default: break;
        }
    }
    else
    {
        switch(a_uiKey)
        {
            //-----------------------
            // Buffering mode control
            case VK_NUMPAD1:
            case '1':
            {
                SetBufferingMode(sxEBufferingMode::eNoBuffering);
                break;
            }
            case VK_NUMPAD2:
            case '2':
            {
                SetBufferingMode(sxEBufferingMode::eDoubleBuffer);
                break;
            }
            case VK_NUMPAD3:
            case '3':
            {
                SetBufferingMode(sxEBufferingMode::eTripleBuffer);
                break;
            }
            // Dummy light rendering
            case 'D':
            {
                EnableDummyLightRendering(!IsDummyLightRenderingEnabled());
                break;
            }
            // Reset
            case 'R':
            {
                Reset();
                break;
            }
            default: break;
        }
    }
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCControls::Format(ostringstream& a_rstrMessage)
{
    a_rstrMessage << "--------------------\n";
    a_rstrMessage << "Application controls\n";

    // Add buffering mode
    a_rstrMessage << "-Buffering mode (key 1/2/3): ";
    switch(GetBufferingMode())
    {
        case sxEBufferingMode::eNoBuffering: a_rstrMessage << "No buffering"; break;
        case sxEBufferingMode::eDoubleBuffer: a_rstrMessage << "Double buffering"; break;
        case sxEBufferingMode::eTripleBuffer: a_rstrMessage << "Triple buffering"; break;
        default: sxAssertUnreachableCode(); break;
    }
    a_rstrMessage << '\n';

    // Add main thread delay
    a_rstrMessage << "-Main thread delay (key A/Z): " << m_iaMainThreadDelay << "ms\n";

    // Add Light count
    a_rstrMessage << "-Light count (key +/-): " << m_iaLightCount.GetValue() << '\n';

    // Add dummy light enabled flag
    a_rstrMessage << "-Dummy lights (key D): ";
    if(IsDummyLightRenderingEnabled())
    {
        a_rstrMessage << "enabled";
    }
    else
    {
        a_rstrMessage << "disabled";
    }
    a_rstrMessage << '\n';
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCControls::Format(wostringstream& a_rstrMessage)
{
    a_rstrMessage << L"--------------------\n";
    a_rstrMessage << L"Application controls\n";

    // Add buffering mode
    a_rstrMessage << L"-Buffering mode (key 1/2/3): ";
    switch (GetBufferingMode())
    {
    case sxEBufferingMode::eNoBuffering: a_rstrMessage << L"No buffering"; break;
    case sxEBufferingMode::eDoubleBuffer: a_rstrMessage << L"Double buffering"; break;
    case sxEBufferingMode::eTripleBuffer: a_rstrMessage << L"Triple buffering"; break;
    default: sxAssertUnreachableCode(); break;
    }
    a_rstrMessage << L'\n';

    // Add main thread delay
    a_rstrMessage << L"-Main thread delay (key A/Z): " << m_iaMainThreadDelay << L"ms\n";

    // Add Light count
    a_rstrMessage << L"-Light count (key +/-): " << m_iaLightCount.GetValue() << L'\n';

    // Add dummy light enabled flag
    a_rstrMessage << L"-Dummy lights (key D): ";
    if (IsDummyLightRenderingEnabled())
    {
        a_rstrMessage << L"enabled";
    }
    else
    {
        a_rstrMessage << L"disabled";
    }
    a_rstrMessage << L'\n';
}