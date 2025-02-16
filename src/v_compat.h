//**************************************************************************
//**
//** v_compat.h
//**
//**************************************************************************

#ifndef __V_COMPAT_H
#define __V_COMPAT_H

/* SetPalette */

#ifndef PLAYPAL_NUM
#define PLAYPAL_NUM		W_GetNumForName("PLAYPAL")
#endif

#define V_SetPaletteBase()	I_SetPalette((byte *)W_CacheLumpName("PLAYPAL", PU_CACHE))
#define V_SetPaletteShift(num)	I_SetPalette((byte *)W_CacheLumpNum(PLAYPAL_NUM, PU_CACHE) + (num)*768)

/* Minimal definitions for DrawPatch / DrawRawScreen stuff. */
#define BYTE_REF		byte*
#define PATCH_REF		patch_t*
#define INVALID_PATCH		NULL

/* Minimal definitions for CacheLumpName / CacheLumpNum stuff. */
#define ZR_ChangeTag			Z_ChangeTag
#define WR_CacheLumpName		W_CacheLumpName
#define WR_CacheLumpNum			W_CacheLumpNum

#endif	/* __V_COMPAT_H */
