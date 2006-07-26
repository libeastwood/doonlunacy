#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp" 

#include "ResMan.h"
#include "Settings.h"

//#include "boost/filesystem/fstream.hpp"    // ditto

#include <assert.h>

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
    assert(0); // this doesnt work
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
    unsigned char *b =  m_pakfile->getFile(path.string().c_str(), &filesize);
    buf = b;
    printf("read pak %s size %d\n", path.string().c_str(), filesize);
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

bool ResMan::addRes(std::string name)
{
    std::string fullpath = Settings::Instance()->GetDataDir();
    fullpath.append(name);
    printf("adding resource %s from %s...\n", name.c_str(), fullpath.c_str());
    bfs::path file (fullpath);
    Resource *res = NULL;

    if (bfs::exists(file))
    {
        printf("Using DIRResource for %s\n", name.c_str());
        res = new DIRResource(file);
    }
    else 
    {
        std::string pakname = file.string();
        pakname.append(".PAK");
        bfs::path pakpath (pakname);

        if (!bfs::exists(pakpath))
        {
            printf("Neither DIR or PAK found for %s\n", name.c_str());
            return false;
        }
        
        res = new PAKResource(pakpath);
    };

    m_resources[name.c_str()] = res;

    return true;
};


int ResMan::readFile(std::string name, unsigned char* buf)
{
    unsigned int p = name.find(':');
    assert(p != std::string::npos);

    std::string fsname = std::string(name, 0, p);
    std::string filename = std::string(name, p+1, 
                                       name.length() - fsname.length() - 1);

    printf("opening file from %s named %s...\n", fsname.c_str(), filename.c_str());

    Resource* res = m_resources[fsname];

    if (res == NULL)
    {
        printf("ERROR: cannot find file!\n");
        buf = 0;
        return 0;
    };
    
    return res->readFile(filename.c_str(), buf);
};
