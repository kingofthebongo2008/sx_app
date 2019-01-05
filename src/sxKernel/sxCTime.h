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
// Timer object. Compute and store application time.
class sxCTime
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor, initialize time to 0
	sxCTime() :
		m_fSeconds(0.f)
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Copy constructor, initialize time to a_fSeconds
	explicit sxCTime(sxFloat a_fSeconds) :
		m_fSeconds(a_fSeconds)
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Copy constructor.
	sxCTime(sxCTime const& a_rTime) :
		m_fSeconds(a_rTime.m_fSeconds)
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Assignment operator
	void operator = (sxCTime const& a_rTime)
	{
		m_fSeconds = a_rTime.m_fSeconds;
	}
	
	//-------------------------------------------------------------------------------------------------------
	// Compute current application time
	static sxCTime GetApplicationTime();

	//-------------------------------------------------------------------------------------------------------
	// Get stored time in seconds
	sxFloat GetSeconds() const {return m_fSeconds;}

	//-------------------------------------------------------------------------------------------------------
	// Get stored time in milliseconds
	sxFloat GetMilliSeconds() const {return m_fSeconds * 1000.f;}
	
private:

	// Stored time
	sxFloat m_fSeconds;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Frame per second. Update a fps counter every time a certain time has elapsed.
class sxCFpsCounter : private sxINonCopyable
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Set the mean time (time between every update) to a_fMeanTime.
	sxCFpsCounter(sxFloat a_fMeanTime = 0.5f);

	//-------------------------------------------------------------------------------------------------------
	// Update to next frame, compute the fps if mean time has been elapsed
	void Update(sxFloat a_fFrameTime);

	//-------------------------------------------------------------------------------------------------------
	// Get the last computed fps
	sxUInt GetFramePerSecond() const {return m_uiFramePerSecond;}

private:

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// Cummulated time since last fps reset
	sxFloat m_fCumulatedTime;

	// Time elapsed between every fps counter updates
	sxFloat m_fMeanTime;

	// Frame count between every fps counter updates
	sxUInt m_uiFrameCount;

	// Last computed fps counter
	sxUInt m_uiFramePerSecond;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Frame per second automatique counter. Update a fps counter every time a certain time has elapsed.
class sxCFpsAutoCounter : private sxCFpsCounter
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Set the mean time (time between every update) to a_fMeanTime.
	sxCFpsAutoCounter(sxFloat a_fMeanTime = 1);

	//-------------------------------------------------------------------------------------------------------
	// Update to next frame, compute the fps if mean time has been elapsed
	void Update();

	//-------------------------------------------------------------------------------------------------------
	// Get the last computed fps
	sxUInt GetFramePerSecond() const {return sxCFpsCounter::GetFramePerSecond();}

private:

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// Time at which the fps was updated last
	sxFloat m_fLastTime;
};