#ifndef VFSFOLDERINFO_H 
#define VFSFOLDERINFO_H

#include<string>
#include<vector>
#include"VfsRepository.h"

class VfsFileInfo;

class VfsNodeInfo;

//class VfsRepository;

class VfsFolderInfo : public VfsNodeInfo
{
	string Folder_path;
	std::vector<VfsFileInfo*> file_pointers;
	std::vector<VfsFolderInfo*> sub_folders;
	VfsFolderInfo* parent_folder;
public:
	VfsFolderInfo(string fpath, string fname);
	void addChild(VfsFolderInfo* f);
	void setParent(VfsFolderInfo* p);
	void printFolders(vector<string> &content);
	string retPath();
	void addConnection(VfsFileInfo* f);
	void getHeader(HeaderRecord &obj);
};

#endif
