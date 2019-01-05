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

#include "sxDriver/Client/State/sxCVertexBufferState.h"
#include "sxDriver/Client/State/sxCTopologyState.h"
#include "sxDriver/Client/Command/sxCDrawCommand.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
class sxCMesh
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Constructor. Initialize the mesh from the binary file passed as argument
	sxCMesh(sxChar const* a_szFileName);

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCMesh();

	void Render(class sxCContext& a_rContext);

protected:

	struct sxSSubMesh
	{
		sxCVertexBufferState m_oVertexBufferState;
		sxCTopologyState m_oTopologyState;
	};

private:

	//-------------------------------------------------------------------------------------------------------
	// Attributes
	typedef vector<sxSSubMesh*> sxCSubMeshVector;
	sxCSubMeshVector m_vpSubMeshes;

	sxCDrawCommand m_oDrawCommand;
};
