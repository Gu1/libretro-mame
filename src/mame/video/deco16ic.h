/*************************************************************************

    deco16ic.h

    Implementation of various Data East custom 16bit video ICs

**************************************************************************/

#pragma once
#ifndef __DECO16IC_H__
#define __DECO16IC_H__

#include "devcb.h"


/***************************************************************************
    TYPE DEFINITIONS
***************************************************************************/

typedef int (*deco16_bank_cb)( const int bank );


typedef struct _deco16ic_interface deco16ic_interface;
struct _deco16ic_interface
{
	const char         *screen;
	int                split;
	int                full_width12;

	int                trans_mask1, trans_mask2;
	int                col_base1, col_base2;
	int                col_mask1, col_mask2;
	deco16_bank_cb     bank_cb0, bank_cb1;
	int				   _8x8_gfxregion, _16x16_gfxregion;
};

DECLARE_LEGACY_DEVICE(DECO16IC, deco16ic);


/***************************************************************************
    DEVICE CONFIGURATION MACROS
***************************************************************************/

#define MCFG_DECO16IC_ADD(_tag, _interface) \
	MCFG_DEVICE_ADD(_tag, DECO16IC, 0) \
	MCFG_DEVICE_CONFIG(_interface)

/***************************************************************************
    DEVICE I/O FUNCTIONS
***************************************************************************/

WRITE16_DEVICE_HANDLER( deco16ic_pf1_data_w );
WRITE16_DEVICE_HANDLER( deco16ic_pf2_data_w );

READ16_DEVICE_HANDLER( deco16ic_pf1_data_r );
READ16_DEVICE_HANDLER( deco16ic_pf2_data_r );


WRITE16_DEVICE_HANDLER( deco16ic_pf_control_w );


READ16_DEVICE_HANDLER( deco16ic_pf_control_r );


WRITE32_DEVICE_HANDLER( deco16ic_pf1_data_dword_w );
WRITE32_DEVICE_HANDLER( deco16ic_pf2_data_dword_w );

READ32_DEVICE_HANDLER( deco16ic_pf1_data_dword_r );
READ32_DEVICE_HANDLER( deco16ic_pf2_data_dword_r );

WRITE32_DEVICE_HANDLER( deco16ic_pf_control_dword_w );

READ32_DEVICE_HANDLER( deco16ic_pf_control_dword_r );

WRITE16_DEVICE_HANDLER( deco16ic_nonbuffered_palette_w );
WRITE16_DEVICE_HANDLER( deco16ic_buffered_palette_w );
WRITE16_DEVICE_HANDLER( deco16ic_palette_dma_w );

WRITE16_DEVICE_HANDLER( deco16ic_priority_w );
READ16_DEVICE_HANDLER( deco16ic_priority_r );

READ16_DEVICE_HANDLER( deco16ic_71_r );

void deco16ic_print_debug_info(device_t *device, bitmap_t *bitmap);

void deco16ic_pf_update(device_t *device, const UINT16 *rowscroll_1_ptr, const UINT16 *rowscroll_2_ptr);

void deco16ic_tilemap_1_draw(device_t *device, bitmap_t *bitmap, const rectangle *cliprect, int flags, UINT32 priority);
void deco16ic_tilemap_2_draw(device_t *device, bitmap_t *bitmap, const rectangle *cliprect, int flags, UINT32 priority);

/* used by boogwing, nitrobal */
void deco16ic_tilemap_12_combine_draw(device_t *device, bitmap_t *bitmap, const rectangle *cliprect, int flags, UINT32 priority, int is_tattoo = false);

/* used by boogwing, dassault, nitrobal */
void deco16ic_clear_sprite_priority_bitmap(device_t *device);
void deco16ic_pdrawgfx(
		device_t *device,
		bitmap_t *dest,const rectangle *clip,const gfx_element *gfx,
		UINT32 code,UINT32 color,int flipx,int flipy,int sx,int sy,
		int transparent_color,UINT32 pri_mask,UINT32 sprite_mask,UINT8 write_pri,UINT8 alpha);

/* used by robocop2 */
void deco16ic_set_tilemap_colour_mask(device_t *device, int tmap, int mask);
void deco16ic_pf12_set_gfxbank(device_t *device, int small, int big);

/* used by captaven */
void deco16ic_set_pf1_8bpp_mode(device_t *device, int mode);

/* used by stoneage */
void deco16ic_set_scrolldx(device_t *device, int tmap, int size, int dx, int dx_if_flipped);

#endif
