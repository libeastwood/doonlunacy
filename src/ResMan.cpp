#include "ResMan.h"

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"    // ditto
namespace bfs = boost::filesystem;

Resource::~Resource()
{

};

// ------------------------------------------------------------------

DIRResource::DIRResource(bfs::path path)
{
   m_path = path; 
};

int DIRResource::readFile(bfs::path path, unsigned char* buf)
{
    bfs::path fullpath (m_path.string() + path.string());

    FILE *file = fopen (fullpath.string().c_str(), "rb");
    fseek(file, 0, SEEK_END);
    int filesize = ftell(file);

    fseek(file, 0, SEEK_SET);

    buf = new unsigned char[filesize];

    fread(buf, filesize, 1, file);

    fclose(file);

    return filesize;
};

// ------------------------------------------------------------------

PAKResource::PAKResource(bfs::path path)
{
    m_path = path;
    m_pakfile = new Pakfile(path.string().c_str());
};

PAKResource::~PAKResource()
{
    delete m_pakfile;
};

int PAKResource::readFile(bfs::path path, unsigned char* buf)
{
    int filesize;
    buf = m_pakfile->getFile(path.string().c_str(), &filesize);
    return filesize;
};

// ------------------------------------------------------------------

ResMan::ResMan()
{

};

ResMan::~ResMan()
{
    ResList::iterator it;
    for (it = m_resources.begin();
         it != m_resources.end();
         ++it)
    {
        delete (it->second);
    };

    m_resources.clear();
};

bool ResMan::addRes(const char* name)
{
    bfs::path file (name);
    Resource *res = NULL;

    if (bfs::is_directory(file))
    {
        res = new DIRResource(file);
    }
    else
    {
        res = new PAKResource(file);
    };

    // this isnt going to work..
    if (res == NULL)
    {
        return false;
    };

    m_resources[name] = res;

    return true;
};



