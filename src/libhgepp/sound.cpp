/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * Core functions implementation: audio routines
 */
#include <hgepp/hge_impl.h>

namespace hge
{

#define BASSDEF(f) (WINAPI *f)	// define the functions as pointers
#include "BASS\bass.h"

#define LOADBASSFUNCTION(f) *((void**)&f)=(void*)GetProcAddress(m_bassdll,#f)

HEFFECT HGE_CALL HGE_Impl::Effect_Load(const char *filename, uint32_t size)
{
	uint32_t _size, length, samples;
	bass_HSAMPLE hs;
	HSTREAM hstrm;
	BASS_CHANNELINFO info;
	void * buffer;
	void * data;
	bytes_t loaded_from_resource;

	if (m_bassdll)
	{
		if (m_no_sound)
			return HEFFECT(1);

		if (size)
		{
			data = (void *) filename;
			_size = size;
		}
		else
		{
			loaded_from_resource = Resource_Load(filename, &_size);
			if (!loaded_from_resource)
				return HEFFECT();
			data = loaded_from_resource.get();
		}

		hs = BASS_SampleLoad(TRUE, data, 0, _size, 4, BASS_SAMPLE_OVER_VOL);
		if (!hs)
		{
			hstrm = HSTREAM(BASS_StreamCreateFile(TRUE, data, 0, _size, BASS_STREAM_DECODE));
			if (hstrm)
			{
				length = (uint32_t) BASS_ChannelGetLength(hstrm);
				BASS_ChannelGetInfo(hstrm, &info);
				samples = length;
				if (info.chans < 2)
					samples >>= 1;
				if ((info.flags & BASS_SAMPLE_8BITS) == 0)
					samples >>= 1;
				buffer = BASS_SampleCreate(samples, info.freq, 2, 4, info.flags
						| BASS_SAMPLE_OVER_VOL);
				if (!buffer)
				{
					BASS_StreamFree(hstrm);
					_PostError("Can't create sound effect: Not enough memory");
				}
				else
				{
					BASS_ChannelGetData(hstrm, buffer, length);
					hs = BASS_SampleCreateDone();
					BASS_StreamFree(hstrm);
					if (!hs)
						_PostError("Can't create sound effect");
				}
			}
		}

// 		if (!size)
// 			Resource_Free(data);
		return HEFFECT(hs);
	}
	else
		return HEFFECT();
}

HCHANNEL HGE_CALL HGE_Impl::Effect_Play(HEFFECT eff)
{
	if (m_bassdll)
	{
		HCHANNEL chn(BASS_SampleGetChannel(eff, false));
		BASS_ChannelPlay(chn, TRUE);
		return chn;
	}
	else
		return HCHANNEL();
}

HCHANNEL HGE_CALL HGE_Impl::Effect_PlayEx(HEFFECT eff, int volume, int pan, float pitch, bool loop)
{
	if (m_bassdll)
	{
		BASS_SAMPLE info;
		BASS_SampleGetInfo(eff, &info);

		HCHANNEL chn(BASS_SampleGetChannel(eff, false));
		BASS_ChannelSetAttributes(chn, (int) (pitch * info.freq), volume, pan);

		info.flags &= ~BASS_SAMPLE_LOOP;
		if (loop)
			info.flags |= BASS_SAMPLE_LOOP;
		BASS_ChannelSetFlags(chn, info.flags);
		BASS_ChannelPlay(chn, TRUE);
		return chn;
	}
	else
		return HCHANNEL();
}

void HGE_CALL HGE_Impl::Effect_Free(HEFFECT eff)
{
	if (m_bassdll)
		BASS_SampleFree(eff);
}

HMUSIC HGE_CALL HGE_Impl::Music_Load(const char *filename, uint32_t size)
{
	void * data;
	bytes_t loaded_from_resource;
	uint32_t _size;
	if (m_bassdll)
	{
		if (size)
		{
			data = (void *) filename;
			_size = size;
		}
		else
		{
			loaded_from_resource = Resource_Load(filename, &_size);
			if (!loaded_from_resource)
				return HMUSIC();
			data = loaded_from_resource.get();
		}

		HMUSIC hm(
				BASS_MusicLoad(TRUE, data, 0, 0, BASS_MUSIC_PRESCAN | BASS_MUSIC_POSRESETEX | BASS_MUSIC_RAMP, 0)
				);
		if (!hm)
			_PostError("Can't load music");
// 		if (!size)
// 			Resource_Free(data);
		return hm;
	}
	else
		return HMUSIC();
}

HCHANNEL HGE_CALL HGE_Impl::Music_Play(HMUSIC mus, bool loop, int volume, int order, int row)
{
	if (m_bassdll)
	{
		uint32_t pos = BASS_MusicGetOrderPosition(mus);
		if (order == -1)
			order = LOWORD(pos);
		if (row == -1)
			row = HIWORD(pos);
		BASS_ChannelSetPosition(mus, MAKEMUSICPOS(order, row));

		BASS_CHANNELINFO info;
		BASS_ChannelGetInfo(mus, &info);
		BASS_ChannelSetAttributes(mus, info.freq, volume, 0);

		info.flags &= ~BASS_SAMPLE_LOOP;
		if (loop)
			info.flags |= BASS_SAMPLE_LOOP;
		BASS_ChannelSetFlags(mus, info.flags);

		BASS_ChannelPlay(mus, false);

		return HCHANNEL(mus.get());
	}
	else
		return HCHANNEL();
}

void HGE_CALL HGE_Impl::Music_Free(HMUSIC mus)
{
	if (m_bassdll)
		BASS_MusicFree(mus.get());
}

void HGE_CALL HGE_Impl::Music_SetAmplification(HMUSIC music, int ampl)
{
	if (m_bassdll)
		BASS_MusicSetAttribute(music, BASS_MUSIC_ATTRIB_AMPLIFY, ampl);
}

int HGE_CALL HGE_Impl::Music_GetAmplification(HMUSIC music)
{
	if (m_bassdll)
		return BASS_MusicGetAttribute(music, BASS_MUSIC_ATTRIB_AMPLIFY);
	else
		return -1;
}

int HGE_CALL HGE_Impl::Music_GetLength(HMUSIC music)
{
	if (m_bassdll)
	{
		return BASS_MusicGetOrders(music);
	}
	else
		return -1;
}

void HGE_CALL HGE_Impl::Music_SetPos(HMUSIC music, int order, int row)
{
	if (m_bassdll)
	{
		BASS_ChannelSetPosition(music, MAKEMUSICPOS(order, row));
	}
}

bool HGE_CALL HGE_Impl::Music_GetPos(HMUSIC music, int *order, int *row)
{
	if (m_bassdll)
	{
		uint32_t pos;
		pos = BASS_MusicGetOrderPosition(music);
		if (pos == -1)
			return false;
		*order = LOWORD(pos);
		*row = HIWORD(pos);
		return true;
	}
	else
		return false;
}

void HGE_CALL HGE_Impl::Music_SetInstrVolume(HMUSIC music, int instr, int volume)
{
	if (m_bassdll)
	{
		BASS_MusicSetAttribute(music, BASS_MUSIC_ATTRIB_VOL_INST + instr, volume);
	}
}

int HGE_CALL HGE_Impl::Music_GetInstrVolume(HMUSIC music, int instr)
{
	if (m_bassdll)
	{
		return BASS_MusicGetAttribute(music, BASS_MUSIC_ATTRIB_VOL_INST + instr);
	}
	else
		return -1;
}

void HGE_CALL HGE_Impl::Music_SetChannelVolume(HMUSIC music, int channel, int volume)
{
	if (m_bassdll)
	{
		BASS_MusicSetAttribute(music, BASS_MUSIC_ATTRIB_VOL_CHAN + channel, volume);
	}
}

int HGE_CALL HGE_Impl::Music_GetChannelVolume(HMUSIC music, int channel)
{
	if (m_bassdll)
	{
		return BASS_MusicGetAttribute(music, BASS_MUSIC_ATTRIB_VOL_CHAN + channel);
	}
	else
		return -1;
}

HSTREAM HGE_CALL HGE_Impl::Stream_Load(const char *filename, uint32_t size)
{
	void * data;
	bytes_t loaded_from_resource;
	uint32_t _size;
	CStreamList *stmItem;

	if (m_bassdll)
	{
		if (m_no_sound)
			return HSTREAM(1);

		if (size)
		{
			data = (void *) filename;
			_size = size;
		}
		else
		{
			loaded_from_resource = Resource_Load(filename, &_size);
			if (!loaded_from_resource)
				return HSTREAM();
			data = loaded_from_resource.get();
		}
		HSTREAM hs(BASS_StreamCreateFile(TRUE, data, 0, _size, 0));
		if (!hs)
		{
			_PostError("Can't load stream");
// 			if (!size)
// 				Resource_Free(data);
			return HSTREAM();
		}
		if (!size)
		{
			stmItem = new CStreamList;
			stmItem->hstream = hs;
			stmItem->data = loaded_from_resource;
			stmItem->next = m_streams_list;
			m_streams_list = stmItem;
		}
		return hs;
	}
	else
		return HSTREAM();
}

void HGE_CALL HGE_Impl::Stream_Free(HSTREAM stream)
{
	CStreamList *stmItem = m_streams_list, *stmPrev = 0;

	if (m_bassdll)
	{
		while (stmItem)
		{
			if (stmItem->hstream == stream)
			{
				if (stmPrev)
					stmPrev->next = stmItem->next;
				else
					m_streams_list = stmItem->next;
				//Resource_Free(stmItem->data);
				delete stmItem;
				break;
			}
			stmPrev = stmItem;
			stmItem = stmItem->next;
		}
		BASS_StreamFree(stream);
	}
}

HCHANNEL HGE_CALL HGE_Impl::Stream_Play(HSTREAM stream, bool loop, int volume)
{
	if (m_bassdll)
	{
		BASS_CHANNELINFO info;
		BASS_ChannelGetInfo(stream, &info);
		BASS_ChannelSetAttributes(stream, info.freq, volume, 0);

		info.flags &= ~BASS_SAMPLE_LOOP;
		if (loop)
			info.flags |= BASS_SAMPLE_LOOP;
		BASS_ChannelSetFlags(stream, info.flags);
		BASS_ChannelPlay(stream, TRUE);
		return HCHANNEL(stream.get());
	}
	else
		return HCHANNEL();
}

void HGE_CALL HGE_Impl::Channel_SetPanning(HCHANNEL chn, int pan)
{
	if (m_bassdll)
		BASS_ChannelSetAttributes(chn, -1, -1, pan);
}

void HGE_CALL HGE_Impl::Channel_SetVolume(HCHANNEL chn, int volume)
{
	if (m_bassdll)
		BASS_ChannelSetAttributes(chn, -1, volume, -101);
}

void HGE_CALL HGE_Impl::Channel_SetPitch(HCHANNEL chn, float pitch)
{
	if (m_bassdll)
	{
		BASS_CHANNELINFO info;
		BASS_ChannelGetInfo(chn, &info);
		BASS_ChannelSetAttributes(chn, (int) (pitch * info.freq), -1, -101);
	}
}

void HGE_CALL HGE_Impl::Channel_Pause(HCHANNEL chn)
{
	if (m_bassdll)
		BASS_ChannelPause(chn);
}

void HGE_CALL HGE_Impl::Channel_Resume(HCHANNEL chn)
{
	if (m_bassdll)
		BASS_ChannelPlay(chn, false);
}

void HGE_CALL HGE_Impl::Channel_Stop(HCHANNEL chn)
{
	if (m_bassdll)
		BASS_ChannelStop(chn);
}

void HGE_CALL HGE_Impl::Channel_PauseAll()
{
	if (m_bassdll)
		BASS_Pause();
}

void HGE_CALL HGE_Impl::Channel_ResumeAll()
{
	if (m_bassdll)
		BASS_Start();
}

void HGE_CALL HGE_Impl::Channel_StopAll()
{
	if (m_bassdll)
	{
		BASS_Stop();
		BASS_Start();
	}
}

bool HGE_CALL HGE_Impl::Channel_IsPlaying(HCHANNEL chn)
{
	if (m_bassdll)
	{
		if (BASS_ChannelIsActive(chn) == BASS_ACTIVE_PLAYING)
			return true;
		else
			return false;
	}
	else
		return false;
}

float HGE_CALL HGE_Impl::Channel_GetLength(HCHANNEL chn)
{
	if (m_bassdll)
	{
		return BASS_ChannelBytes2Seconds(chn, BASS_ChannelGetLength(chn));
	}
	else
		return -1;
}

float HGE_CALL HGE_Impl::Channel_GetPos(HCHANNEL chn)
{
	if (m_bassdll)
	{
		return BASS_ChannelBytes2Seconds(chn, BASS_ChannelGetPosition(chn));
	}
	else
		return -1;
}

void HGE_CALL HGE_Impl::Channel_SetPos(HCHANNEL chn, float fSeconds)
{
	if (m_bassdll)
	{
		BASS_ChannelSetPosition(chn, BASS_ChannelSeconds2Bytes(chn, fSeconds));
	}
}

void HGE_CALL HGE_Impl::Channel_SlideTo(HCHANNEL channel, float time, int volume, int pan,
		float pitch)
{
	if (m_bassdll)
	{
		BASS_CHANNELINFO info;
		BASS_ChannelGetInfo(channel, &info);

		int freq;
		if (pitch == -1)
			freq = -1;
		else
			freq = (int) (pitch * info.freq);

		BASS_ChannelSlideAttributes(channel, freq, volume, pan, uint32_t(time * 1000));
	}
}

bool HGE_CALL HGE_Impl::Channel_IsSliding(HCHANNEL channel)
{
	if (m_bassdll)
	{
		if (BASS_ChannelIsSliding(channel))
			return true;
		else
			return false;
	}
	else
		return false;
}

//////// Implementation ////////


bool HGE_Impl::_SoundInit()
{
	if (!m_use_sound || m_bassdll)
		return true;

	m_bassdll = LoadLibrary("bass.dll");
	if (!m_bassdll)
	{
		_PostError("Can't load BASS.DLL");
		return false;
	}

	LOADBASSFUNCTION(BASS_GetVersion);

	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		_PostError("Incorrect BASS.DLL version");
		return false;
	}

