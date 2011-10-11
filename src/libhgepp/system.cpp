/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * Core system functions
 */
#include "hge_impl.h"

namespace hge
{

#define LOWORDINT(n) ((int)((signed short)(LOWORD(n))))
#define HIWORDINT(n) ((int)((signed short)(HIWORD(n))))

const char *WINDOW_CLASS_NAME = "HGE__WNDCLASS";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int g_hge_refcount = 0;
HGE_Impl * g_hge_singleton = nullptr;

BOOL APIENTRY DllMain(HANDLE, uint32_t, LPVOID)
{
	return TRUE;
}

HGE* HGE_CALL hgeCreate(int ver)
{
	if (ver == HGE_VERSION)
		return (HGE*) HGE_Impl::_Interface_Get();
	else
		return nullptr;
}

HGE_Impl* HGE_Impl::_Interface_Get()
{
	if (!g_hge_singleton)
		g_hge_singleton = new HGE_Impl();
	g_hge_refcount++;
	return g_hge_singleton;
}

void HGE_CALL HGE_Impl::Release()
{
	g_hge_refcount--;

	if (!g_hge_refcount)
	{
		if (g_hge_singleton->m_hwnd)
			g_hge_singleton->System_Shutdown();
		Resource_RemoveAllPacks();
		delete g_hge_singleton;
		g_hge_singleton = nullptr;
	}
}

bool HGE_CALL HGE_Impl::System_Initiate()
{
	OSVERSIONINFO os_ver;
	SYSTEMTIME tm;
	MEMORYSTATUS mem_st;
	WNDCLASS winclass;
	int width, height;

	// Log system info

	System_Log("HGE Started..\n");

	System_Log("HGE version: %X.%X", HGE_VERSION >> 8, HGE_VERSION & 0xFF);
	GetLocalTime(&tm);
	System_Log("Date: %02d.%02d.%d, %02d:%02d:%02d\n", tm.wDay, tm.wMonth, tm.wYear, tm.wHour,
			tm.wMinute, tm.wSecond);

	System_Log("Application: %s", m_window_title);
	os_ver.dwOSVersionInfoSize = sizeof(os_ver);
	GetVersionEx(&os_ver);
	System_Log("OS: Windows %ld.%ld.%ld", os_ver.dwMajorVersion, os_ver.dwMinorVersion,
			os_ver.dwBuildNumber);

	GlobalMemoryStatus(&mem_st);
	System_Log("Memory: %ldK total, %ldK free\n", mem_st.dwTotalPhys / 1024L, mem_st.dwAvailPhys
			/ 1024L);

	// Register window class

	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = m_hinstance;
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	if (m_icon)
		winclass.hIcon = LoadIcon(m_hinstance, m_icon);
	else
		winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClass(&winclass))
	{
		_PostError("Can't register window class");
		return false;
	}

	// Create window

	width = m_scr_width + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	height = m_scr_height + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

	m_rectW.left = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	m_rectW.top = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	m_rectW.right = m_rectW.left + width;
	m_rectW.bottom = m_rectW.top + height;
	m_styleW = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE; //WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;

	m_rectFS.left = 0;
	m_rectFS.top = 0;
	m_rectFS.right = m_scr_width;
	m_rectFS.bottom = m_scr_height;
	m_styleFS = WS_POPUP | WS_VISIBLE; //WS_POPUP

	if (m_parent_hwnd)
	{
		m_rectW.left = 0;
		m_rectW.top = 0;
		m_rectW.right = m_scr_width;
		m_rectW.bottom = m_scr_height;
		m_styleW = WS_CHILD | WS_VISIBLE;
		m_windowed = true;
	}

	if (m_windowed)
		m_hwnd = CreateWindowEx(0, WINDOW_CLASS_NAME, m_window_title, m_styleW, m_rectW.left,
				m_rectW.top, m_rectW.right - m_rectW.left, m_rectW.bottom - m_rectW.top,
				m_parent_hwnd, NULL, m_hinstance, NULL);
	else
		m_hwnd = CreateWindowEx(WS_EX_TOPMOST, WINDOW_CLASS_NAME, m_window_title, m_styleFS, 0, 0,
				0, 0, NULL, NULL, m_hinstance, NULL);
	if (!m_hwnd)
	{
		_PostError("Can't create window");
		return false;
	}

