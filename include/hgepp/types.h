/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 */
#pragma once

#include <stdint.h>
#include <memory>

namespace hge {

/*
 * HGE Handle type
 * handle_type_t is the contained value type (uint32_t or a pointer to resource)
 * flavour_t is some empty/dummy struct type to make handle types different by C++ type
 */
template <typename handle_type_t, class flavour_t>
class HGE_EXPORT handle_t
{
private:
    handle_type_t   m_value;

public:
    typedef handle_t <handle_type_t, flavour_t> this_class_t;

    explicit handle_t(): m_value(handle_type_t())
    {
    }
    explicit handle_t( handle_type_t v ): m_value(v)
    {
    }
//  static handle_t create( handle_type_t x ) {
//      return handle_t( x );
//  }

    inline void reset()
    {
        m_value = handle_type_t();
    }

    inline operator bool() const
    {
        return m_value != handle_type_t();
    }

    inline bool operator == (const this_class_t & other) const
    {
        return m_value == other.m_value;
    }

    inline bool operator != (const this_class_t & other) const
    {
        return m_value != other.m_value;
    }

    inline this_class_t & operator = (const this_class_t & other) 
    {
        m_value = other.m_value;
        return * this;
    }

    inline bool operator < (const this_class_t & other) const
    {
        return m_value < other.m_value;
    }

    inline handle_type_t get() const
    {
        return m_value;
    }
};

#define DECLARE_HGE_HANDLE(Type, FlavName, HandleTypeName) \
    typedef struct { int _dummy; } FlavName##_flavour_t; \
    typedef handle_t <Type, FlavName##_flavour_t> HandleTypeName;

class render_target_t;

DECLARE_HGE_HANDLE( hgeGAPITexture *, texture, HTEXTURE );
DECLARE_HGE_HANDLE( render_target_t *, rendertarget, HTARGET );
DECLARE_HGE_HANDLE( uint32_t, bass_effect, HEFFECT );
DECLARE_HGE_HANDLE( uint32_t, bass_music, HMUSIC );
DECLARE_HGE_HANDLE( uint32_t, bass_stream, HSTREAM );
DECLARE_HGE_HANDLE( uint32_t, bass_channel, HCHANNEL );

#if HGE_DIRECTX_VER >= 9
    DECLARE_HGE_HANDLE( hgeGAPIPixelShader *, shader, HSHADER );
#endif

#undef DECLARE_HGE_HANDLE

//! smart type representing a byte array, auto-deletes when no longer used
typedef std::shared_ptr <char> bytes_t;

/*
** Hardware color macros
*/
inline uint32_t ARGB( uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t(a)<<24) + (uint32_t(r)<<16) + (uint32_t(g)<<8) + uint32_t(b));
}
static const uint32_t COLOR_WHITE = 0xFFFFFFFF;
static const uint32_t COLOR_BLACK = 0xFF000000;

inline uint8_t GETA( uint32_t col ) {
    return ((col)>>24);
}
inline uint8_t GETR( uint32_t col ) {
    return (((col)>>16) & 0xFF);
}
inline uint8_t GETG( uint32_t col ) {
    return (((col)>>8) & 0xFF);
}
inline uint8_t GETB( uint32_t col ) {
    return ((col) & 0xFF);
}
inline uint32_t SETA( uint32_t col, uint8_t a) {
    return (((col) & 0x00FFFFFF) + (uint32_t(a)<<24));
}
inline uint32_t SETR( uint32_t col, uint8_t r) {
    return (((col) & 0xFF00FFFF) + (uint32_t(r)<<16));
}
inline uint32_t SETG( uint32_t col, uint8_t g) {
    return (((col) & 0xFFFF00FF) + (uint32_t(g)<<8));
}
inline uint32_t SETB( uint32_t col, uint8_t b) {
    return (((col) & 0xFFFFFF00) + uint32_t(b));
}

/*
** HGE Blending constants
*/
enum {
    BLEND_COLORADD     = 1,
    BLEND_COLORMUL     = 0,
    BLEND_ALPHABLEND   = 2,
    BLEND_ALPHAADD     = 0,
    BLEND_ZWRITE       = 4,
    BLEND_NOZWRITE     = 0,

    BLEND_DEFAULT      = (BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE),
    BLEND_DEFAULT_Z    = (BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE)
};


/*
** HGE System state constants
*/
typedef enum
{
    HGE_WINDOWED        = 1,    // bool     run in window?      (default: false)
    HGE_ZBUFFER         = 2,    // bool     use z-buffer?       (default: false)
    HGE_TEXTUREFILTER   = 3,    // bool     texture filtering?  (default: true)
    
    HGE_USESOUND        = 4,    // bool     use BASS for sound? (default: true)
    
    HGE_DONTSUSPEND     = 5,    // bool     focus lost:suspend? (default: false)
    HGE_HIDEMOUSE       = 6,    // bool     hide system cursor? (default: true)

    HGE_SHOWSPLASH      = 7,    // bool     hide system cursor? (default: true)

    HGEBOOLSTATE_FORCE_DWORD = 0x7FFFFFFF
} system_state_bool_t;