	LOADBASSFUNCTION(BASS_GetDeviceDescription);
	LOADBASSFUNCTION(BASS_Init);
	LOADBASSFUNCTION(BASS_Free);
	LOADBASSFUNCTION(BASS_Start);
	LOADBASSFUNCTION(BASS_Pause);
	LOADBASSFUNCTION(BASS_Stop);
	LOADBASSFUNCTION(BASS_SetConfig);
	//LOADBASSFUNCTION(BASS_ErrorGetCode);

	LOADBASSFUNCTION(BASS_SampleLoad);
	LOADBASSFUNCTION(BASS_SampleCreate);
	LOADBASSFUNCTION(BASS_SampleCreateDone);
	LOADBASSFUNCTION(BASS_SampleGetInfo);
	LOADBASSFUNCTION(BASS_SampleGetChannel);
	LOADBASSFUNCTION(BASS_SampleFree);

	LOADBASSFUNCTION(BASS_MusicLoad);
	LOADBASSFUNCTION(BASS_MusicFree);
	LOADBASSFUNCTION(BASS_MusicGetOrders);
	LOADBASSFUNCTION(BASS_MusicGetOrderPosition);
	LOADBASSFUNCTION(BASS_MusicSetAttribute);
	LOADBASSFUNCTION(BASS_MusicGetAttribute);