	ShowWindow(m_hwnd, SW_SHOW);

	// Init subsystems

	timeBeginPeriod(1);
	Random_Seed();
	_InitPowerStatus();
	_InputInit();
	if (!_GfxInit())
	{
		System_Shutdown();
		return false;
	}
	if (!_SoundInit())
	{
		System_Shutdown();
		return false;
	}

	System_Log("Init done.\n");

	m_time = 0.0f;
	m_time_t0 = m_time_t0fps = timeGetTime();
	m_time_dt = m_time_cfps = 0;
	m_time_fps = 0;

	// Show splash

#ifdef HGE_COMPILE_SPLASHSCREEN

	bool (*func)();
	bool (*rfunc)();
	HWND hwndTmp;

	if (g_hge_singleton->m_show_demo)
	{
		Sleep(200);
		func = (bool(*)()) g_hge_singleton->System_GetStateFunc(HGE_FRAMEFUNC);
		rfunc = (bool(*)()) g_hge_singleton->System_GetStateFunc(HGE_RENDERFUNC);
		hwndTmp = m_parent_hwnd;
		m_parent_hwnd = 0;
		g_hge_singleton->System_SetStateFunc(HGE_FRAMEFUNC, splash_screen_frame);
		g_hge_singleton->System_SetStateFunc(HGE_RENDERFUNC, 0);
		splash_screen_init();
		g_hge_singleton->System_Start();
		splash_screen_done();
		m_parent_hwnd = hwndTmp;
		g_hge_singleton->System_SetStateFunc(HGE_FRAMEFUNC, func);
		g_hge_singleton->System_SetStateFunc(HGE_RENDERFUNC, rfunc);
	}

#endif

	// Done

	return true;
}

void HGE_CALL HGE_Impl::System_Shutdown()
{
	System_Log("\nFinishing..");

	timeEndPeriod(1);
	if (m_hsearch)
	{
		FindClose(m_hsearch);
		m_hsearch = 0;
	}
	_ClearQueue();
	_SoundDone();
	_GfxDone();
	_DonePowerStatus();

	if (m_hwnd)
	{
		//ShowWindow(hwnd, SW_HIDE);
		//SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
		//ShowWindow(hwnd, SW_SHOW);
		DestroyWindow(m_hwnd);
		m_hwnd = nullptr;
	}

	if (m_hinstance)
		UnregisterClass(WINDOW_CLASS_NAME, m_hinstance);

	System_Log("The End.");
}

bool HGE_CALL HGE_Impl::System_Start()
{
	MSG msg;

	if (!m_hwnd)
	{
		_PostError("System_Start: System_Initiate wasn't called");
		return false;
	}

	if (!m_frame_func)
	{
		_PostError("System_Start: No frame function defined");
		return false;
	}

	m_active = true;

	// MAIN LOOP

	for (;;)
	{

		// Process window messages if not in "child mode"
		// (if in "child mode" the parent application will do this for us)

		if (!m_parent_hwnd)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;
				// TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
			}
		}

		// Check if mouse is over HGE window for Input_IsMouseOver

		_UpdateMouse();

		// If HGE window is focused or we have the "don't suspend" state - process the main loop

		if (m_active || m_dont_suspend)
		{
			// Ensure we have at least 1ms time step
			// to not confuse user's code with 0

			do
			{
				m_time_dt = timeGetTime() - m_time_t0;
			} while (m_time_dt < 1);

			// If we reached the time for the next frame
			// or we just run in unlimited FPS mode, then
			// do the stuff

			if (m_time_dt >= m_time_fixeddelta)
			{
				// fDeltaTime = time step in seconds returned by Timer_GetDelta

				m_time_delta = m_time_dt / 1000.0f;

				// Cap too large time steps usually caused by lost focus to avoid jerks

				if (m_time_delta > 0.2f)
				{
					m_time_delta = m_time_fixeddelta ? m_time_fixeddelta / 1000.0f : 0.01f;
				}

				// Update time counter returned Timer_GetTime

				m_time += m_time_delta;

				// Store current time for the next frame
				// and count FPS

				m_time_t0 = timeGetTime();
				if (m_time_t0 - m_time_t0fps <= 1000)
					m_time_cfps++;
				else
				{
					m_time_fps = m_time_cfps;
					m_time_cfps = 0;
					m_time_t0fps = m_time_t0;
					_UpdatePowerStatus();
				}

				// Do user's stuff

				if (m_frame_func())
					break;
				if (m_render_func)
					m_render_func();

				// If if "child mode" - return after processing single frame

				if (m_parent_hwnd)
					break;

				// Clean up input events that were generated by
				// WindowProc and weren't handled by user's code

				_ClearQueue();

				// If we use VSYNC - we could afford a little
				// sleep to lower CPU usage

				// if(!bWindowed && nHGEFPS==HGEFPS_VSYNC) Sleep(1);
			}

			// If we have a fixed frame rate and the time
			// for the next frame isn't too close, sleep a bit

			else
			{
				if (m_time_fixeddelta && m_time_dt + 3 < m_time_fixeddelta)
					Sleep(1);
			}
		}

		// If main loop is suspended - just sleep a bit
		// (though not too much to allow instant window
		// redraw if requested by OS)

		else
			Sleep(1);
	}

	_ClearQueue();

	m_active = false;

	return true;
}

