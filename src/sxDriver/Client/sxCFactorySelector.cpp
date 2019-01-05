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
#include "sxCFactorySelector.h"
#include "sxDriver/Server/Interface/sxIServerFactory.h"

//-----------------------------------------------------------------------------------------------------------
// Register factory selector singleton object for instantiation
sxRegisterSingleton(sxCFactorySelector, sxERegisterPrimaryPriority::eHighest,
                                        sxERegisterSecondaryPriority::eHighest);

//-----------------------------------------------------------------------------------------------------------
//
sxCFactorySelector::sxCFactorySelector() :
    m_pSelectedFactory(NULL)
{
    // Allocate entries for all factories
    m_vpFactories.resize(sxEGraphicApi::eValuesCount);
}

//-----------------------------------------------------------------------------------------------------------
//
sxCFactorySelector::~sxCFactorySelector()
{
    m_vpFactories.clear();
    m_pSelectedFactory = NULL;
}

//-----------------------------------------------------------------------------------------------------------
//
sxIServerObject& sxCFactorySelector::Instantiate(sxEFactoryObject::sxEValue a_eValue)
{
    // Ensure a factory is selected
    sxAssert(m_pSelectedFactory != NULL, "No factory selected");

    // Forward instancing to the selected factory
    return m_pSelectedFactory->Instantiate(a_eValue);
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCFactorySelector::SelectFactory(sxEGraphicApi::sxEValue a_eApi)
{
    // Select factory from the registered factories according to a_eApi
    m_pSelectedFactory = m_vpFactories[a_eApi];

    // Ensure a factory was registered for that type
    sxAssert(m_pSelectedFactory != NULL, "No factory registerd for Api %d", a_eApi);
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCFactorySelector::UnselectFactory()
{
    // Unselect factory
    m_pSelectedFactory = NULL;
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCFactorySelector::RegisterFactory(   sxEGraphicApi::sxEValue a_eApi,
                                            sxIServerFactory& a_rFactory)
{
    sxAssert(m_vpFactories[a_eApi] == NULL, "A factory is already registered for Api %d", a_eApi);

    // Stores factory in the registration vector
    m_vpFactories[a_eApi] = &a_rFactory;
}