	LOADBASSFUNCTION(BASS_StreamCreateFile);
	LOADBASSFUNCTION(BASS_StreamFree);

	LOADBASSFUNCTION(BASS_ChannelGetInfo);
	LOADBASSFUNCTION(BASS_ChannelGetAttributes);
	LOADBASSFUNCTION(BASS_ChannelSetAttributes);
	LOADBASSFUNCTION(BASS_ChannelSlideAttributes);
	LOADBASSFUNCTION(BASS_ChannelIsSliding);
	LOADBASSFUNCTION(BASS_ChannelSetFlags);
	LOADBASSFUNCTION(BASS_ChannelGetData);
	LOADBASSFUNCTION(BASS_ChannelPlay);
	LOADBASSFUNCTION(BASS_ChannelPause);
	LOADBASSFUNCTION(BASS_ChannelStop);
	LOADBASSFUNCTION(BASS_ChannelIsActive);
	LOADBASSFUNCTION(BASS_ChannelGetLength);
	LOADBASSFUNCTION(BASS_ChannelGetPosition);
	LOADBASSFUNCTION(BASS_ChannelSetPosition);
	LOADBASSFUNCTION(BASS_ChannelSeconds2Bytes);
	LOADBASSFUNCTION(BASS_ChannelBytes2Seconds);

