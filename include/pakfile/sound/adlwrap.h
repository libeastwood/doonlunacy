#ifndef H_ADLWRAP
#define H_ADLWRAP
#include <SDL.h>

#include <adl.h>
class Copl;

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
			return m_channels * (m_format == AUDIO_S8 ? 1 : 2); }
};

#endif
