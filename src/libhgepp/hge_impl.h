/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * Common core implementation header
 */
#pragma once

#include <hge.h>
#include <stdio.h>
#include <hge_gapi.h>

#include <map>
#include <list>
#include <string>

#define HGE_COMPILE_SPLASHSCREEN

namespace hge {

enum {
	D3DFVF_HGEVERTEX = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1),
	VERTEX_BUFFER_SIZE = 4000
};

typedef BOOL (WINAPI *GetSystemPowerStatusFunc)(LPSYSTEM_POWER_STATUS);


class render_target_t
{
public:
	int width;
	int height;
	hgeGAPITexture * pTex;
	hgeGAPISurface * pDepth;
	//render_target_t * next;
};
typedef std::list <render_target_t *> render_target_list_t;

struct texture_cache_item_t
{
	//HTEXTURE tex;
	int width;
	int height;
	//texture_cache_item_t* next;
};
typedef std::map <HTEXTURE, texture_cache_item_t> texture_cache_t;

struct resource_packs_item_t
{
	//char filename[_MAX_PATH];
	//char password[64];
	std::string filename;
	std::string password;
	//resource_packs_item_t* next;

	// TODO: cache open pack info here, create constructor and variables for open pack
	//unzFile m_zip;

	// TODO: add here destructor and cleanup code
};
typedef std::map <std::string, resource_packs_item_t> resource_packs_map_t;

struct CStreamList
{
	HSTREAM hstream;
	bytes_t data;
	CStreamList * next;
};

// struct CInputEventList
// {
// 	hgeInputEvent event;
// 	CInputEventList* next;
// };

void splash_screen_init();
void splash_screen_done();
bool splash_screen_frame();


/*
** HGE Interface implementation
*/
class HGE_Impl : public HGE
{
public:
	virtual void HGE_CALL Release();

	virtual bool HGE_CALL System_Initiate();
	virtual void HGE_CALL System_Shutdown();
	virtual bool HGE_CALL System_Start();
	virtual void HGE_CALL System_SetStateBool(system_state_bool_t state, bool value);
	virtual void HGE_CALL System_SetStateFunc(system_state_func_t state, void_callback_t value);
	virtual void HGE_CALL System_SetStateHwnd(system_state_hwnd_t state, HWND value);
	virtual void HGE_CALL System_SetStateInt(system_state_int_t state, int value);
	virtual void HGE_CALL System_SetStateString(system_state_string_t state, const char *value);
	virtual bool HGE_CALL System_GetStateBool(system_state_bool_t);
	virtual void_callback_t HGE_CALL System_GetStateFunc(system_state_func_t);
	virtual HWND HGE_CALL System_GetStateHwnd(system_state_hwnd_t);
	virtual int HGE_CALL System_GetStateInt(system_state_int_t);
	virtual const char* HGE_CALL System_GetStateString(system_state_string_t);
	virtual char* HGE_CALL System_GetErrorMessage();
	virtual void HGE_CALL System_Log(const char *format, ...);
	virtual bool HGE_CALL System_Launch(const char *url);
	virtual void HGE_CALL System_Snapshot(const char *filename = 0);

	//
	// RESOURCE management functions
	//
	virtual bytes_t HGE_CALL Resource_Load(const char *filename, uint32_t *size = 0);
	//virtual void HGE_CALL Resource_Free(HTEXTURE res);
	virtual bool HGE_CALL Resource_AttachPack(const char *filename, const char *password = 0);
	virtual void HGE_CALL Resource_RemovePack(const char *filename);
	virtual void HGE_CALL Resource_RemoveAllPacks();
	virtual char* HGE_CALL Resource_MakePath(const char *filename = 0);
	virtual char* HGE_CALL Resource_EnumFiles(const char *wildcard = 0);
	virtual char* HGE_CALL Resource_EnumFolders(const char *wildcard = 0);