void HGE_CALL HGE_Impl::System_SetStateBool(system_state_bool_t state, bool value)
{
	switch (state)
	{
	case HGE_WINDOWED:
		if (m_vertices || m_parent_hwnd)
			break;
		if (m_d3d_device && m_windowed != value)
		{
			if (m_d3dppW.BackBufferFormat == D3DFMT_UNKNOWN || m_d3dppFS.BackBufferFormat
					== D3DFMT_UNKNOWN)
				break;

			if (m_windowed)
				GetWindowRect(m_hwnd, &m_rectW);
			m_windowed = value;
			if (m_windowed)
				m_d3dpp = &m_d3dppW;
			else
				m_d3dpp = &m_d3dppFS;

			if (_format_id(m_d3dpp->BackBufferFormat) < 4)
				m_color_depth = 16;
			else
				m_color_depth = 32;

			_GfxRestore();
			_AdjustWindow();
		}
		else
			m_windowed = value;
		break;

	case HGE_ZBUFFER:
		if (!m_d3d_device)
			m_zbuffer = value;
		break;

	case HGE_TEXTUREFILTER:
		m_tex_filter = value;
		if (m_d3d_device)
		{
			_render_batch();
			if (m_tex_filter)
			{
#if HGE_DIRECTX_VER == 8
				m_d3d_device->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
				m_d3d_device->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_LINEAR);
#endif
#if HGE_DIRECTX_VER == 9
				m_d3d_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
				m_d3d_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
#endif
			}
			else
			{
#if HGE_DIRECTX_VER == 8
				m_d3d_device->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_POINT);
				m_d3d_device->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_POINT);
#endif
#if HGE_DIRECTX_VER == 9
				m_d3d_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
				m_d3d_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
#endif
			}
		}
		break;

	case HGE_USESOUND:
		if (m_use_sound != value)
		{
			m_use_sound = value;
			if (m_use_sound && m_hwnd)
				_SoundInit();
			if (!m_use_sound && m_hwnd)
				_SoundDone();
		}
		break;

	case HGE_HIDEMOUSE:
		m_hide_mouse = value;
		break;

	case HGE_DONTSUSPEND:
		m_dont_suspend = value;
		break;

#ifdef HGE_COMPILE_SPLASHSCREEN
	case HGE_SHOWSPLASH:
		m_show_demo = value;
		break;
#endif
	}
}

void HGE_CALL HGE_Impl::System_SetStateFunc(system_state_func_t state, void_callback_t value)
{
	switch (state)
	{
	case HGE_FRAMEFUNC:
		m_frame_func = value;
		break;
	case HGE_RENDERFUNC:
		m_render_func = value;
		break;
	case HGE_FOCUSLOSTFUNC:
		m_focuslost_func = value;
		break;
	case HGE_FOCUSGAINFUNC:
		m_focusgain_func = value;
		break;
	case HGE_GFXRESTOREFUNC:
		m_gfxrestore_func = value;
		break;
	case HGE_EXITFUNC:
		m_exit_func = value;
		break;
	}
}

