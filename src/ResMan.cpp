#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp" 
#include "boost/format.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include "ResMan.h"
#include "Settings.h"
//#include "Log.h"

//#include "boost/filesystem/fstream.hpp"    // ditto

#include <assert.h>

namespace bfs = boost::filesystem;

FileLike::FileLike(unsigned char* buf, int size)
{
    m_buf = buf;
    m_size = size;
    m_pos = 0;
};

FileLike::~FileLike()
{
    delete m_buf;
};

void FileLike::read(void* buf, int size)
{
    memcpy(buf, &m_buf[m_pos], size);
    m_pos += size; 
};

void FileLike::seek(int offset)
{
    m_pos = offset;
};

// ------------------------------------------------------------------

Resource::Resource()
{
	mb_writable = false;
};

Resource::~Resource()
{

};

// ------------------------------------------------------------------

DIRResource::DIRResource(bfs::path path)
{
   m_path = path; 
};

unsigned char* DIRResource::readFile(bfs::path path, int *size)
{
    // TODO: seems to work now, tested on linux, please test it on windows. (otpetrik)
    bfs::path fullpath (m_path);
	fullpath /= path;

    FILE *file = fopen (fullpath.string().c_str(), "rb");
    fseek(file, 0, SEEK_END);
    int filesize = ftell(file);

    fseek(file, 0, SEEK_SET);

    unsigned char* buf = new unsigned char[filesize];

    fread(buf, filesize, 1, file);

    fclose(file);

    if (size != NULL) *size = filesize;

    return buf;
};

std::string DIRResource::readText(boost::filesystem::path path) 
{
	bfs::path fullpath (m_path);
	fullpath /= path;
	std::string file_contents;
	printf("opening file %s...\n", fullpath.string().c_str());
	std::ifstream file_stream(fullpath.string().c_str());
	
	assert( file_stream.is_open() );

	std::stringstream temp;
	temp << file_stream.rdbuf();

	file_contents = temp.str();
	//std::cout << file_contents << std::endl;

	return file_contents;
};


// ------------------------------------------------------------------

WritableDIRResource::WritableDIRResource(boost::filesystem::path path) : DIRResource(path)
{
	mb_writable = true;
};

void WritableDIRResource::writeText(boost::filesystem::path path, std::string text)
{
	bfs::path fullpath(m_path);
	fullpath /= path;

	std::ofstream file;
	file.open(fullpath.string().c_str());
	file << text;
	file.close();
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

unsigned char* PAKResource::readFile(bfs::path path, int *size)
{
    int filesize;
    unsigned char *buf =  m_pakfile->getFile(path.string().c_str(), &filesize);
    
    //RESMAN_LOG_INFO(boost::format("read pak %s size %d\n") % path.string().c_str() % filesize);

    assert(buf != NULL);
    assert(filesize != 0);

    if (size != NULL) *size = filesize;

    return buf;
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

    return addRes(name, res);
};

bool ResMan::addRes(std::string name, Resource *res)
{
	m_resources[name.c_str()] = res;	

	return true;
};

Resource* ResMan::getResource(std::string name, std::string& filename)
{
	unsigned int p = name.find(':');
    assert(p != std::string::npos);

    std::string fsname = std::string(name, 0, p);
    filename = std::string(name, p+1, 
                                       name.length() - fsname.length() - 1);

    printf("opening file from %s named %s...\n", fsname.c_str(), filename.c_str());

    Resource* res = m_resources[fsname];

    if (res == NULL)
    {
        printf("ERROR: cannot find file!\n");
        
        return NULL;
    };

	return res;
};

unsigned char*  ResMan::readFile(std::string name, int *size)
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
};

FileLike* ResMan::readFile(std::string name)
{
    int size;
    unsigned char* buf = readFile(name, &size);
    return new FileLike(buf, size);
};

std::string ResMan::readText(std::string name)
{
	std::string filename;
	Resource* res = getResource(name, filename);
	if (res == NULL) return "";

	assert(res != NULL);

	return res->readText(filename);
};
	
void ResMan::writeText(std::string name, std::string text)
{
	std::string filename;
	Resource* res = getResource(name, filename);
	if (res == NULL) 
	{
		printf("resource not found!\n");		
	};

	assert(res != NULL);

	res->writeText(filename, text);
}
