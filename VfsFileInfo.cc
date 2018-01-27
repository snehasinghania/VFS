#include"VfsNodeInfo.h"
#include"VfsFileInfo.h"
#include"VfsFile.h"
#include"VfsRepository.h"
#include<cstring>

VfsFileInfo :: VfsFileInfo(string name, string path ,long size, int offset) : VfsNodeInfo(name)
{
	file_offset = offset;
	file_bytes = size;
	file_path = path;

}

VfsFileInfo :: VfsFileInfo(string name,string path) : VfsNodeInfo(name)
{
	file_path = path;
}

string VfsFileInfo :: retfilepath()
{
	return file_path;
}

void VfsFileInfo :: getHeader(HeaderRecord &obj)
{
	string Nname = retSTRING();
	strcpy(obj.node_name,Nname.c_str());
	obj.node_type = 0;
	obj.offset = file_offset;
	obj.size = file_bytes;
	strcpy(obj.Folder_Path,file_path.c_str());
} 

void VfsFileInfo :: save(string ext_path, fstream& file) //assume that the y path is already done. ie linking it to the corresponding folder
{
	VfsFile* obj = new VfsFile(); 
	obj -> saveInVFS(file, ext_path ,file_bytes, file_offset);
	 //delete obj;
}

void VfsFileInfo :: load(string ext_path, fstream& repo_handler)
{
	VfsFile* obj = new VfsFile(); 
	obj -> export_file(ext_path, repo_handler, file_bytes, file_offset);
	//delete obj;
}
