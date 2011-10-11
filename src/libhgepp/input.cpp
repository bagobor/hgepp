/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * Core functions implementation: input
 */
#include "hge_impl.h"

namespace hge
{

char *KeyNames[] =
	{ "?", "Left Mouse Button", "Right Mouse Button", "?", "Middle Mouse Button", "?", "?", "?",
	"Backspace", "Tab", "?", "?", "?", "Enter", "?", "?", "Shift", "Ctrl", "Alt", "Pause",
	"Caps Lock", "?", "?", "?", "?", "?", "?", "Escape", "?", "?", "?", "?", "Space",
	"Page Up", "Page Down", "End", "Home", "Left Arrow", "Up Arrow", "Right Arrow",
	"Down Arrow", "?", "?", "?", "?", "Insert", "Delete", "?", "0", "1", "2", "3", "4", "5",
	"6", "7", "8", "9", "?", "?", "?", "?", "?", "?", "?", "A", "B", "C", "D", "E", "F", "G",
	"H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y",
	"Z", "Left Win", "Right Win", "Application", "?", "?", "NumPad 0", "NumPad 1", "NumPad 2",
	"NumPad 3", "NumPad 4", "NumPad 5", "NumPad 6", "NumPad 7", "NumPad 8", "NumPad 9",
	"Multiply", "Add", "?", "Subtract", "Decimal", "Divide", "F1", "F2", "F3", "F4", "F5",
	"F6", "F7", "F8", "F9", "F10", "F11", "F12", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "Num Lock", "Scroll Lock", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "Semicolon", "Equals", "Comma", "Minus", "Period", "Slash", "Grave", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "Left bracket", "Backslash", "Right bracket", "Apostrophe",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?"
	};

bool HGE_CALL HGE_Impl::Input_GetEvent(input_event_t * evt)
{
	//CInputEventList *eptr;

	if (! m_input_queue.empty())
	{
		//eptr = m_input_queue;
		//memcpy(evt, &eptr->evt, sizeof(hgeInputEvent));
		//m_input_queue = eptr->next;
		//delete eptr;
		*evt = m_input_queue.front();
		m_input_queue.pop_front();
		return true;
	}

	return false;
}

void HGE_CALL HGE_Impl::Input_GetMousePos(float *x, float *y)
{
	*x = m_xpos;
	*y = m_ypos;
}

void HGE_CALL HGE_Impl::Input_SetMousePos(float x, float y)
{
	POINT pt;
	pt.x = (long) x;
	pt.y = (long) y;
	ClientToScreen(m_hwnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

int HGE_CALL HGE_Impl::Input_GetMouseWheel()
{
	return m_zpos;
}

bool HGE_CALL HGE_Impl::Input_IsMouseOver()
{
	return m_mouse_over;
}

bool HGE_CALL HGE_Impl::Input_GetKeyState(key_code_t key)
{
	return ((GetKeyState(key) & 0x8000) != 0);
}

bool HGE_CALL HGE_Impl::Input_KeyDown(key_code_t key)
{
	return (m_key_states[key] & 1) != 0;
}

bool HGE_CALL HGE_Impl::Input_KeyUp(key_code_t key)
{
	return (m_key_states[key] & 2) != 0;
}

char* HGE_CALL HGE_Impl::Input_GetKeyName(key_code_t key)
{
	return KeyNames[key];
}

key_code_t HGE_CALL HGE_Impl::Input_GetKey()
{
	return m_vkey;
}

int HGE_CALL HGE_Impl::Input_GetChar()
{
	return m_char;
}

//////// Implementation ////////


void HGE_Impl::_InputInit()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hwnd, &pt);
	m_xpos = (float) pt.x;
	m_ypos = (float) pt.y;

	memset(&m_key_states, 0, sizeof(m_key_states));
}

void HGE_Impl::_UpdateMouse()
{
	POINT pt;
	RECT rc;

	GetCursorPos(&pt);
	GetClientRect(m_hwnd, &rc);
	MapWindowPoints(m_hwnd, NULL, (LPPOINT) &rc, 2);

	if (m_captured || (PtInRect(&rc, pt) && WindowFromPoint(pt) == m_hwnd))
		m_mouse_over = true;
	else
		m_mouse_over = false;
}

void HGE_Impl::_BuildEvent(event_type_t type, key_code_t key, int scan, event_flags_t flags, int x, int y)
{
	//CInputEventList *last, *eptr = new CInputEventList;
	input_event_t new_event;
	unsigned char kbstate[256];
	POINT pt;

	new_event.type = type;
	new_event.chr = 0;
	pt.x = x;
	pt.y = y;

	GetKeyboardState(kbstate);
	if (type == INPUT_KEYDOWN)
	{
		if (!flags.repeat)
			m_key_states[key] |= 1;
		ToAscii(key, scan, kbstate, (unsigned short *) &new_event.chr, 0);
	}
	if (type == INPUT_KEYUP)
	{
		m_key_states[key] |= 2;
		ToAscii(key, scan, kbstate, (unsigned short *) &new_event.chr, 0);
	}
	if (type == INPUT_MOUSEWHEEL)
	{
		new_event.key = HGEK_NO_KEY;
		new_event.wheel = key;
		ScreenToClient(m_hwnd, &pt);
	}
	else
	{
		new_event.key = key;
		new_event.wheel = 0;
	}

	if (type == INPUT_MBUTTONDOWN)
	{
		m_key_states[key] |= 1;
		SetCapture(m_hwnd);
		m_captured = true;
	}
	if (type == INPUT_MBUTTONUP)
	{
		m_key_states[key] |= 2;
		ReleaseCapture();
		Input_SetMousePos(m_xpos, m_ypos);
		pt.x = (int) m_xpos;
		pt.y = (int) m_ypos;
		m_captured = false;
	}

	if (kbstate[VK_SHIFT] & 0x80)
		flags.shift = true;
	if (kbstate[VK_CONTROL] & 0x80)
		flags.ctrl = true;
	if (kbstate[VK_MENU] & 0x80)
		flags.alt = true;
	if (kbstate[VK_CAPITAL] & 0x1)
		flags.caps_lock = true;
	if (kbstate[VK_SCROLL] & 0x1)
		flags.scroll_lock = true;
	if (kbstate[VK_NUMLOCK] & 0x1)
		flags.num_lock = true;
	new_event.flags = flags;

	if (pt.x == -1)
	{
		new_event.x = m_xpos;
		new_event.y = m_ypos;
	}
	else
	{
		if (pt.x < 0)
			pt.x = 0;
		if (pt.y < 0)
			pt.y = 0;
		if (pt.x >= m_scr_width)
			pt.x = m_scr_width - 1;
		if (pt.y >= m_scr_height)
			pt.y = m_scr_height - 1;

		new_event.x = (float) pt.x;
		new_event.y = (float) pt.y;
	}

	//new_event->next = 0;

// 	if (!m_input_queue)
// 		m_input_queue = new_event;
// 	else
// 	{
// 		last = m_input_queue;
// 		while (last->next)
// 			last = last->next;
// 		last->next = new_event;
// 	}
	m_input_queue.push_back( new_event );

	if (new_event.type == INPUT_KEYDOWN || new_event.type == INPUT_MBUTTONDOWN)
	{
		m_vkey = new_event.key;
		m_char = new_event.chr;
	}
	else if (new_event.type == INPUT_MOUSEMOVE)
	{
		m_xpos = new_event.x;
		m_ypos = new_event.y;
	}
	else if (new_event.type == INPUT_MOUSEWHEEL)
	{
		m_zpos += new_event.wheel;
	}
}

void HGE_Impl::_ClearQueue()
{
// 	CInputEventList *nexteptr, *eptr = m_input_queue;
// 
// 	memset(&m_key_states, 0, sizeof(m_key_states));
// 
// 	while (eptr)
// 	{
// 		nexteptr = eptr->next;
// 		delete eptr;
// 		eptr = nexteptr;
// 	}
// 
// 	m_input_queue = 0;
	m_input_queue.clear();
	m_vkey = HGEK_NO_KEY;
	m_char = 0;
	m_zpos = 0;
}

} // namespace hge