	virtual void HGE_CALL Ini_SetInt(const char *section, const char *name, int value);
	virtual int HGE_CALL Ini_GetInt(const char *section, const char *name, int def_val);
	virtual void HGE_CALL Ini_SetFloat(const char *section, const char *name, float value);
	virtual float HGE_CALL Ini_GetFloat(const char *section, const char *name, float def_val);
	virtual void HGE_CALL Ini_SetString(const char *section, const char *name, const char *value);
	virtual char * HGE_CALL Ini_GetString(const char *section, const char *name, const char *def_val);

	virtual void HGE_CALL Random_Seed(int seed = 0);
	virtual int HGE_CALL Random_Int(int min, int max);
	virtual float HGE_CALL Random_Float(float min, float max);

	virtual float HGE_CALL Timer_GetTime();
	virtual float HGE_CALL Timer_GetDelta();
	virtual int HGE_CALL Timer_GetFPS();

	virtual HEFFECT HGE_CALL Effect_Load(const char *filename, uint32_t size = 0);
	virtual void HGE_CALL Effect_Free(HEFFECT eff);
	virtual HCHANNEL HGE_CALL Effect_Play(HEFFECT eff);
	virtual HCHANNEL HGE_CALL Effect_PlayEx(HEFFECT eff, int volume = 100, int pan = 0,
			float pitch = 1.0f, bool loop = false);

	virtual HMUSIC HGE_CALL Music_Load(const char *filename, uint32_t size = 0);
	virtual void HGE_CALL Music_Free(HMUSIC mus);
	virtual HCHANNEL HGE_CALL Music_Play(HMUSIC mus, bool loop, int volume = 100, int order = 0,
			int row = 0);
	virtual void HGE_CALL Music_SetAmplification(HMUSIC music, int ampl);
	virtual int HGE_CALL Music_GetAmplification(HMUSIC music);
	virtual int HGE_CALL Music_GetLength(HMUSIC music);
	virtual void HGE_CALL Music_SetPos(HMUSIC music, int order, int row);
	virtual bool HGE_CALL Music_GetPos(HMUSIC music, int *order, int *row);
	virtual void HGE_CALL Music_SetInstrVolume(HMUSIC music, int instr, int volume);
	virtual int HGE_CALL Music_GetInstrVolume(HMUSIC music, int instr);
	virtual void HGE_CALL Music_SetChannelVolume(HMUSIC music, int channel, int volume);
	virtual int HGE_CALL Music_GetChannelVolume(HMUSIC music, int channel);

	virtual HSTREAM HGE_CALL Stream_Load(const char *filename, uint32_t size = 0);
	virtual void HGE_CALL Stream_Free(HSTREAM stream);
	virtual HCHANNEL HGE_CALL Stream_Play(HSTREAM stream, bool loop, int volume = 100);

	virtual void HGE_CALL Channel_SetPanning(HCHANNEL chn, int pan);
	virtual void HGE_CALL Channel_SetVolume(HCHANNEL chn, int volume);
	virtual void HGE_CALL Channel_SetPitch(HCHANNEL chn, float pitch);
	virtual void HGE_CALL Channel_Pause(HCHANNEL chn);
	virtual void HGE_CALL Channel_Resume(HCHANNEL chn);
	virtual void HGE_CALL Channel_Stop(HCHANNEL chn);
	virtual void HGE_CALL Channel_PauseAll();
	virtual void HGE_CALL Channel_ResumeAll();
	virtual void HGE_CALL Channel_StopAll();
	virtual bool HGE_CALL Channel_IsPlaying(HCHANNEL chn);
	virtual float HGE_CALL Channel_GetLength(HCHANNEL chn);
	virtual float HGE_CALL Channel_GetPos(HCHANNEL chn);
	virtual void HGE_CALL Channel_SetPos(HCHANNEL chn, float fSeconds);
	virtual void HGE_CALL Channel_SlideTo(HCHANNEL channel, float time, int volume, int pan = -101,
			float pitch = -1);
	virtual bool HGE_CALL Channel_IsSliding(HCHANNEL channel);

