//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//																											|
//						Guillaume Blanc. ELB (Etranges Libellules). Lyon FRANCE. 2008.						|
//								ShaderX7. Cross platform rendering thread									|
//																											|
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#pragma once

#include "sxDriver/Client/sxIState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vertex buffer state data. Stores vertices information in a fixed format for simplicity.
class sxCVertexBufferStateData : public sxIStateData
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor.
	sxCVertexBufferStateData()
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual clear function
	virtual void Clear();

	// Define vertex type. This is simplified for the example purpose to a fixed type but should rather be
	// a flexible format.
	struct sxSVertex
	{
		// Vertex position
		sxFloat m_f3Eye[3];

		// Vertex normal
		sxFloat m_fNormal[3];
	};

	// Buffer of vertices. Uses a vector this example purpose. A real solution would use a smart pointer that
	// would share buffer reading access across all sxCVertexBufferStateData, and duplicate the buffer on a 
	// write operations.
	typedef vector<sxSVertex> sxCVertices;

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual copy function
	virtual void Copy(sxICommandData const& a_rData);

	void SetVertices(sxCVertices const& a_rBuffer);
	sxCVertices const& GetVertices() const;
	sxCVertices& GetVertices();

private:

	// Buffer of vertices
	sxCVertices m_vVertices;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type of the sxCVertexBufferState object built base on the state wrapper helper
typedef sxCTStateWrapper<	sxCVertexBufferStateData,
							sxEStateType::eVertexBuffer,
							sxEFactoryObject::eVertexBufferState> sxCVertexBufferState;
