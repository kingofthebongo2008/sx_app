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

#include "sxCFloat3.h"
#include "sxCFloat4.h"

//-----------------------------------------------------------------------------------------------------------
// Min function
template <typename t_Object>
inline t_Object sxMin(t_Object a_rLeft, t_Object a_rRight)
{
	return (a_rLeft < a_rRight) ? a_rLeft : a_rRight;
}

//-----------------------------------------------------------------------------------------------------------
// Max function
template <typename t_Object>
inline t_Object sxMax(t_Object a_rLeft, t_Object a_rRight)
{
	return (a_rLeft > a_rRight) ? a_rLeft : a_rRight;
}
