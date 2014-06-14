/**************************************************************************

Yamaha SFG01/SFG05 emulation

TODO:
- Use a real YM2164 implementation for SFG05
- Implement midi functionality (YM2148)

**************************************************************************/

#include "emu.h"
#include "yamaha.h"


const device_type MSX_CART_SFG01 = &device_creator<msx_cart_sfg01>;
const device_type MSX_CART_SFG05 = &device_creator<msx_cart_sfg05>;


msx_cart_sfg::msx_cart_sfg(const machine_config &mconfig, const device_type type, const char *name, const char *tag, device_t *owner, UINT32 clock, const char *shortname)
	: device_t(mconfig, type, name, tag, owner, clock, shortname, __FILE__)
	, msx_cart_interface(mconfig, *this)
	, m_region_sfg(*this, "sfg")
	, m_ym2151(*this, "ym2151")
	, m_kbdc(*this, "kbdc")
	, m_ym2151_irq_state(CLEAR_LINE)
	, m_ym2148_irq_state(CLEAR_LINE)
	, m_rom_mask(0)
	, m_ym2148_irq_vector(0xff)
	, m_ym2148_external_irq_vector(0xff)
{
}


msx_cart_sfg01::msx_cart_sfg01(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock)
	: msx_cart_sfg(mconfig, MSX_CART_SFG01, "MSX Cartridge - SFG01", tag, owner, clock, "msx_cart_sfg01")
{
}


msx_cart_sfg05::msx_cart_sfg05(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock)
	: msx_cart_sfg(mconfig, MSX_CART_SFG05, "MSX Cartridge - SFG05", tag, owner, clock, "msx_cart_sfg05")
{
}


static MACHINE_CONFIG_FRAGMENT( msx_sfg )
	// YM2151 (OPM)
	// YM3012 (DAC)
	// YM2148 (MKS)

	MCFG_SPEAKER_STANDARD_STEREO("lspeaker", "rspeaker")
	MCFG_YM2151_ADD("ym2151", XTAL_4MHz)  // The SFG01 uses a YM2151, the SFG05 uses a YM2164
	MCFG_YM2151_IRQ_HANDLER(WRITELINE(msx_cart_sfg, ym2151_irq_w))
//	MCFG_YM2151_PORT_WRITE_HANDLER(WRITE8(msx_cart_sfg, port_w))
	MCFG_SOUND_ROUTE(0, "lspeaker", 0.80)
	MCFG_SOUND_ROUTE(1, "rspeaker", 0.80)

	MCFG_MSX_AUDIO_KBDC_PORT_ADD("kbdc", msx_audio_keyboards, NULL)
MACHINE_CONFIG_END


machine_config_constructor msx_cart_sfg::device_mconfig_additions() const
{
	return MACHINE_CONFIG_NAME( msx_sfg );
}


ROM_START( msx_sfg01 )
	ROM_REGION(0x4000, "sfg", 0)
	ROM_LOAD("sfg01.rom", 0x0, 0x4000, CRC(0995fb36) SHA1(434651305f92aa770a89e40b81125fb22d91603d))
ROM_END


const rom_entry *msx_cart_sfg01::device_rom_region() const
{
	return ROM_NAME( msx_sfg01 );
}


ROM_START( msx_sfg05 )
	ROM_REGION(0x8000, "sfg", 0)
	ROM_LOAD("sfg05.rom", 0x0, 0x8000, CRC(2425c279) SHA1(d956167e234f60ad916120437120f86fc8c3c321))
ROM_END


const rom_entry *msx_cart_sfg05::device_rom_region() const
{
	return ROM_NAME( msx_sfg05 );
}


void msx_cart_sfg::device_start()
{
	// Set rom mask
	m_rom_mask = m_region_sfg->bytes() - 1;

	// This should probably moved up in the bus/slot hierarchy for the msx driver
	cpu_device *maincpu = machine().device<cpu_device>("maincpu");
	device_execute_interface::static_set_irq_acknowledge_callback(*maincpu, device_irq_acknowledge_delegate(FUNC(msx_cart_sfg::irq_callback),this));
}


IRQ_CALLBACK_MEMBER(msx_cart_sfg::irq_callback)
{
	if (m_ym2148_irq_state == ASSERT_LINE)
	{
		return m_ym2148_irq_vector;
	}

	return m_ym2148_external_irq_vector;
}


WRITE_LINE_MEMBER(msx_cart_sfg::ym2151_irq_w)
{
	m_ym2151_irq_state = state ? ASSERT_LINE : CLEAR_LINE;
	check_irq();
}


WRITE_LINE_MEMBER(msx_cart_sfg::ym2148_irq_w)
{
	m_ym2148_irq_state = state ? ASSERT_LINE : CLEAR_LINE;
	check_irq();
}


void msx_cart_sfg::check_irq()
{
	if (m_ym2151_irq_state != CLEAR_LINE || m_ym2148_irq_state != CLEAR_LINE)
	{
		m_out_irq_cb(ASSERT_LINE);
	}
	else
	{
		m_out_irq_cb(CLEAR_LINE);
	}
}


READ8_MEMBER(msx_cart_sfg::read_cart)
{
	switch (offset & 0x3fff)
	{
		case 0x3ff0:     // YM-2151 status read
		case 0x3ff1:	 // YM-2151 status read mirror?
			return m_ym2151->status_r(space, 0);

		case 0x3ff2:     // YM-2148 keyboard column read
			return m_kbdc->read(space, 0);

		case 0x3ff3:     // YM-2148 --
		case 0x3ff4:     // YM-2148 --
		case 0x3ff5:     // YM-2148 MIDI UART data read register
		case 0x3ff6:     // YM-2148 MIDI UART status register
			break;
	}

	if (offset < 0x8000)
	{
		return m_region_sfg->u8(offset & m_rom_mask);
	}

	return 0xff;
}


WRITE8_MEMBER(msx_cart_sfg::write_cart)
{
	switch (offset & 0x3fff)
	{
		case 0x3ff0:     // YM-2151 register
			m_ym2151->register_w(space, 0, data);
			break;

		case 0x3ff1:    // YM-2151 data
			m_ym2151->data_w(space, 0, data);
			break;

		case 0x3ff2:   // YM-2148 write keyboard row
			m_kbdc->write(space, 0, data);
			break;

		case 0x3ff3:   // YM-2148 MIDI IRQ vector
			m_ym2148_irq_vector = data;
			break;

		case 0x3ff4:   // YM-2148 External IRQ vector
			m_ym2148_external_irq_vector = data;
			break;

		case 0x3ff5:   // YM-2148 MIDI UART data write register
		case 0x3ff6:   // YM-2148 MIDI UART command register
		default:
			logerror("msx_cart_sfg::write_cart: write %02x to %04x\n", data, offset);
			break;
	}
}

