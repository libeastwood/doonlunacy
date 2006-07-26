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
        virtual unsigned char* readFile(boost::filesystem::path path, int *size) { return NULL; }
    protected:
        boost::filesystem::path m_path;
};


class DIRResource : public Resource
{
    public:
        DIRResource(boost::filesystem::path path) ;
        unsigned char* readFile(boost::filesystem::path path, int *size);
};


class PAKResource : public Resource
{
    public:
        PAKResource(boost::filesystem::path path) ;
        ~PAKResource();
        unsigned char* readFile(boost::filesystem::path path, int *size);
    
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

        unsigned char* readFile(std::string name, int *size);

    private:
        ResList m_resources;

};

#endif // DUNE_RESMAN_H



