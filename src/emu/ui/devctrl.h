/***************************************************************************

    ui/devctrl.h

    Device specific control (base class for tapectrl and bbcontrl)

    Copyright Nicola Salmoria and the MAME Team.
    Visit http://mamedev.org for licensing and usage restrictions.

***************************************************************************/

#pragma once

#ifndef __UI_DEVCTRL_H__
#define __UI_DEVCTRL_H__

template<class _DeviceType>
class ui_menu_device_control : public ui_menu
{
public:
	ui_menu_device_control(running_machine &machine, render_container *container, _DeviceType *device);

protected:
	_DeviceType *current_device() { return m_device; }
	int count() { return m_count; }
	
	int current_index();
	void previous();
	void next();

private:
	// device iterator
	typedef device_type_iterator<&device_creator<_DeviceType>, _DeviceType> iterator;

	_DeviceType *	m_device;
	int				m_count;
};


//-------------------------------------------------
//  ctor
//-------------------------------------------------

template<class _DeviceType>
ui_menu_device_control<_DeviceType>::ui_menu_device_control(running_machine &machine, render_container *container, _DeviceType *device)
	: ui_menu(machine, container)
{
	iterator iter(machine.root_device());
	m_device = device ? device : downcast<_DeviceType *>(iter.first());
	m_count = iter.count();
}


//-------------------------------------------------
//  current_index
//-------------------------------------------------

template<class _DeviceType>
int ui_menu_device_control<_DeviceType>::current_index()
{
	iterator iter(machine().root_device());
	return iter.indexof(*m_device);
}


//-------------------------------------------------
//  previous
//-------------------------------------------------

template<class _DeviceType>
void ui_menu_device_control<_DeviceType>::previous()
{
	// left arrow - rotate left through cassette devices
	if (m_device != NULL)
	{
		iterator iter(machine().root_device());
		int index = iter.indexof(*m_device);
		if (index > 0)
			index--;
		else
			index = m_count - 1;
		m_device = downcast<_DeviceType *>(iter.byindex(index));
	}
}


//-------------------------------------------------
//  next
//-------------------------------------------------

template<class _DeviceType>
void ui_menu_device_control<_DeviceType>::next()
{
	// right arrow - rotate right through cassette devices
	if (m_device != NULL)
	{
		iterator iter(machine().root_device());
		int index = iter.indexof(*m_device);
		if (index < m_count - 1)
			index++;
		else
			index = 0;
		m_device = downcast<_DeviceType *>(iter.byindex(index));
	}
}

#endif /* __UI_DEVCTRL_H__ */