	m_no_sound = false;
	if (!BASS_Init(-1, m_sample_rate, 0, m_hwnd, NULL))
	{
		System_Log("BASS Init failed, using no sound");
		BASS_Init(0, m_sample_rate, 0, m_hwnd, NULL);
		m_no_sound = true;
	}
	else
	{
		System_Log("Sound Device: %s", BASS_GetDeviceDescription(1));
		System_Log("Sample rate: %ld\n", m_sample_rate);
	}

	//BASS_SetConfig(BASS_CONFIG_BUFFER, 5000);
	//BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 50);

	_SetFXVolume(m_fx_vol);
	_SetMusVolume(m_mus_vol);
	_SetStreamVolume(m_stream_vol);

	return true;
}

void HGE_Impl::_SoundDone()
{
	CStreamList *stmItem = m_streams_list, *stmNext;

	if (m_bassdll)
	{
		BASS_Stop();
		BASS_Free();

		//int err = BASS_ErrorGetCode(); 

		FreeLibrary(m_bassdll);
		m_bassdll = 0;

		while (stmItem)
		{
			stmNext = stmItem->next;
			//Resource_Free(stmItem->data);
			delete stmItem;
			stmItem = stmNext;
		}
		m_streams_list = 0;
	}
}

void HGE_Impl::_SetMusVolume(int vol)
{
	if (m_bassdll)
		BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, vol);
}

void HGE_Impl::_SetStreamVolume(int vol)
{
	if (m_bassdll)
		BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, vol);
}

void HGE_Impl::_SetFXVolume(int vol)
{
	if (m_bassdll)
		BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, vol);
}

} // namespace hge
