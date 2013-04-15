/***************************************************************************

  primo.c

  Functions to emulate the video hardware of Primo.

  Krzysztof Strzecha

***************************************************************************/

#include "emu.h"
#include "includes/primo.h"



static void primo_draw_scanline(running_machine &machine,bitmap_ind16 &bitmap, int primo_scanline)
{
	primo_state *state = machine.driver_data<primo_state>();
	int x, i;
	UINT8 data;

	/* set up scanline */
	UINT16 *scanline = &bitmap.pix16(primo_scanline);

	/* address of current line in Primo video memory */
	const UINT8* primo_video_ram_line = (const UINT8*)state->m_maincpu->space(AS_PROGRAM).get_read_ptr(state->m_video_memory_base + 32 * primo_scanline);

	for (x=0; x<256; x+=8)
	{
		data = primo_video_ram_line[x/8];

		for (i=0; i<8; i++)
			scanline[x+i]=(data & (0x80>>i)) ? 1 : 0;

	}
}


UINT32 primo_state::screen_update_primo(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	int primo_scanline;

	for (primo_scanline=0; primo_scanline<192; primo_scanline++)
		primo_draw_scanline(machine(), bitmap, primo_scanline);
	return 0;
}
