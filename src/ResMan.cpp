#include "ResMan.h"

#include "Log.h"
#include "Settings.h"
//#include "Log.h"

//#include "boost/filesystem/fstream.hpp"    // ditto

#include <eastwood/PakFile.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>

#include <assert.h>
#include <fstream>
#include <sstream>
#include <iostream>

namespace bfs = boost::filesystem;

FileLike::FileLike(unsigned char* buf, size_t size)
{
    m_buf = buf;
    m_size = size;
    m_pos = 0;
}

FileLike::~FileLike()
{
    free(m_buf);
}

void FileLike::read(void* buf, size_t size)
{
    memcpy(buf, &m_buf[m_pos], size);
    m_pos += size; 
}

void FileLike::seek(off_t offset)
{
    m_pos = offset;
}

// ------------------------------------------------------------------

Resource::Resource()
{
    mb_writable = false;
}

Resource::~Resource()
{

}

std::string Resource::getRealPath(std::string path)
{
    bfs::path fullpath(m_path);
    fullpath /= path;
    return fullpath.string();
}

// ------------------------------------------------------------------

DIRResource::DIRResource(bfs::path path)
{
    m_path = path; 
}

unsigned char* DIRResource::readFile(std::string path, size_t *size)
{
    bfs::path fullpath (m_path);
    fullpath /= path;

    FILE *file = fopen (fullpath.string().c_str(), "rb");
    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);

    fseek(file, 0, SEEK_SET);

    unsigned char* buf = new unsigned char[filesize];

    fread(buf, filesize, 1, file);

    fclose(file);

    if (size != NULL) *size = filesize;

    return buf;
}

std::string DIRResource::readText(std::string path) 
{
    bfs::path fullpath (m_path);
    fullpath /= path;
    std::string file_contents;
    LOG(LV_INFO, "ResMan", "Opening file %s...", fullpath.string().c_str());
    std::ifstream file_stream(fullpath.string().c_str());

    assert( file_stream.is_open() );

    std::stringstream temp;
    temp << file_stream.rdbuf();

    file_contents = temp.str();
    //std::cout << file_contents << std::endl;

    return file_contents;
}

bool DIRResource::exists(std::string path)
{
    bfs::path fullpath(m_path);
    fullpath /= path;
    return bfs::exists(fullpath);
}

// ------------------------------------------------------------------

WritableDIRResource::WritableDIRResource(std::string path, bool create) : DIRResource(path)
{
    mb_writable = true;
    if(create && !exists(path))
	bfs::create_directory(path);
}

void WritableDIRResource::writeText(std::string path, std::string text)
{
    bfs::path fullpath(m_path);
    fullpath /= path;

    std::ofstream file;
    file.open(fullpath.string().c_str());
    file << text;
    file.close();
}

// ------------------------------------------------------------------

PAKResource::PAKResource(bfs::path path)
{
    m_path = path;
    m_pakfile = new PakFile(path.string().c_str());
}

PAKResource::~PAKResource()
{
    delete m_pakfile;
}

unsigned char* PAKResource::readFile(std::string path, size_t *size)
{
    size_t filesize;
    unsigned char *buf =  m_pakfile->getFile(path.c_str(), &filesize);

    //RESMAN_LOG(LV_INFO, boost::format("read pak %s size %d\n") % path.string().c_str() % filesize);

    assert(buf != NULL);
    assert(filesize != 0);

    if (size != NULL) *size = filesize;

    return buf;
}

bool PAKResource::exists(std::string path)
{
    for (unsigned int i = 0; i != m_pakfile->getNumFiles(); i++)
    {
	if (m_pakfile->getFileName(i) == path) return true;
    };
    return false;
}

// ------------------------------------------------------------------

ResMan::ResMan()
{

}

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
}

bool ResMan::addRes(std::string name)
{
    bfs::path fullpath(Settings::Instance()->GetDataDir());
    fullpath /= name;
    LOG(LV_INFO, "ResMan", "Adding resource %s from %S...", name.c_str(), &(ConstString)fullpath.string());
    bfs::path file (fullpath);
    Resource *res = NULL;

    if (bfs::exists(file))
    {
	LOG(LV_INFO, "ResMan", "Using DIRResource for %s", name.c_str());
	res = new DIRResource(file);
    }
    else 
    {
	std::string pakname = file.string();
	pakname.append(".PAK");
	bfs::path pakpath (pakname);

	if (!bfs::exists(pakpath))
	{
	    LOG(LV_ERROR, "ResMan", "Neither DIR or PAK found for %s", name.c_str());
	    return false;
	}

	res = new PAKResource(pakpath);
    };

    return addRes(name, res);
}

bool ResMan::addRes(std::string name, Resource *res)
{
    m_resources[name.c_str()] = res;	

    return true;
}

Resource* ResMan::getResource(std::string name, std::string& filename)
{
    unsigned int p = name.find(':');
    assert(p != std::string::npos);

    std::string fsname = std::string(name, 0, p);
    filename = std::string(name, p+1, name.length() - fsname.length() - 1);

    LOG(LV_INFO, "ResMan", "Opening file from %s named %s...", fsname.c_str(), filename.c_str());

    Resource* res = m_resources[fsname];

    if (res == NULL)
    {
	LOG(LV_WARNING, "ResMan", "Cannot find %s:%s", fsname.c_str(), filename.c_str());

	return NULL;
    };

    return res;
}

bool ResMan::exists(std::string path)
{
    std::string filename;
    Resource* res = getResource(path, filename);

    if (res == NULL)
    {
	return false;
    };

    return res->exists(filename);
}

std::string ResMan::getRealPath(std::string path)
{
    std::string filename;
    Resource *res = getResource(path, filename);

    /*if (res == NULL)
	throw*/

    return res->getRealPath(filename);
}

unsigned char*  ResMan::readFile(std::string name, size_t *size)
{
    std::string filename;
    Resource* res = getResource(name, filename);

    if (res == NULL) 
    {
	if (size != NULL) size = 0;
	return NULL;
    };

    unsigned char *buf = res->readFile(filename.c_str(), size);

    assert(buf != NULL);

    return buf;
}

FileLike* ResMan::readFile(std::string name)
{
    size_t size;
    unsigned char* buf = readFile(name, &size);
    return new FileLike(buf, size);
}

std::string ResMan::readText(std::string name)
{
    std::string filename;
    Resource* res = getResource(name, filename);
    if (res == NULL) return "";

    assert(res != NULL);

    return res->readText(filename);
}

void ResMan::writeText(std::string name, std::string text)
{
    std::string filename;
    Resource* res = getResource(name, filename);
    if (res == NULL) 
    {
	LOG(LV_ERROR, "ResMan", "Resource not found!");
	assert(0);
	return;
    };

    if (!res->isWritable())
    {
	LOG(LV_ERROR, "ResMan", "Resource not writable!\n");
	assert(0);
	return;
    };

    res->writeText(filename, text);
}
