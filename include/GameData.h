#ifndef DUNE_GAMEDATA_H
#define DUNE_GAMEDATA_H

#include "houses.h"
#include "Definitions.h"

#include <map>
#include <string>

class GameData
{
	public:
		GameData(std::string path);
		~GameData();
		// FIXME: Since this object refers to m_surface, it's reference count will
		// never go below 1 and be freed by itself. Maybe intrusive_ptr can handle this?
		// In the meantime freeIfUnique() will free m_surface if reference count is 1.
		bool freeIfUnique();
		ImagePtr getImage();
		ImagePtr getImage(HOUSETYPE house);
		SoundPtr getSound();
		std::string getPath() { return m_path; }

	private:
		void drawImage();
		ImagePtr m_surface;
		std::map<HOUSETYPE, ImagePtr> m_remappedImg;
		SoundPtr m_sound;

		std::string m_path;
		uint32_t m_freeCounter;
		bool m_persistent;
};

#endif // DUNE_GAMEDATA_H