void HGE_CALL HGE_Impl::System_SetStateHwnd(system_state_hwnd_t state, HWND value)
{
	switch (state)
	{
	case HGE_HWNDPARENT:
		if (!m_hwnd)
			m_parent_hwnd = value;
		break;
	}
}

void HGE_CALL HGE_Impl::System_SetStateInt(system_state_int_t state, int value)
{
	switch (state)
	{
	case HGE_SCREENWIDTH:
		if (!m_d3d_device)
			m_scr_width = value;
		break;

	case HGE_SCREENHEIGHT:
		if (!m_d3d_device)
			m_scr_height = value;
		break;

	case HGE_SCREENBPP:
		if (!m_d3d_device)
			m_color_depth = value;
		break;

	case HGE_SAMPLERATE:
		if (!m_bassdll)
			m_sample_rate = value;
		break;

	case HGE_FXVOLUME:
		m_fx_vol = value;
		_SetFXVolume(m_fx_vol);
		break;

	case HGE_MUSVOLUME:
		m_mus_vol = value;
		_SetMusVolume(m_mus_vol);
		break;

	case HGE_STREAMVOLUME:
		m_stream_vol = value;
		_SetStreamVolume(m_stream_vol);
		break;

	case HGE_FPS:
		if (m_vertices)
			break;

		if (m_d3d_device)
		{
			if ((m_fps_limit >= 0 && value < 0) || (m_fps_limit < 0 && value >= 0))
			{
				if (value == HGEFPS_VSYNC)
				{
#if HGE_DIRECTX_VER == 8
					m_d3dppW.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
					m_d3dppFS.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
#endif
#if HGE_DIRECTX_VER == 9
					m_d3dppW.SwapEffect = D3DSWAPEFFECT_COPY;
					m_d3dppW.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
					m_d3dppFS.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
#endif
				}
				else
				{
					m_d3dppW.SwapEffect = D3DSWAPEFFECT_COPY;
#if HGE_DIRECTX_VER == 8
					m_d3dppFS.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#endif
#if HGE_DIRECTX_VER == 9
					m_d3dppFS.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#endif
				}
				//if(procFocusLostFunc) procFocusLostFunc();
				_GfxRestore();
				//if(procFocusGainFunc) procFocusGainFunc();
			}
		}
		m_fps_limit = value;
		if (m_fps_limit > 0)
			m_time_fixeddelta = int(1000.0f / value);
		else
			m_time_fixeddelta = 0;
		break;
	}
}

void HGE_CALL HGE_Impl::System_SetStateString(system_state_string_t state, const char *value)
{
	FILE *hf;

	switch (state)
	{
	case HGE_ICON:
		m_icon = value;
		if (g_hge_singleton->m_hwnd)
			SetClassLong(g_hge_singleton->m_hwnd, GCL_HICON, (LONG) LoadIcon(
					g_hge_singleton->m_hinstance, m_icon));
		break;
	case HGE_TITLE:
		strcpy(m_window_title, value);
		if (g_hge_singleton->m_hwnd)
			SetWindowText(g_hge_singleton->m_hwnd, m_window_title);
		break;
	case HGE_INIFILE:
		if (value)
			strcpy(m_ini_file, Resource_MakePath(value));
		else
			m_ini_file[0] = 0;
		break;
	case HGE_LOGFILE:
		if (value)
		{
			strcpy(m_log_file, Resource_MakePath(value));
			hf = fopen(m_log_file, "w");
			if (!hf)
				m_log_file[0] = 0;
			else
				fclose(hf);
		}
		else
			m_log_file[0] = 0;
		break;
	}
}

bool HGE_CALL HGE_Impl::System_GetStateBool(system_state_bool_t state)
{
	switch (state)
	{
	case HGE_WINDOWED:
		return m_windowed;
	case HGE_ZBUFFER:
		return m_zbuffer;
	case HGE_TEXTUREFILTER:
		return m_tex_filter;
	case HGE_USESOUND:
		return m_use_sound;
	case HGE_DONTSUSPEND:
		return m_dont_suspend;
	case HGE_HIDEMOUSE:
		return m_hide_mouse;

#ifdef HGE_COMPILE_SPLASHSCREEN
	case HGE_SHOWSPLASH:
		return m_show_demo;
#endif
	}

	return false;
}

