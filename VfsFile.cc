#include"VfsFile.h"
#include<iostream>
#include<cstring>
#include<fstream>
#include<sys/stat.h>
using namespace std;
#include"vfs_errors.h"

VfsFile :: VfsFile()
{
}

	
void VfsFile::saveInVFS( fstream& repo_handler, string file_path, long& size, int& file_offset )
{
	fstream fileptr;
	fileptr.open(file_path , ios::in | ios::binary);
	if (fileptr.fail())
		throw VFS_COPYIN_03;
	repo_handler.seekp(0,ios::end);
	file_offset = repo_handler.tellp();
	fileptr.seekg(0,ios::end);

	long file_size = fileptr.tellp();
	size = file_size;
	fileptr.seekp(0,ios::beg);
	while(file_size >= max_size)
	{
		if(file_size < max_size)
			break;
		fileptr.read(data_bytes,max_size);
		repo_handler.write(data_bytes,max_size);
		file_size = file_size - max_size;
		strcpy(data_bytes," ");
		if (fileptr.eof())
			break;
	}
	if (file_size > 0)
	{
		fileptr.read(data_bytes, file_size);
		repo_handler.write(data_bytes, file_size);
	}
	fileptr.close();
	//repo_handler.close();
}

void VfsFile :: export_file(string ext_path, fstream& repo_handler, long size, int offset)
{
	fstream fileptr;
	fileptr.open(ext_path, ios::out | ios :: binary);
	repo_handler.seekg(offset,ios::beg);
	while(size >= max_size)
	{
		repo_handler.read(data_bytes,max_size);
		fileptr.write(data_bytes,max_size);
		size -= max_size;
		if (repo_handler.eof())
			break;
	}
	if(size > 0)
	{
		repo_handler.read(data_bytes, size);
		fileptr.write(data_bytes, size);
	}
	fileptr.close();
	repo_handler.close();
}