	virtual void HGE_CALL Input_GetMousePos(float *x, float *y);
	virtual void HGE_CALL Input_SetMousePos(float x, float y);
	virtual int HGE_CALL Input_GetMouseWheel();
	virtual bool HGE_CALL Input_IsMouseOver();
	virtual bool HGE_CALL Input_KeyDown(key_code_t key);
	virtual bool HGE_CALL Input_KeyUp(key_code_t key);
	virtual bool HGE_CALL Input_GetKeyState(key_code_t key);
	virtual char* HGE_CALL Input_GetKeyName(key_code_t key);
	virtual key_code_t HGE_CALL Input_GetKey();
	virtual int HGE_CALL Input_GetChar();
	virtual bool HGE_CALL Input_GetEvent(input_event_t *event);

	virtual bool HGE_CALL Gfx_BeginScene(HTARGET target);
	virtual void HGE_CALL Gfx_EndScene();
	virtual void HGE_CALL Gfx_Clear(uint32_t color);
	virtual void HGE_CALL Gfx_RenderLine(float x1, float y1, float x2, float y2, uint32_t color =
			hge::COLOR_WHITE, float z = 0.5f);
	virtual void HGE_CALL Gfx_RenderTriple(const hgeTriple *triple);
	virtual void HGE_CALL Gfx_RenderQuad(const hgeQuad *quad);
	virtual hgeVertex* HGE_CALL Gfx_StartBatch(primitive_mode_t prim_type, HTEXTURE tex, int blend,
			int *max_prim);
	virtual void HGE_CALL Gfx_FinishBatch(int nprim);
	virtual void HGE_CALL Gfx_SetClipping(int x = 0, int y = 0, int w = 0, int h = 0);
	virtual void HGE_CALL Gfx_SetTransform(float x = 0, float y = 0, float dx = 0, float dy = 0,
			float rot = 0, float hscale = 0, float vscale = 0);

#if HGE_DIRECTX_VER >= 9
	virtual HSHADER HGE_CALL Shader_Create(const char *filename);
	virtual void HGE_CALL Shader_Free(HSHADER shader);
	virtual void HGE_CALL Gfx_SetShader(HSHADER shader);
#endif

    virtual HTARGET HGE_CALL Target_Create(int width, int height, bool zbuffer);
	virtual void HGE_CALL Target_Free(HTARGET target);
	virtual HTEXTURE HGE_CALL Target_GetTexture(HTARGET target);

	virtual HTEXTURE HGE_CALL Texture_Create(int width, int height);
	virtual HTEXTURE HGE_CALL Texture_Load(const char *filename, uint32_t size = 0, bool bMipmap =
			false);
	virtual void HGE_CALL Texture_Free(HTEXTURE tex);
	virtual int HGE_CALL Texture_GetWidth(HTEXTURE tex, bool bOriginal = false);
	virtual int HGE_CALL Texture_GetHeight(HTEXTURE tex, bool bOriginal = false);
	virtual uint32_t* HGE_CALL Texture_Lock(HTEXTURE tex, bool bReadOnly = true, int left = 0,
			int top = 0, int width = 0, int height = 0);
	virtual void HGE_CALL Texture_Unlock(HTEXTURE tex);

    //////// Implementation ////////

    static HGE_Impl* _Interface_Get();
	void _FocusChange(bool bAct);
	void _PostError(const char *error);

	HINSTANCE m_hinstance;
	HWND m_hwnd;

	bool m_active;
	// TODO: string this
	char m_error[256];
	// TODO: string this
	char m_app_path[_MAX_PATH];
	// TODO: string this
	char m_ini_string[256];

