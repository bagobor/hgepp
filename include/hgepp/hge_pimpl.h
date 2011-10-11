/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 */
#pragma once

//#include <gapi.h>

#include <windows.h>

#include <memory>

/*
** Common math constants
*/
#ifndef M_PI
#define M_PI    3.14159265358979323846f
#define M_PI_2  1.57079632679489661923f
#define M_PI_4  0.785398163397448309616f
#define M_1_PI  0.318309886183790671538f
#define M_2_PI  0.636619772367581343076f
#endif

namespace hge {


/*
** HGE Interface class
*/
class HGE
{
public:
    virtual void HGE_CALL Release() = 0;

    virtual bool HGE_CALL System_Initiate() = 0;
    virtual void HGE_CALL System_Shutdown() = 0;
    virtual bool HGE_CALL System_Start() = 0;
    virtual char* HGE_CALL System_GetErrorMessage() = 0;
    virtual void HGE_CALL System_Log(const char *format, ...) = 0;
    virtual bool HGE_CALL System_Launch(const char *url) = 0;
    virtual void HGE_CALL System_Snapshot(const char *filename = 0) = 0;

private:
    virtual void HGE_CALL System_SetStateBool(system_state_bool_t state, bool value) = 0;
    virtual void HGE_CALL System_SetStateFunc(system_state_func_t state, void_callback_t value) = 0;
    virtual void HGE_CALL System_SetStateHwnd(system_state_hwnd_t state, HWND value) = 0;
    virtual void HGE_CALL System_SetStateInt(system_state_int_t state, int value) = 0;
    virtual void HGE_CALL System_SetStateString(system_state_string_t state, const char *value) = 0;
    virtual bool HGE_CALL System_GetStateBool(system_state_bool_t state) = 0;
    virtual void_callback_t HGE_CALL System_GetStateFunc( system_state_func_t state) = 0;
    virtual HWND HGE_CALL System_GetStateHwnd(system_state_hwnd_t state) = 0;
    virtual int HGE_CALL System_GetStateInt(system_state_int_t state) = 0;
    virtual const char* HGE_CALL System_GetStateString( system_state_string_t state) = 0;

public:
    inline void System_SetState(system_state_bool_t state, bool value)  {
        System_SetStateBool(state, value);
    }
    inline void System_SetState(system_state_func_t state, void_callback_t value) {
        System_SetStateFunc(state, value);
    }
    inline void System_SetState(system_state_hwnd_t state, HWND value) {
        System_SetStateHwnd(state, value);
    }
    inline void System_SetState(system_state_int_t state, int value) {
        System_SetStateInt(state, value);
    }
    inline void System_SetState(system_state_string_t state, const char *value) {
        System_SetStateString(state, value);
    }
    inline bool System_GetState(system_state_bool_t state) {
        return System_GetStateBool(state);
    }
    inline void_callback_t System_GetState(system_state_func_t state) {
        return System_GetStateFunc(state);
    }
    inline HWND System_GetState(system_state_hwnd_t state) {
        return System_GetStateHwnd(state);
    }
    inline int System_GetState(system_state_int_t state) {
        return System_GetStateInt(state);
    }
    inline const char* System_GetState(system_state_string_t state) {
        return System_GetStateString(state);
    }
    
    //
    // RESOURCE management functions
    //
    virtual bytes_t HGE_CALL Resource_Load(const char *filename, uint32_t *size = 0) = 0;   
    //virtual void HGE_CALL Resource_Free( bytes_t res) = 0;

    virtual bool HGE_CALL Resource_AttachPack(const char *filename, const char *password = 0) = 0;
    virtual void HGE_CALL Resource_RemovePack(const char *filename) = 0;
    virtual void HGE_CALL Resource_RemoveAllPacks() = 0;
    virtual char* HGE_CALL Resource_MakePath(const char *filename = 0) = 0;
    virtual char* HGE_CALL Resource_EnumFiles(const char *wildcard = 0) = 0;
    virtual char* HGE_CALL Resource_EnumFolders(const char *wildcard = 0) = 0;

