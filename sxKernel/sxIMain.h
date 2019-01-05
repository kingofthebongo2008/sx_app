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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application "main" interface
class sxIMain : public sxITSingleton<sxIMain>
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Instantiate all registered objects
	sxIMain(sxChar const* a_szExecutableFilePath);

	//-------------------------------------------------------------------------------------------------------
	// Destructor. Deinstantiate all registered objects
	~sxIMain();

	//-------------------------------------------------------------------------------------------------------
	// Get the Os specific application handle
	virtual void* GetApplicationHandle() = 0;

	//-------------------------------------------------------------------------------------------------------
	// Resizing notification
	virtual void Resize(sxUInt a_uiWidth, sxUInt a_uiHeight) = 0;

	//-------------------------------------------------------------------------------------------------------
	// Get the executable path
	sxChar const* GetExecutablePath() const {return m_szExecutablePath;}

private:

	// executable path
	sxChar* m_szExecutablePath;
};
