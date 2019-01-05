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
#include "sxIObject.h"
#include "sxDriver/Client/sxCFactorySelector.h"

//-----------------------------------------------------------------------------------------------------------
//
sxIObject::sxIObject(sxEFactoryObject::sxEValue a_eValue) :
    m_spServerObject(sxCFactorySelector::GetInstance().Instantiate(a_eValue))
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxIObject::~sxIObject()
{
    // Server object deletion is accomplished by the smart pointer
}