	// System States
	bool (*m_frame_func)();
	bool (*m_render_func)();
	bool (*m_focuslost_func)();
	bool (*m_focusgain_func)();
	bool (*m_gfxrestore_func)();
	bool (*m_exit_func)();
	const char* m_icon;
	char m_window_title[256];
	int m_scr_width;
	int m_scr_height;
	int m_color_depth;
	bool m_windowed;
	bool m_zbuffer;
	bool m_tex_filter;
	char m_ini_file[_MAX_PATH];
	char m_log_file[_MAX_PATH];
	bool m_use_sound;
	int m_sample_rate;
	int m_fx_vol;
	int m_mus_vol;
	int m_stream_vol;
	int m_fps_limit;
	bool m_hide_mouse;
	bool m_dont_suspend;
	HWND m_parent_hwnd;

#ifdef HGE_COMPILE_SPLASHSCREEN
	bool m_show_demo;
#endif

	// Power
	int m_power_status;
	HMODULE m_kernel32;
	GetSystemPowerStatusFunc m_getsystempowerstatus_func;

	void _InitPowerStatus();
	void _UpdatePowerStatus();
	void _DonePowerStatus();

	// Graphics
	D3DPRESENT_PARAMETERS* m_d3dpp;

	D3DPRESENT_PARAMETERS m_d3dppW;
	RECT m_rectW;
	LONG m_styleW;

	D3DPRESENT_PARAMETERS m_d3dppFS;
	RECT m_rectFS;
	LONG m_styleFS;

	hgeGAPI * m_d3d;
	hgeGAPIDevice * m_d3d_device;
	hgeGAPIVertexBuffer * m_vertex_buf;
	hgeGAPIIndexBuffer * m_index_buf;

	hgeGAPISurface * m_screen_sfc;
	hgeGAPISurface * m_depth_sfc;

	render_target_list_t m_targets_list;
	render_target_t * m_cur_target;

	D3DXMATRIX m_view_matrix;
	D3DXMATRIX m_proj_matrix;

	// TODO: replace with STL container
	texture_cache_t m_textures_map;
	hgeVertex* m_vertices;

	int m_prim_count;
	primitive_mode_t m_cur_prim_type;
	int m_cur_blend_mode;
	HTEXTURE m_cur_texture;

#if HGE_DIRECTX_VER >= 9
	HSHADER m_cur_shader;
#endif

	bool _GfxInit();
	void _GfxDone();
	bool _GfxRestore();
	void _AdjustWindow();
	void _Resize(int width, int height);
	bool _init_lost();
	void _render_batch(bool bEndScene = false);
	int _format_id(D3DFORMAT fmt);
	void _SetBlendMode(int blend);
	void _SetProjectionMatrix(int width, int height);

	// Audio
	HINSTANCE m_bassdll;
	bool m_no_sound;
	// TODO: replace with STL container
	CStreamList* m_streams_list;

	bool _SoundInit();
	void _SoundDone();
	void _SetMusVolume(int vol);
	void _SetStreamVolume(int vol);
	void _SetFXVolume(int vol);

	// Input
	key_code_t m_vkey;
	int m_char;
	int m_zpos;
	float m_xpos;
	float m_ypos;
	bool m_mouse_over;
	bool m_captured;
	char m_key_states[256];
	//CInputEventList* m_input_queue;
	std::list <input_event_t> m_input_queue;

	void _UpdateMouse();
	void _InputInit();
	void _ClearQueue();
	void _BuildEvent(event_type_t type, key_code_t key, int scan, event_flags_t flags, int x, int y);

	// Resources
	char m_tmp_filename[_MAX_PATH];
	//resource_cache_item_t * m_res_list;
	resource_packs_map_t m_resources;
	HANDLE m_hsearch;
	WIN32_FIND_DATA m_search_data;

	// Timer
	float m_time;
	float m_time_delta;
	uint32_t m_time_fixeddelta;
	int m_time_fps;
	uint32_t m_time_t0;
	uint32_t m_time_t0fps;
	uint32_t m_time_dt;
	int m_time_cfps;

private:
	HGE_Impl();
};

extern HGE_Impl * g_hge_singleton;

} // namespace hge
