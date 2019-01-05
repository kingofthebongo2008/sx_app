//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//																											|
//								ShaderX7. Cross platform rendering thread. 2008.							|
//				Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.			|
//																											|
//	This program is free software. It is distributed in the hope that it will be useful, but without any	|
//	warranty, without even the implied warranty of merchantability or fitness for a particular purpose.		|
//																											|
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#include "sxKernel/sxPcHeader.h"
#include "sxCMesh.h"
#include "sxKernel/sxIMain.h"
#include "sxDriver/Client/sxCContext.h"

//-----------------------------------------------------------------------------------------------------------
//
sxCMesh::sxCMesh(sxChar const* a_szFileName)
{
	//----------------
	// Build file path which is relative to the eecutable path

	// Get executable path
	sxChar const* szExecutablePath = sxIMain::GetInstance().GetExecutablePath();

	// Concat path to the file name
	sxChar szFilePath[1024];
	sprintf_s(szFilePath, sizeof(szFilePath), "%s%s", szExecutablePath, a_szFileName);

	// Open mesh file
	FILE* pFile = NULL;
	sxValidate(	fopen_s(&pFile, szFilePath, "rb") == 0,
				"Unable to open file %s", a_szFileName);
	
	// Logging
	sxLog("Loading mesh file %s", a_szFileName);

	// Read sub-mesh count
	sxUInt uiSubMeshCount = 0;
	sxValidateNoText(	fread(	&uiSubMeshCount,
								sizeof(uiSubMeshCount),
								1,
								pFile)
						== 1);

	// Fills all sub-meshes
	sxUInt uiVertices = 0;
	sxUInt uiTriangles = 0;
	sxForEachUInt(uiSubMesh, uiSubMeshCount)
	{
		// Fetch sub-mesh
		sxSSubMesh& rSubMesh = sxNew(sxSSubMesh);

		//--------------------------------------------------
		// Read vertices directly in the vertex buffer state 
		{
			// Get the number of vertices
			sxUInt uiVertexCount = 0;
			sxValidateNoText(	fread(	&uiVertexCount,
										sizeof(uiVertexCount),
										1,
										pFile)
								== 1);

			// Increment vertex count
			uiVertices += uiVertexCount;

			// Lock the vertex buffer in order to get a writable access
			sxCVertexBufferState::sxCReadWriteLocker oVertexLock(rSubMesh.m_oVertexBufferState);

			// Fetch buffer of vertices
			sxCVertexBufferStateData::sxCVertices& rVertices = oVertexLock.GetData().GetVertices();

			// Resize a buffer of vertices
			rVertices.resize(uiVertexCount);

			// Read vertices
			sxValidateNoText(	fread(	&rVertices.front(),
										sizeof(sxCVertexBufferStateData::sxSVertex),
										uiVertexCount,
										pFile)
								== uiVertexCount);
		}

		//-----------------
		// Now read indices
		{
			// Get the number of indices
			sxUInt uiIndexCount = 0;
			sxValidateNoText(	fread(	&uiIndexCount,
										sizeof(uiIndexCount),
										1,
										pFile)
								== 1);

			// Increment index count
			uiTriangles += uiIndexCount /3;

			// Lock the vertex buffer in order to get a writable access
			sxCTopologyState::sxCReadWriteLocker oIndexLock(rSubMesh.m_oTopologyState);

			// Fetch buffer of vertices
			sxCTopologyStateData::sxCIndices& rIndices = oIndexLock.GetData().GetIndices();

			// Resize a buffer of vertices
			rIndices.resize(uiIndexCount);

			// Read vertices
			sxValidateNoText(	fread(	&rIndices.front(),
										sizeof(sxCTopologyStateData::sxIndex),
										uiIndexCount,
										pFile)
								== uiIndexCount);
		}

		// Finally add the sub-mesh to the list
		m_vpSubMeshes.push_back(&rSubMesh);
	}

	// Logging
	sxLog(	"Mesh file loaded, %d vertices, %d triangles, %d sub mesh(es)",
			uiVertices,
			uiTriangles,
			uiSubMeshCount);

	// Close the file
	fclose(pFile);
}

//-----------------------------------------------------------------------------------------------------------
//
sxCMesh::~sxCMesh()
{
	sxLog("Releasing mesh object");

	// Iterate and deallocate sub-meshes
	sxForEach(oSubMeshIterator, m_vpSubMeshes, sxCSubMeshVector)
	{
		// Fetch sub-mesh
		sxSSubMesh& rSubMesh = sxDereference(*oSubMeshIterator);

		// Delete the sub-mesh object
		sxDelete(rSubMesh);
	}
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCMesh::Render(sxCContext& a_rContext)
{
	// Iterate through each sub-mesh
	sxForEach(oMeshIterator, m_vpSubMeshes, sxCSubMeshVector)
	{
		// Fetch sub-mesh
		sxSSubMesh& rSubMesh = sxDereference(*oMeshIterator);

		// Bind states
		a_rContext.Bind(rSubMesh.m_oVertexBufferState);
		a_rContext.Bind(rSubMesh.m_oTopologyState);

		// Queue the draw command
		a_rContext.Queue(m_oDrawCommand);
	}
}