void_callback_t HGE_CALL HGE_Impl::System_GetStateFunc(system_state_func_t state)
{
	switch (state)
	{
	case HGE_FRAMEFUNC:
		return m_frame_func;
	case HGE_RENDERFUNC:
		return m_render_func;
	case HGE_FOCUSLOSTFUNC:
		return m_focuslost_func;
	case HGE_FOCUSGAINFUNC:
		return m_focusgain_func;
	case HGE_EXITFUNC:
		return m_exit_func;
	}

	return NULL;
}

HWND HGE_CALL HGE_Impl::System_GetStateHwnd(system_state_hwnd_t state)
{
	switch (state)
	{
	case HGE_HWND:
		return m_hwnd;
	case HGE_HWNDPARENT:
		return m_parent_hwnd;
	}

	return 0;
}

int HGE_CALL HGE_Impl::System_GetStateInt(system_state_int_t state)
{
	switch (state)
	{
	case HGE_SCREENWIDTH:
		return m_scr_width;
	case HGE_SCREENHEIGHT:
		return m_scr_height;
	case HGE_SCREENBPP:
		return m_color_depth;
	case HGE_SAMPLERATE:
		return m_sample_rate;
	case HGE_FXVOLUME:
		return m_fx_vol;
	case HGE_MUSVOLUME:
		return m_mus_vol;
	case HGE_STREAMVOLUME:
		return m_stream_vol;
	case HGE_FPS:
		return m_fps_limit;
	case HGE_POWERSTATUS:
		return m_power_status;
	}

	return 0;
}

const char* HGE_CALL HGE_Impl::System_GetStateString(system_state_string_t state)
{
	switch (state)
	{
	case HGE_ICON:
		return m_icon;
	case HGE_TITLE:
		return m_window_title;
	case HGE_INIFILE:
		if (m_ini_file[0])
			return m_ini_file;
		else
			return 0;
	case HGE_LOGFILE:
		if (m_log_file[0])
			return m_log_file;
		else
			return 0;
	}

	return NULL;
}

char* HGE_CALL HGE_Impl::System_GetErrorMessage()
{
	return m_error;
}

void HGE_CALL HGE_Impl::System_Log(const char *szFormat, ...)
{
	FILE *hf = NULL;
	va_list ap;

	if (!m_log_file[0])
		return;

	hf = fopen(m_log_file, "a");
	if (!hf)
		return;

	va_start(ap, szFormat);
	vfprintf(hf, szFormat, ap);
	va_end(ap);

	fprintf(hf, "\n");

	fclose(hf);
}

bool HGE_CALL HGE_Impl::System_Launch(const char *url)
{
	if ((uint32_t) ShellExecute(g_hge_singleton->m_hwnd, NULL, url, NULL, NULL, SW_SHOWMAXIMIZED)
			> 32)
		return true;
	else
		return false;
}

void HGE_CALL HGE_Impl::System_Snapshot(const char *filename)
{
	hgeGAPISurface * pSurf;
	char *shotname, tempname[_MAX_PATH];
	int i;

	if (!filename)
	{
		i = 0;
		shotname = Resource_EnumFiles("shot???.bmp");
		while (shotname)
		{
			i++;
			shotname = Resource_EnumFiles();
		}
		sprintf(tempname, "shot%03d.bmp", i);
		filename = Resource_MakePath(tempname);
	}

	if (m_d3d_device)
	{
#if HGE_DIRECTX_VER == 8
		m_d3d_device->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pSurf);
#endif
#if HGE_DIRECTX_VER == 9
		m_d3d_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurf);
#endif
		D3DXSaveSurfaceToFile(filename, D3DXIFF_BMP, pSurf, NULL, NULL);
		pSurf->Release();
	}
}

//////// Implementation ////////


