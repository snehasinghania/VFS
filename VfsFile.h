#ifndef VFSFILE_H 
#define VFSFILE_H

#include<fstream>
#include<string>

class VfsFileInfo;
using namespace std;
const int max_size = 1024;

class VfsFile
{
	char data_bytes[max_size];

	VfsFileInfo * ptr_fileInfo;
		
public:
	VfsFile();
	void export_file(string path, fstream& repo_handler,long size, int offset);
	void saveInVFS(fstream& container_file_name, string ext_path, long& size, int& file_offset );
};

#endif
