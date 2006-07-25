#include "ResMan.h"

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"    // ditto
namespace bfs = boost::filesystem;

DIRResource::DIRResource(bfs::path path)
{
   m_path = path; 
};

long DIRResource::readFile(bfs::path path, char* buf)
{
    bfs::path fullpath = m_path + path;

    FILE file (fullpath.c_str(), "rb");
    fseek(file, 0, SEEK_END);
    long filesize = ftell;

    fseek(file, 0, SEEK_SET);

    buf = new char[filesize];

    fread(buf, filesize, 1, file);

    fclose(file);

    return filesize;
};

// ------------------------------------------------------------------

PAKResource::PAKResource(bfs::path path)
{
    m_path = path;
    m_pakfile = Pakfile(path.c_str());
};

PAKResource::~PAKResource()
{
    delete m_pakfile;
};

long PAKResource::readFile(bfs::path path, char* buf)
{
    long filesize;
    buf = m_pakfile->getFile(path.c_str(), &filesize);
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
        delete *it;
    };

    m_resources.clear();
};

bool ResMan::addRes(const char* name)
{
    bfs::path file (name);
    Resource *res = NULL;

    if (bfs::is_directory(file))
    {
        res = DIRResource(file);
    }
    else
    {
        res = PAKResource(file);
    };

    // this isnt going to work..
    if (res == NULL)
    {
        return false;
    };

    m_resources[fn] = res;

    return true;
};



