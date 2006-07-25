#ifndef DUNE_RESMAN_H
#define DUNE_RESMAN_H

#include "singleton.h"
#include "pakfile/Pakfile.h"

#include "boost/filesystem/path.hpp"

#include <map>

class Resource
{
    public:
        Resource() {};
        virtual long readFile(boost::filesystem::path path, char* buf) { return 0; }

    protected:
        boost::filesystem::path m_path;
};


class DIRResource : public Resource
{
    public:
        DIRResource(boost::filesystem::path path) {};
        virtual long readFile(boost::filesystem::path path, char* buf) {return 0;}
    
};


class PAKResource : public Resource
{
    public:
        PAKResource(boost::filesystem::path path) {};
        ~PAKResource();
        virtual long readFile(boost::filesystem::path path, char* buf) {return 0;}
    
    private:
        Pakfile *m_pakfile;
};


class ResMan : public Singleton<ResMan>
{
    friend class Singleton<ResMan>;

    typedef std::map<const char*, Resource*> ResList;

    protected:
        ResMan();
        ~ResMan();

    public:
        bool addRes(const char* name);

        long readFile(const char* path, char* buf);

    private:
        ResList m_resources;

};

#endif // DUNE_RESMAN_H


