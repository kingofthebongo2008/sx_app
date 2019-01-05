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
#include "sxIMain.h"

//-----------------------------------------------------------------------------------------------------------
//
sxIMain::sxIMain(sxChar const* a_szExecutableFilePath) :
	m_szExecutablePath(NULL)
{
	//-------------------
	// Build program path

	// Allocate path
	sxSizeT const stAllocSize = 1024 * sizeof(sxChar);
	m_szExecutablePath = (sxChar*)sxMalloc(stAllocSize);

	// Initialize it
	if(a_szExecutableFilePath == NULL)
	{
		strcpy_s(m_szExecutablePath, stAllocSize, ".\\");
	}
	else
	{
		// Remove file/extension..
		sxChar const* pcFolderEnd = strrchr(a_szExecutableFilePath, '\\');
		sxAssert(pcFolderEnd != NULL, "Bad program path construction");

		// Set '\0' for every character
		sxMemSet(m_szExecutablePath, 0, stAllocSize);

		// Copy path
		strncpy_s(m_szExecutablePath, stAllocSize, a_szExecutableFilePath, (pcFolderEnd - a_szExecutableFilePath) + 1);
	}

	//-------------------------------------------------
	// Initilalize all registered classes and functions
	sxCRegistrer::CallInitialize();
}

//-----------------------------------------------------------------------------------------------------------
//
sxIMain::~sxIMain()
{
	// Destroy all registered classes and functions
	sxCRegistrer::CallDestroy();

	// Free program path
	sxFree(m_szExecutablePath);
	m_szExecutablePath = NULL;
}