HGE_Impl::HGE_Impl()
{
	m_hinstance = GetModuleHandle(0);
	m_hwnd = nullptr;
	m_active = false;
	m_error[0] = 0;

	m_d3d = nullptr;
	m_d3d_device = nullptr;
	m_d3dpp = nullptr;
	//m_targets_list = nullptr;
	m_cur_target = nullptr;
	m_screen_sfc = nullptr;
	m_depth_sfc = nullptr;
	m_vertex_buf = nullptr;
	m_index_buf = nullptr;
	m_vertices = nullptr;
	//m_textures_map = nullptr;

	m_bassdll = nullptr;
	m_no_sound = false;
	m_streams_list = nullptr;

	m_hsearch = nullptr;
	//m_resources = nullptr;

	//m_input_queue = nullptr;
	m_char = m_zpos = 0;
	m_vkey = HGEK_NO_KEY;
	m_xpos = m_ypos = 0.0f;
	m_mouse_over = false;
	m_captured = false;

	m_fps_limit = HGEFPS_UNLIMITED;
	m_time = 0.0f;
	m_time_delta = 0.0f;
	m_time_fps = 0;

	m_frame_func = nullptr;
	m_render_func = nullptr;
	m_focuslost_func = nullptr;
	m_focusgain_func = nullptr;
	m_gfxrestore_func = nullptr;
	m_exit_func = nullptr;
	m_icon = nullptr;
	strcpy(m_window_title, "HGE");
	m_scr_width = 800;
	m_scr_height = 600;
	m_color_depth = 32;
	m_windowed = false;
	m_zbuffer = false;
	m_tex_filter = true;
	m_log_file[0] = 0;
	m_ini_file[0] = 0;
	m_use_sound = true;
	m_sample_rate = 44100;
	m_fx_vol = 100;
	m_mus_vol = 100;
	m_stream_vol = 100;
	m_time_fixeddelta = 0;
	m_hide_mouse = true;
	m_dont_suspend = false;
	m_parent_hwnd = 0;

	m_power_status = HGEPWR_UNSUPPORTED;
	m_kernel32 = NULL;
	m_getsystempowerstatus_func = NULL;

#ifdef HGE_COMPILE_SPLASHSCREEN
	m_show_demo = true;
#endif

	GetModuleFileName(GetModuleHandle(NULL), m_app_path, sizeof(m_app_path));
	int i;
	for (i = strlen(m_app_path) - 1; i > 0; i--)
		if (m_app_path[i] == '\\')
			break;
	m_app_path[i + 1] = 0;
}

void HGE_Impl::_PostError(const char *error)
{
	System_Log(error);
	strcpy(m_error, error);
}