    virtual void HGE_CALL Ini_SetInt(const char *section, const char *name, int value) = 0;
    virtual int HGE_CALL Ini_GetInt(const char *section, const char *name, int def_val) = 0;
    virtual void HGE_CALL Ini_SetFloat(const char *section, const char *name, float value) = 0;
    virtual float HGE_CALL Ini_GetFloat(const char *section, const char *name, float def_val) = 0;
    virtual void HGE_CALL Ini_SetString(const char *section, const char *name, const char *value) = 0;
    virtual char* HGE_CALL Ini_GetString(const char *section, const char *name, const char *def_val) = 0;

    virtual void HGE_CALL Random_Seed(int seed = 0) = 0;
    virtual int HGE_CALL Random_Int(int min, int max) = 0;
    virtual float HGE_CALL Random_Float(float min, float max) = 0;

    virtual float HGE_CALL Timer_GetTime() = 0;
    virtual float HGE_CALL Timer_GetDelta() = 0;
    virtual int HGE_CALL Timer_GetFPS() = 0;

    virtual HEFFECT HGE_CALL Effect_Load(const char *filename, uint32_t size = 0) = 0;
    virtual void HGE_CALL Effect_Free(HEFFECT eff) = 0;
    virtual HCHANNEL HGE_CALL Effect_Play(HEFFECT eff) = 0;
    virtual HCHANNEL HGE_CALL Effect_PlayEx(HEFFECT eff, int volume = 100, int pan = 0,
            float pitch = 1.0f, bool loop = false) = 0;

    virtual HMUSIC HGE_CALL Music_Load(const char *filename, uint32_t size = 0) = 0;
    virtual void HGE_CALL Music_Free(HMUSIC mus) = 0;
    virtual HCHANNEL HGE_CALL Music_Play(HMUSIC mus, bool loop, int volume = 100, int order = -1, int row = -1) = 0;
    virtual void HGE_CALL Music_SetAmplification(HMUSIC music, int ampl) = 0;
    virtual int HGE_CALL Music_GetAmplification(HMUSIC music) = 0;
    virtual int HGE_CALL Music_GetLength(HMUSIC music) = 0;
    virtual void HGE_CALL Music_SetPos(HMUSIC music, int order, int row) = 0;
    virtual bool HGE_CALL Music_GetPos(HMUSIC music, int *order, int *row) = 0;
    virtual void HGE_CALL Music_SetInstrVolume(HMUSIC music, int instr, int volume) = 0;
    virtual int HGE_CALL Music_GetInstrVolume(HMUSIC music, int instr) = 0;
    virtual void HGE_CALL Music_SetChannelVolume(HMUSIC music, int channel, int volume) = 0;
    virtual int HGE_CALL Music_GetChannelVolume(HMUSIC music, int channel) = 0;

    virtual HSTREAM HGE_CALL Stream_Load(const char *filename, uint32_t size = 0) = 0;
    virtual void HGE_CALL Stream_Free(HSTREAM stream) = 0;
    virtual HCHANNEL HGE_CALL Stream_Play(HSTREAM stream, bool loop, int volume = 100) = 0;

    virtual void HGE_CALL Channel_SetPanning(HCHANNEL chn, int pan) = 0;
    virtual void HGE_CALL Channel_SetVolume(HCHANNEL chn, int volume) = 0;
    virtual void HGE_CALL Channel_SetPitch(HCHANNEL chn, float pitch) = 0;
    virtual void HGE_CALL Channel_Pause(HCHANNEL chn) = 0;
    virtual void HGE_CALL Channel_Resume(HCHANNEL chn) = 0;
    virtual void HGE_CALL Channel_Stop(HCHANNEL chn) = 0;
    virtual void HGE_CALL Channel_PauseAll() = 0;
    virtual void HGE_CALL Channel_ResumeAll() = 0;
    virtual void HGE_CALL Channel_StopAll() = 0;
    virtual bool HGE_CALL Channel_IsPlaying(HCHANNEL chn) = 0;
    virtual float HGE_CALL Channel_GetLength(HCHANNEL chn) = 0;
    virtual float HGE_CALL Channel_GetPos(HCHANNEL chn) = 0;
    virtual void HGE_CALL Channel_SetPos(HCHANNEL chn, float fSeconds) = 0;
    virtual void HGE_CALL Channel_SlideTo(HCHANNEL channel, float time, int volume, int pan = -101,
            float pitch = -1) = 0;
    virtual bool HGE_CALL Channel_IsSliding(HCHANNEL channel) = 0;

