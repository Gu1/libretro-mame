/***************************************************************************

    Atari vector hardware

***************************************************************************/

NVRAM_HANDLER( atari_vg );

/***************************************************************************
    DEVICE CONFIGURATION MACROS
***************************************************************************/

#define MDRV_ATARIVGEAROM_ADD(_tag) \
	MDRV_DEVICE_ADD(_tag, ATARIVGEAROM, 0)

#define MDRV_ATARIVGEAROM_REMOVE(_tag) \
	MDRV_DEVICE_REMOVE(_tag, ATARIVGEAROM)

/***************************************************************************
    FUNCTION PROTOTYPES
***************************************************************************/

READ8_DEVICE_HANDLER( atari_vg_earom_r );
WRITE8_DEVICE_HANDLER( atari_vg_earom_w );
WRITE8_DEVICE_HANDLER( atari_vg_earom_ctrl_w );

/* ----- device interface ----- */

#define ATARIVGEAROM DEVICE_GET_INFO_NAME(atari_vg_earom)
DEVICE_GET_INFO( atari_vg_earom );