void HGE_Impl::_FocusChange(bool bAct)
{
	m_active = bAct;

	if (m_active)
	{
		if (m_focusgain_func)
			m_focusgain_func();
	}
	else
	{
		if (m_focuslost_func)
			m_focuslost_func();
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	bool bActivating;

	switch (msg)
	{
	case WM_CREATE:
		return false;

	case WM_PAINT:
		if (g_hge_singleton->m_d3d && g_hge_singleton->m_render_func && g_hge_singleton->m_windowed)
			g_hge_singleton->m_render_func();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return false;

		/*
		 case WM_ACTIVATEAPP:
		 bActivating = (wparam == TRUE);
		 if(pHGE->pD3D && pHGE->bActive != bActivating) pHGE->_FocusChange(bActivating);
		 return FALSE;
		 */
	case WM_ACTIVATE:
		// tricky: we should catch WA_ACTIVE and WA_CLICKACTIVE,
		// but only if HIWORD(wParam) (fMinimized) == FALSE (0)
		bActivating = (LOWORD(wparam) != WA_INACTIVE) && (HIWORD(wparam) == 0);
		if (g_hge_singleton->m_d3d && g_hge_singleton->m_active != bActivating)
			g_hge_singleton->_FocusChange(bActivating);
		return false;

	case WM_SETCURSOR:
		if (g_hge_singleton->m_active && LOWORD(lparam) == HTCLIENT
				&& g_hge_singleton->m_hide_mouse)
			SetCursor(NULL);
		else
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		return false;

	case WM_SYSKEYDOWN:
		if (wparam == VK_F4)
		{
			if (g_hge_singleton->m_exit_func && !g_hge_singleton->m_exit_func())
				return false;
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		else if (wparam == VK_RETURN)
		{
			g_hge_singleton->System_SetState(HGE_WINDOWED, !g_hge_singleton->System_GetState(
					HGE_WINDOWED));
			return false;
		}
		else
		{
			g_hge_singleton->_BuildEvent(INPUT_KEYDOWN, (key_code_t)wparam,
					HIWORD(lparam) & 0xFF, (lparam & 0x40000000) ? event_flags_t(true) : event_flags_t(), -1, -1);
			return false;
		}

	case WM_KEYDOWN:
		g_hge_singleton->_BuildEvent(INPUT_KEYDOWN, (key_code_t)wparam, HIWORD(lparam) & 0xFF,
				(lparam & 0x40000000) ? event_flags_t(true) : event_flags_t(), -1, -1);
		return false;

	case WM_SYSKEYUP:
		g_hge_singleton->_BuildEvent(INPUT_KEYUP, (key_code_t)wparam, HIWORD(lparam) & 0xFF, event_flags_t(),
				-1, -1);
		return false;

	case WM_KEYUP:
		g_hge_singleton->_BuildEvent(INPUT_KEYUP, (key_code_t)wparam, HIWORD(lparam) & 0xFF, event_flags_t(),
				-1, -1);
		return false;

	case WM_LBUTTONDOWN:
		SetFocus(hwnd);
		g_hge_singleton->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_LBUTTON, 0, 0, LOWORDINT(lparam),
				HIWORDINT(lparam));
		return false;
	case WM_MBUTTONDOWN:
		SetFocus(hwnd);
		g_hge_singleton->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_MBUTTON, 0, 0, LOWORDINT(lparam),
				HIWORDINT(lparam));
		return false;
	case WM_RBUTTONDOWN:
		SetFocus(hwnd);
		g_hge_singleton->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_RBUTTON, 0, 0, LOWORDINT(lparam),
				HIWORDINT(lparam));
		return false;

	case WM_LBUTTONDBLCLK:
		g_hge_singleton->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_LBUTTON, 0, event_flags_t(true),
				LOWORDINT(lparam), HIWORDINT(lparam));
		return false;
	case WM_MBUTTONDBLCLK:
		g_hge_singleton->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_MBUTTON, 0, event_flags_t(true),
				LOWORDINT(lparam), HIWORDINT(lparam));
		return false;
	case WM_RBUTTONDBLCLK:
		g_hge_singleton->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_RBUTTON, 0, event_flags_t(true),
				LOWORDINT(lparam), HIWORDINT(lparam));
		return false;

	case WM_LBUTTONUP:
		g_hge_singleton->_BuildEvent(INPUT_MBUTTONUP, HGEK_LBUTTON, 0, 0, LOWORDINT(lparam),
				HIWORDINT(lparam));
		return false;
	case WM_MBUTTONUP:
		g_hge_singleton->_BuildEvent(INPUT_MBUTTONUP, HGEK_MBUTTON, 0, 0, LOWORDINT(lparam),
				HIWORDINT(lparam));
		return false;
	case WM_RBUTTONUP:
		g_hge_singleton->_BuildEvent(INPUT_MBUTTONUP, HGEK_RBUTTON, 0, 0, LOWORDINT(lparam),
				HIWORDINT(lparam));
		return false;

	case WM_MOUSEMOVE:
		g_hge_singleton->_BuildEvent(INPUT_MOUSEMOVE, HGEK_NO_KEY, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
		return false;
	case 0x020A: // WM_MOUSEWHEEL, GET_WHEEL_DELTA_WPARAM(wparam);
		g_hge_singleton->_BuildEvent(INPUT_MOUSEWHEEL, (key_code_t)(short(HIWORD(wparam)) / 120),
			0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
		return false;

	case WM_SIZE:
		if (g_hge_singleton->m_d3d && wparam == SIZE_RESTORED)
			g_hge_singleton->_Resize(LOWORD(lparam), HIWORD(lparam));
		//return FALSE;
		break;

	case WM_SYSCOMMAND:
		if (wparam == SC_CLOSE)
		{
			if (g_hge_singleton->m_exit_func && !g_hge_singleton->m_exit_func())
				return false;
			g_hge_singleton->m_active = false;
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

} // namespace hge
