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
// Topology state data. This class is limited to indexed triangles primitive type for the example purpose.
class sxCTopologyStateData final : public sxIStateData
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor.
	sxCTopologyStateData() {}

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual clear function
	virtual void Clear();

	// Buffer of indices. Uses a vector this example purpose. A real solution would use a smart pointer that
	// would share buffer reading access across all sxCTopologyStateData, and duplicate the buffer on a write
	// operations.
	typedef sxUInt16 sxIndex;
	typedef vector<sxIndex> sxCIndices;

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual copy function
	virtual void Copy(sxICommandData const& a_rData);

	void SetIndices(sxCIndices const& a_rBuffer);
	sxCIndices& GetIndices();
	sxCIndices const& GetIndices() const;
private:

	// Buffer of indices
	sxCIndices m_vIndices;
};

// Type of the sxCTopologyState object built base on the state wrapper helper
typedef sxCTStateWrapper<	sxCTopologyStateData,
							sxEStateType::eTopology,
							sxEFactoryObject::eTopologyState> sxCTopologyState;
