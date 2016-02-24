//Landfill Definitions for Namespace Engine

#pragma once

namespace Engine
{
	static unsigned char _bNoMansLandFill = 0xAA; /* fill memory that should never be touched (guard bands) with this */
	static unsigned char _bAlignLandFill = 0xBB; /* fill space required for alignment routines with this*/
	static unsigned char _bFreedLandFill = 0xCC; /* fill memory returned by freed objects with this */
	static unsigned char _bNewAllocLandFill = 0xDD; /* fill newly allocated memory with this */
}
