#ifndef DUNE_RESMAN_H
#define DUNE_RESMAN_H

#include "singleton.h"
#include "PakFile.h"

#include "boost/filesystem/path.hpp"

#include <map>
#include <string>
//#include "Log.h"
class binistream;
#include <binstr.h>
#include <fprovide.h>

/*!
	Class to emulate a file object using an unsigned char* buffer
*/
class FileLike
{
    public:
		//! @name Constructors & Destructor
        //@{
        FileLike(unsigned char* buf, int size);
        ~FileLike();
		//@}

		//! @name FileLike methods
        //@{
        /*! 
			read data from the buffer
            @param buf buffer to read data into 
			@param size amount of bytes to read
        */
        void read(void* buf, int size);
		/*!
			seek to a position in the buffer
			@param offset offset from the beginning of the buffer in bytes
		*/
        void seek(int offset);    
		//@}

    private:
        unsigned char* m_buf;
        int m_size;
        int m_pos;
};

class CProvider_Pakfile: public CFileProvider
{
public:
	virtual binisstream *open(std::string filename) const;
	virtual void close(binistream *f) const {}

};

/*!
	Base class for all resources. 
*/
class Resource
{
    public:
        //! @name Constructors & Destructor
        //@{
		Resource();
        virtual ~Resource();
		//@}

		/*!
			read a file from the resource.
			@param path path to the file to read
			@param size if not NULL the file size is put here 
			@return file data
		*/
        virtual unsigned char* readFile(std::string path, int *size) { return NULL; }
		
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
        DIRResource(boost::filesystem::path path) ;
        unsigned char* readFile(std::string path, int *size);
		std::string readText(std::string path);
		bool exists(std::string path);
};

/*!
	Writable directory resource for storing config files 
*/
class WritableDIRResource : public DIRResource
{
	public:
		WritableDIRResource(std::string path);
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
		unsigned char* readFile(std::string path, int *size);
		bool exists(std::string path);
    private:
        PakFile *m_pakfile;
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
        unsigned char* readFile(std::string path, int *size);
		/*!
			read a file from the resource.
			@param path path to the file to read
			@return FileLike object
		*/
        FileLike* readFile(std::string path);
		//@}

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

    private:
        ResList m_resources;
};

#endif // DUNE_RESMAN_H



