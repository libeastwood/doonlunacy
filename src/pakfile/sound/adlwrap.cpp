#include <SDL_mixer.h>
#include <opl.h>

#include "pakfile/sound/adlwrap.h"
#include "ResMan.h"

AdlibPlayer::AdlibPlayer(Copl* newopl) : CadlPlayer(newopl){
	Mix_QuerySpec(&m_freq, &m_format, &m_channels);
}

void AdlibPlayer::callback(void *userdata, Uint8 *audiobuf, int len)
{
	AdlibPlayer *self = (AdlibPlayer *)userdata;
	static long   minicnt = 0;
	long          i, towrite = len / self->getsampsize();
	char          *pos = (char *)audiobuf;

	// Prepare audiobuf with emulator output
	while(towrite > 0) {
		while(minicnt < 0) {
			minicnt += self->m_freq;
			self->playing = self->update();
		}
		i = std::min(towrite, (long)(minicnt / self->getrefresh() + 4) & ~3);
		self->opl->update((short *)pos, i);
		pos += i * self->getsampsize(); towrite -= i;
		minicnt -= (long)(self->getrefresh() * i);
	}
}
