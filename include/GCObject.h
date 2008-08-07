#ifndef DUNE_GCOBJECT_H
#define DUNE_GCOBJECT_H

#include "DrawImage.h"
#include <string>

class GCObject
{
	public:
		GCObject(std::string path);
		// FIXME: Since this object refers to m_surface, it's reference count will
		// never go below 1 and be freed by itself. Maybe intrusive_ptr can handle this?
		// In the meantim freeIfUnique() will free m_surface if reference count is 1.
		void freeIfUnique();
		ImagePtr getImage();
		std::string getPath() { return m_path; }

	private:
		void drawImage();
		ImagePtr m_surface;
		std::string m_path;
};

#endif // DUNE_GCOBJECT_H
