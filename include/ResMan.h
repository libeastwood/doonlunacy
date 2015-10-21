#ifndef DUNE_RESMAN_H
#define DUNE_RESMAN_H
#include "singleton.h"

#include <boost/filesystem/path.hpp>

#include <map>
#include <string>
//#include "Log.h"

#include <eastwood/PakFile.h>
#include <fstream>

/*!
  Base class for all resources. 
  */
class Resource
{
    public:
	//! @name Constructors & Destructor
	//@{
	Resource(boost::filesystem::path path, bool writable = false);
	virtual ~Resource();
	//@}

	/*!
	  read a file from the resource.
	  @param path path to the file to read
	  @param size if not NULL the file size is put here 
	  @return file data
	  */
	virtual eastwood::IStream& getFile(std::string path) = 0;

	/*!
	  read a text file from resource
	  @param path path to the file to open 
	  @return text from the file
	  */
	virtual std::string readText(std::string path) { return ""; }

	/*!
	  write a text file to a resource
	  @param path path to write the file
	  @param text text to write to file 
	  */
	virtual void writeText(std::string path, std::string text) {}

	/*!
	  return true if the resource can be written to 
	  */
	inline bool isWritable() { return mb_writable; }

	/*!
	  return true if the file exists
	  */
	virtual bool exists(std::string path) { return false; }

	std::string getRealPath(std::string path = "");

    protected:
	boost::filesystem::path m_path;

	bool mb_writable;

};

/*!
  Directory Resource - all files are read from a directory
  */
class DIRResource : public Resource
{
    public:
	DIRResource(boost::filesystem::path path, bool writable = false) ;
	eastwood::IStream& getFile(std::string path);
	std::string readText(std::string path);
	bool exists(std::string path);

    protected:
	std::ifstream mb_file;
};

/*!
  Writable directory resource for storing config files 
  */
class WritableDIRResource : public DIRResource
{
    public:
	WritableDIRResource(std::string path, bool writable = true);
	void writeText(std::string path, std::string text);
};

/*!
  PAK file resource - all files are read from a PAK file
  */
class PAKResource : public Resource
{
    public:
	PAKResource(boost::filesystem::path path) ;
	~PAKResource();

	eastwood::IStream& getFile(std::string path);
	bool exists(std::string path);
    private:

	std::fstream m_fstream;
	eastwood::PakFile m_pakfile;
};

/*!
  Class to simplify reading and writing from different resource files. 
  */
class ResMan : public Singleton<ResMan>
{
    friend class Singleton<ResMan>;

    typedef std::map<std::string, Resource*> ResList;

    protected:
    ResMan();
    ~ResMan();

    public:
    //! @name resource management
    //@{
    /*!
      add a resource to the manager. the resource will be searched for 
      in the directory pointed to by Settings::GetDataDir. It will 
      first search for a directory and then for the pak file
      @ param name name of the resource to open 
      @ return true on success
      */
    bool addRes(std::string name);
    /*!
      add an existing resource to the resource manager.
      */
    bool addRes(std::string name, Resource *res);

    Resource* getResource(std::string name, std::string& filename);
    //@}

    //! @name binary functions
    //@{
    /*!
      read a file from the resource.
      @param path path to the file to read
      @param size if not NULL the file size is put here 
      @return file data
      */
    eastwood::IStream& getFile(std::string path);


    //! @name textmode functions
    //@{
    /*!
      read a text file from resource
      @param path path to the file to open 
      @return text from the file
      */
    virtual std::string readText(std::string path);
    /*!
      write a text file to a resource
      @param path path to write the file
      @param text text to write to file 
      */
    virtual void writeText(std::string path, std::string text);
    //@}

    /*!
      return true if the path exists
      */
    bool exists(std::string path);

    std::string getRealPath(std::string path = "");

    private:
    ResList m_resources;
};

#endif // DUNE_RESMAN_H