    virtual void HGE_CALL Input_GetMousePos(float *x, float *y) = 0;
    virtual void HGE_CALL Input_SetMousePos(float x, float y) = 0;
    virtual int HGE_CALL Input_GetMouseWheel() = 0;
    virtual bool HGE_CALL Input_IsMouseOver() = 0;
    virtual bool HGE_CALL Input_KeyDown(key_code_t key) = 0;
    virtual bool HGE_CALL Input_KeyUp(key_code_t key) = 0;
    virtual bool HGE_CALL Input_GetKeyState(key_code_t key) = 0;
    virtual char* HGE_CALL Input_GetKeyName(key_code_t key) = 0;
    virtual key_code_t HGE_CALL Input_GetKey() = 0;
    virtual int HGE_CALL Input_GetChar() = 0;
    virtual bool HGE_CALL Input_GetEvent(input_event_t *event) = 0;

    virtual bool HGE_CALL Gfx_BeginScene(HTARGET target) = 0;
    inline bool HGE_CALL Gfx_BeginScene()
    {
        return Gfx_BeginScene( HTARGET() );
    }
    virtual void HGE_CALL Gfx_EndScene() = 0;
    virtual void HGE_CALL Gfx_Clear(uint32_t color) = 0;
    virtual void HGE_CALL Gfx_RenderLine(float x1, float y1, float x2, float y2, uint32_t color = hge::COLOR_WHITE,
            float z = 0.5f) = 0;
    virtual void HGE_CALL Gfx_RenderTriple(const hgeTriple *triple) = 0;
    virtual void HGE_CALL Gfx_RenderQuad(const hgeQuad *quad) = 0;
    virtual hgeVertex* HGE_CALL Gfx_StartBatch(primitive_mode_t prim_type, HTEXTURE tex, int blend,
            int *max_prim) = 0;
    virtual void HGE_CALL Gfx_FinishBatch(int nprim) = 0;
    virtual void HGE_CALL Gfx_SetClipping(int x = 0, int y = 0, int w = 0, int h = 0) = 0;
    virtual void HGE_CALL Gfx_SetTransform(float x = 0, float y = 0, float dx = 0, float dy = 0,
            float rot = 0, float hscale = 0, float vscale = 0) = 0;

#if HGE_DIRECTX_VER >= 9
    virtual HSHADER HGE_CALL Shader_Create(const char *filename) = 0;
    virtual void HGE_CALL Shader_Free(HSHADER shader) = 0;
    virtual void HGE_CALL Gfx_SetShader(HSHADER shader) = 0;
#endif

    virtual HTARGET HGE_CALL Target_Create(int width, int height, bool zbuffer) = 0;
    virtual void HGE_CALL Target_Free(HTARGET target) = 0;
    virtual HTEXTURE HGE_CALL Target_GetTexture(HTARGET target) = 0;

    virtual HTEXTURE HGE_CALL Texture_Create(int width, int height) = 0;
    virtual HTEXTURE HGE_CALL Texture_Load(const char *filename, uint32_t size = 0, bool bMipmap = false) = 0;
    virtual void HGE_CALL Texture_Free(HTEXTURE tex) = 0;
    virtual int HGE_CALL Texture_GetWidth(HTEXTURE tex, bool bOriginal = false) = 0;
    virtual int HGE_CALL Texture_GetHeight(HTEXTURE tex, bool bOriginal = false) = 0;
    virtual uint32_t * HGE_CALL Texture_Lock(HTEXTURE tex, bool bReadOnly = true, int left = 0,
            int top = 0, int width = 0, int height = 0) = 0;
    virtual void HGE_CALL Texture_Unlock(HTEXTURE tex) = 0;
};

extern "C" {
    HGE_EXPORT HGE * HGE_CALL hgeCreate(int ver);
}


}
