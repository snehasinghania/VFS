#include"VfsRepository.h"
#include "VfsNodeInfo.h"
using namespace std;
#ifndef VFSFILEINFO_H 
#define VFSFILEINFO_H
class VfsFile;
class VfsFolderInfo;
//class VfsRepository;

class VfsFileInfo : public VfsNodeInfo
{
	int file_offset;
	long file_bytes;
	string file_path;
	VfsFile* file_pointer;
	VfsFolderInfo* folder_pointer;
public:
	VfsFileInfo(string name,string p);
	VfsFileInfo(string name, string p , long size, int offset);
	void getHeader(HeaderRecord &obj);
	string retfilepath();
	void save(string path,fstream& file); //instantiate the file obj n delete
	void load(string path,fstream& repo_handler);
	
};

#endif