typedef enum
{
    HGE_FRAMEFUNC       = 8,    // bool*()  frame function      (default: NULL) (you MUST set this)
    HGE_RENDERFUNC      = 9,    // bool*()  render function     (default: NULL)
    HGE_FOCUSLOSTFUNC   = 10,   // bool*()  focus lost function (default: NULL)
    HGE_FOCUSGAINFUNC   = 11,   // bool*()  focus gain function (default: NULL)
    HGE_GFXRESTOREFUNC  = 12,   // bool*()  exit function       (default: NULL)
    HGE_EXITFUNC        = 13,   // bool*()  exit function       (default: NULL)
    
    HGEFUNCSTATE_FORCE_DWORD = 0x7FFFFFFF
} system_state_func_t;

typedef enum 
{
    HGE_HWND            = 15,   // int      window handle: read only
    HGE_HWNDPARENT      = 16,   // int      parent win handle   (default: 0)
    
    HGEHWNDSTATE_FORCE_DWORD = 0x7FFFFFFF
} system_state_hwnd_t;

typedef enum 
{
    HGE_SCREENWIDTH     = 17,   // int      screen width        (default: 800)
    HGE_SCREENHEIGHT    = 18,   // int      screen height       (default: 600)
    HGE_SCREENBPP       = 19,   // int      screen bitdepth     (default: 32) (desktop bpp in windowed mode)
    
    HGE_SAMPLERATE      = 20,   // int      sample rate         (default: 44100)
    HGE_FXVOLUME        = 21,   // int      global fx volume    (default: 100)
    HGE_MUSVOLUME       = 22,   // int      global music volume (default: 100)
    HGE_STREAMVOLUME    = 23,   // int      global music volume (default: 100)
    
    HGE_FPS             = 24,   // int      fixed fps           (default: HGEFPS_UNLIMITED)

    HGE_POWERSTATUS     = 25,   // int      battery life percent + status
    
    HGEINTSTATE_FORCE_DWORD = 0x7FFFFFF
} system_state_int_t;

typedef enum 
{
    HGE_ICON            = 26,   // char*    icon resource       (default: NULL)
    HGE_TITLE           = 27,   // char*    window title        (default: "HGE")
    
    HGE_INIFILE         = 28,   // char*    ini file            (default: NULL) (meaning no file)
    HGE_LOGFILE         = 29,   // char*    log file            (default: NULL) (meaning no file)

    HGESTRINGSTATE_FORCE_DWORD = 0x7FFFFFFF
} system_state_string_t;

/*
** Callback protoype used by HGE
*/
typedef bool (*void_callback_t)();


/*
** HGE_FPS system state special constants
*/
enum {
    HGEFPS_UNLIMITED    = 0,
    HGEFPS_VSYNC        = -1
};


/*
** HGE_POWERSTATUS system state special constants
*/
enum {
    HGEPWR_AC           = -1,
    HGEPWR_UNSUPPORTED  = -2
};


/*
** HGE Primitive type constants
*/
typedef enum {
    HGEPRIM_LINES       = 2,
    HGEPRIM_TRIPLES     = 3,
    HGEPRIM_QUADS       = 4
} primitive_mode_t;


/*
** HGE Vertex structure
*/
struct hgeVertex
{
    float           x, y;       // screen position    
    float           z;          // Z-buffer depth 0..1
    uint32_t        col;        // color
    float           tx, ty;     // texture coordinates
};


/*
** HGE Triple structure
*/
struct hgeTriple
{
    hgeVertex       v[3];
    HTEXTURE        tex;
    uint32_t        blend;
};


/*
** HGE Quad structure
*/
struct hgeQuad
{
    hgeVertex       v[4];
    HTEXTURE        tex;
    uint32_t        blend;
};


/*
** HGE Input Event type constants
*/
typedef enum {
    INPUT_NO_EVENT      = 0,
    INPUT_KEYDOWN       = 1,
    INPUT_KEYUP         = 2,
    INPUT_MBUTTONDOWN   = 3,
    INPUT_MBUTTONUP     = 4,
    INPUT_MOUSEMOVE     = 5,
    INPUT_MOUSEWHEEL    = 6
} event_type_t;


/*
** HGE Input Event flags
*/
class event_flags_t
{
public:
    bool    shift:1;
    bool    ctrl:1;
    bool    alt:1;
    bool    caps_lock:1;
    bool    scroll_lock:1;
    bool    num_lock:1;
    bool    repeat:1;

    event_flags_t(): shift(false), ctrl(false), alt(false), caps_lock(false),
        scroll_lock(false), num_lock(false), repeat(false)
    {
    }
    event_flags_t( bool _r ): shift(false), ctrl(false), alt(false), caps_lock(false)
        , scroll_lock(false), num_lock(false)
        , repeat(_r)
    {
    }
};


