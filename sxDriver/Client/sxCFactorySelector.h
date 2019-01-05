//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//                                                                                                          |
//                              ShaderX7. Cross platform rendering thread. 2008.                            |
//              Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.         |
//                                                                                                          |
//  This program is free software. It is distributed in the hope that it will be useful, but without any    |
//  warranty, without even the implied warranty of merchantability or fitness for a particular purpose.     |
//                                                                                                          |
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver factory enumeration
sxEnumBegin(sxEGraphicApi)
    eD3d9,
    eD3d11,
    eD3d12,
    eUTest,
sxEnumEnd()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver factory object enumeration
sxEnumBegin(sxEFactoryObject)
    eDevice,

    // States
    eRenderTargetState,
    eVertexBufferState,
    eTopologyState,
    eMaterialState,
    eLightState,
    eCameraState,
    eTransformState,

    // Commands
    eSwapCommand,
    eDrawCommand,

#ifdef __sxBuildUTest
    eUTestCommand,
    eUTestCheckCommand,
    eUTestState,
#endif // __sxBuildUTest
sxEnumEnd()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implement factory type selection from the sxEGraphicApi enumeration. This minimal factory mechanism allow
// to instantiate a server object based on object type (sxEFactoryObject) and the selected graphic API 
// (sxEGraphicApi).
class sxCFactorySelector : public sxITSingleton<sxCFactorySelector>
{
public:

    //-------------------------------------------------------------------------------------------------------
    // Constructor. No factory is selected by default
    sxCFactorySelector();

    //-------------------------------------------------------------------------------------------------------
    // Destructor
    virtual ~sxCFactorySelector();

    //-------------------------------------------------------------------------------------------------------
    // 
    void SelectFactory(sxEGraphicApi::sxEValue a_eApi);

    //-------------------------------------------------------------------------------------------------------
    // 
    void UnselectFactory();

private:

    // sxIObject is friend in order to call Instantiate when the server object instantiation is required.
    friend class sxIObject;

    //-------------------------------------------------------------------------------------------------------
    // Instantiate a server object from a client one (using client name as the identifier)
    class sxIServerObject& Instantiate(sxEFactoryObject::sxEValue a_eValue);

    // sxIServerFactory in order to register itself
    friend class sxIServerFactory;

    //-------------------------------------------------------------------------------------------------------
    // Register a factory object for a_eApi
    void RegisterFactory(sxEGraphicApi::sxEValue a_eApi, sxIServerFactory& a_rFactory);

    //-------------------------------------------------------------------------------------------------------
    // Attributes

    // Vector of registered factory.
    typedef vector<sxIServerFactory*> sxCServerFactoryVector;
    sxCServerFactoryVector m_vpFactories;

    // Selected factory.
    sxIServerFactory* m_pSelectedFactory;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Raii object that select a factory type from the sxEGraphicApi enumeration and reset it on destruction.
// This minimal factory mechanism allow to instantiate a server object based on object type (sxEFactoryObject)
// and the selected graphic API (sxEGraphicApi).
class sxCFactoryRaiiSelector
{
public:

    //----------------------------------------------------------------------------------------------------------
    // Constructor. Select a factory.
    sxCFactoryRaiiSelector(sxEGraphicApi::sxEValue a_eApi)
    {
        sxCFactorySelector::GetInstance().SelectFactory(a_eApi);
    }

    //----------------------------------------------------------------------------------------------------------
    // Destructor. Deselect the factory
    ~sxCFactoryRaiiSelector()
    {
        sxCFactorySelector::GetInstance().UnselectFactory();
    }
};
