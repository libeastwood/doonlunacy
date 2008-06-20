#ifndef H_ADLWRAP
#define H_ADLWRAP
#include <emuopl.h>

#include "SDL.h"
#include "SDL_mixer.h"

#include <adl.h>
#include <libbinio/binio.h>
#include <libbinio/binstr.h>
class AdlibPlayer : public CadlPlayer
{
	public:
		AdlibPlayer(Copl *newopl);
		AdlibPlayer();
		//~AdlibPlayer();
		static void callback(void *userdata, Uint8 *audiobuf, int len);
		int m_channels;
		int m_freq;
		Uint16 m_format;
		bool playing;
		unsigned char getsampsize() { 
			return m_channels * (m_format == AUDIO_U8 ? 1 : 2); }
};

#endif
