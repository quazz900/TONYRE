/*****************************************************************************
**																			**
**			              Neversoft Entertainment.			                **
**																		   	**
**				   Copyright (C) 2000 - All Rights Reserved				   	**
**																			**
******************************************************************************
**																			**
**	Project:		Skate5 													**
**																			**
**	Module:			Gfx			 											**
**																			**
**	File name:		p_nxnewparticlemgr.h									**
**																			**
**	Created by:		3/24/03	-	SPG											**
**																			**
**	Description:	Xbox-specific new parametric particle system manager	**
**																			**
*****************************************************************************/

#ifndef __GFX_XBOX_P_NXNEWPARTICLEMGR_H__
#define __GFX_XBOX_P_NXNEWPARTICLEMGR_H__

/*****************************************************************************
**							  	  Includes									**
*****************************************************************************/

#include <gfx/nxnewparticlemgr.h>

/*****************************************************************************
**								   Defines									**
*****************************************************************************/


namespace Nx
{

/*****************************************************************************
**							Class Definitions								**
*****************************************************************************/

class CXboxNewParticleManager : public CNewParticleManager
{
protected:
	virtual	CNewParticle*	plat_create_particle( void );
};

/*****************************************************************************
**							 Private Declarations							**
*****************************************************************************/

/*****************************************************************************
**							  Private Prototypes							**
*****************************************************************************/

/*****************************************************************************
**							  Public Declarations							**
*****************************************************************************/

/*****************************************************************************
**							   Public Prototypes							**
*****************************************************************************/

/*****************************************************************************
**								Inline Functions							**
*****************************************************************************/

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

} // namespace Nx

#endif	// __GFX_XBOX_P_NXNEWPARTICLEMGR_H__


