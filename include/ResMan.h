#ifndef DUNE_RESMAN_H
#define DUNE_RESMAN_H

#include "singleton.h"
#include "pakfile/Pakfile.h"

#include "boost/filesystem/path.hpp"

#include <map>
#include <string>

class Resource
{
    public:
        Resource() {};
        virtual ~Resource();
        virtual int readFile(boost::filesystem::path path, unsigned char* buf) { return 0; }

    protected:
        boost::filesystem::path m_path;
};


class DIRResource : public Resource
{
    public:
        DIRResource(boost::filesystem::path path) ;
        virtual int readFile(boost::filesystem::path path, unsigned char* buf) ;
    
};


class PAKResource : public Resource
{
    public:
        PAKResource(boost::filesystem::path path) ;
        ~PAKResource();
        virtual int readFile(boost::filesystem::path path, unsigned char* buf) ;
    
    private:
        Pakfile *m_pakfile;
};


class ResMan : public Singleton<ResMan>
{
    friend class Singleton<ResMan>;

    typedef std::map<std::string, Resource*> ResList;

    protected:
        ResMan();
        ~ResMan();

    public:
        bool addRes(std::string name);

        int readFile(std::string path, unsigned char* buf);

    private:
        ResList m_resources;

};

#endif // DUNE_RESMAN_H