/*
** HGE Virtual-key codes
*/
typedef enum {              HGEK_NO_KEY     = 0x00,
    HGEK_LBUTTON   = 0x01,  HGEK_RBUTTON   = 0x02,  HGEK_MBUTTON   = 0x04,
    HGEK_ESCAPE    = 0x1B,  HGEK_BACKSPACE = 0x08,  HGEK_TAB       = 0x09,  
    HGEK_ENTER     = 0x0D,  HGEK_SPACE     = 0x20,  HGEK_SHIFT     = 0x10,
    HGEK_CTRL      = 0x11,  HGEK_ALT       = 0x12,  HGEK_LWIN      = 0x5B,
    HGEK_RWIN      = 0x5C,  HGEK_APPS      = 0x5D,  HGEK_PAUSE     = 0x13,
    HGEK_CAPSLOCK  = 0x14,  HGEK_NUMLOCK   = 0x90,  HGEK_SCROLLLOCK= 0x91,
    HGEK_PGUP      = 0x21,  HGEK_PGDN      = 0x22,  HGEK_HOME      = 0x24,
    HGEK_END       = 0x23,  HGEK_INSERT    = 0x2D,  HGEK_DELETE    = 0x2E,
    HGEK_LEFT      = 0x25,  HGEK_UP        = 0x26,  HGEK_RIGHT     = 0x27,
    HGEK_DOWN      = 0x28,  HGEK_0         = 0x30,  HGEK_1         = 0x31,
    HGEK_2         = 0x32,  HGEK_3         = 0x33,  HGEK_4         = 0x34,
    HGEK_5         = 0x35,  HGEK_6         = 0x36,  HGEK_7         = 0x37,
    HGEK_8         = 0x38,  HGEK_9         = 0x39,  HGEK_A         = 0x41,
    HGEK_B         = 0x42,  HGEK_C         = 0x43,  HGEK_D         = 0x44,
    HGEK_E         = 0x45,  HGEK_F         = 0x46,  HGEK_G         = 0x47,
    HGEK_H         = 0x48,  HGEK_I         = 0x49,  HGEK_J         = 0x4A,
    HGEK_K         = 0x4B,  HGEK_L         = 0x4C,  HGEK_M         = 0x4D,
    HGEK_N         = 0x4E,  HGEK_O         = 0x4F,  HGEK_P         = 0x50,
    HGEK_Q         = 0x51,  HGEK_R         = 0x52,  HGEK_S         = 0x53,
    HGEK_T         = 0x54,  HGEK_U         = 0x55,  HGEK_V         = 0x56,
    HGEK_W         = 0x57,  HGEK_X         = 0x58,  HGEK_Y         = 0x59,
    HGEK_Z         = 0x5A,  HGEK_GRAVE     = 0xC0,  HGEK_MINUS     = 0xBD,
    HGEK_EQUALS    = 0xBB,  HGEK_BACKSLASH = 0xDC,  HGEK_LBRACKET  = 0xDB,
    HGEK_RBRACKET  = 0xDD,  HGEK_SEMICOLON = 0xBA,  HGEK_APOSTROPHE= 0xDE,
    HGEK_COMMA     = 0xBC,  HGEK_PERIOD    = 0xBE,  HGEK_SLASH     = 0xBF,
    HGEK_NUMPAD0   = 0x60,  HGEK_NUMPAD1   = 0x61,  HGEK_NUMPAD2   = 0x62,
    HGEK_NUMPAD3   = 0x63,  HGEK_NUMPAD4   = 0x64,  HGEK_NUMPAD5   = 0x65,
    HGEK_NUMPAD6   = 0x66,  HGEK_NUMPAD7   = 0x67,  HGEK_NUMPAD8   = 0x68,
    HGEK_NUMPAD9   = 0x69,  HGEK_MULTIPLY  = 0x6A,  HGEK_DIVIDE    = 0x6F,
    HGEK_ADD       = 0x6B,  HGEK_SUBTRACT  = 0x6D,  HGEK_DECIMAL   = 0x6E,
    HGEK_F1        = 0x70,  HGEK_F2        = 0x71,  HGEK_F3        = 0x72,
    HGEK_F4        = 0x73,  HGEK_F5        = 0x74,  HGEK_F6        = 0x75,
    HGEK_F7        = 0x76,  HGEK_F8        = 0x77,  HGEK_F9        = 0x78,
    HGEK_F10       = 0x79,  HGEK_F11       = 0x7A,  HGEK_F12       = 0x7B
} key_code_t;

/*
** HGE Input Event structure
*/
class input_event_t
{
public:
    event_type_t    type;   // event type
    key_code_t      key;    // key code
    event_flags_t   flags;  // event flags
    int             chr;    // character code
    int             wheel;  // wheel shift
    float           x;      // mouse cursor x-coordinate
    float           y;      // mouse cursor y-coordinate

    input_event_t(): type(INPUT_NO_EVENT), key(HGEK_NO_KEY), chr(0), wheel(0), x(0.0f), y(0.0f)
    {
    }
};

typedef enum {
    HGEANIM_FWD         = 0,
    HGEANIM_REV         = 1,
    HGEANIM_PINGPONG    = 2,
    HGEANIM_NOPINGPONG  = 0,
    HGEANIM_LOOP        = 4,
    HGEANIM_NOLOOP      = 0
} anim_mode_t;

} // end ns hge
