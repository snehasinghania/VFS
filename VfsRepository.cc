#include<iostream>
#include<fstream>
#include<sys/stat.h>
#include"VfsRepository.h"
#include<map>
#include"VfsFileInfo.h"
#include"VfsNodeInfo.h"
#include"VfsFolderInfo.h"
#include<cstring>
#include"PathString.h"
#include"vfs_errors_def.h"
#include"vfs_errors.h"
using namespace std;

VfsRepository :: VfsRepository() {}

VfsRepository :: VfsRepository(string name)
{
	repo_name = name;
}

string VfsRepository :: getpath()
{
	return repo_file_path;
}

void VfsRepository :: create (string name, string repofilepath) // stand alone operation
{
	fstream file;
	repo_name = name;
	repo_file_path = repofilepath;
	num_node = 0;
	file.open(repo_file_path, ios::out | ios::binary);
	if(file.fail())
		throw(VFS_CREATE_01);
	file.close();
	file.open(repo_file_path, ios::in | ios::out | ios::binary);
	file.write((char*) &num_node ,sizeof(int));
	for(int i = 0 ; i < MAX_NODES ; i++)
	{	
		HeaderRecord obj;
		obj.node_type = -1;
		file.write((char*) &obj ,sizeof(HeaderRecord));
	}
	file.close();
}

void VfsRepository :: open(string name, string repofilepath) // cant distinguish between empty n filed header. so save an integer indicating the number of occupied nodes
{
	repostat = OPEN;
	node_pointers.clear();
	fstream fptr;
	repo_name = name;
	repo_file_path = repofilepath;
	fptr.open(repo_file_path, ios::in | ios::out |ios::binary);
	if(fptr.fail())
		throw(VFS_OPEN_01);
	fptr.seekg(0,ios::beg);
	fptr.read((char*)&num_node, sizeof(int));
	for(int i = 0; i < num_node ; i++)
	{
		HeaderRecord obj;
		fptr.read((char*) &obj, sizeof (HeaderRecord));
		if(obj.node_type == 0)
		{
			string nodename = obj.node_name;
			string path = obj.Folder_Path;
			VfsFileInfo* fileobj = new VfsFileInfo(nodename, path,  obj.size, obj.offset);
			VfsFolderInfo* folderobj = dynamic_cast<VfsFolderInfo*>(node_pointers[path]);
			folderobj->addConnection(fileobj);
			string final_path;
			if(path == "" && nodename == "")
				final_path = "/";
			else if(path == "" && nodename != "")
				final_path = path + "/" + nodename;
				
			else if(path =="/")
				final_path = path + nodename;
			else
				final_path = path + "/" + nodename;
			node_pointers[final_path] = fileobj;  //change here get path and add
		
			
		}
		else if(obj.node_type == 1)
		{
			string nodename = obj.node_name;
			string path = obj.Folder_Path;
			PathString p (path,"/");
			string fpath = p.excludeLast();
			string fname = p.getLast();
			makeDir(fpath,fname);
		}	
	}
}

void VfsRepository :: close()
{
	if(repostat!=OPEN)
		throw(VFS_GEN_01);	
	fstream fptr;
	fptr.open(repo_file_path, ios::in | ios::out | ios::binary);
	//fptr.seekp(0,ios::beg);
	map<string,VfsNodeInfo* >:: iterator i;
	num_node = node_pointers.size();
	fptr.write((char*) &num_node ,sizeof(int));
	for(i = node_pointers.begin() ; i != node_pointers.end() ; i++)
	{
		HeaderRecord obj ;
		i -> second -> getHeader(obj);
		fptr.write((char*) &obj ,sizeof(HeaderRecord));
		delete i -> second;
	}	
	for(int i = 0 ; i < (MAX_NODES - num_node) ; i++)
	{	
		HeaderRecord empty_obj;
		fptr.write((char*) &empty_obj ,sizeof(HeaderRecord));
	}
	
	node_pointers.clear();
	repostat = CLOSED;
	fptr.close();
}

void VfsRepository :: makeDir( string fpath, string fname ) // Create folder named fname in the path given by fpath
{
	string existing_path ;
	map<string,VfsNodeInfo* > :: iterator i;
	
	if (fpath == "" && fname == "")
		existing_path = "";
	else
		existing_path = fpath + "/" + fname;
	
	i = node_pointers.find(existing_path);
	
	if(i != node_pointers.end())
		throw(VFS_MAKEDIR_02);
	
	i = node_pointers.find(fpath);
	
	if(i != node_pointers.end())
	{
		string path = fpath + "/" + fname;
		VfsFolderInfo *f = new VfsFolderInfo(fpath, fname);
		node_pointers[path] = f;
		dynamic_cast<VfsFolderInfo*>(i->second)->addChild(f);
		f->setParent(dynamic_cast<VfsFolderInfo*>(i->second));
	}
	else if (fpath == "")
	{
		if(fname =="")
		{
			VfsFolderInfo *f = new VfsFolderInfo(fpath, fname);
			node_pointers[fpath] = f;
		}
		else
		{
			VfsFolderInfo* f = new VfsFolderInfo("","");
			node_pointers[""]=f;
			VfsFolderInfo *f2 = new VfsFolderInfo("",fname);
			string path = fpath + "/" + fname;
			node_pointers[path] = f2;
			f->addChild(f2);
			f2->setParent(f);
		}
	}
	else 
	{
		if (i == node_pointers.end())
			throw(VFS_MAKEDIR_01);
	}
}

void VfsRepository :: list(string path, vector<string> &content)
{
	map<string,VfsNodeInfo* >:: iterator i;
	if (path == "/")
		path = "";
	i = node_pointers.find(path);
	if (i == node_pointers.end())
		throw(VFS_LISTDIR_01);
	dynamic_cast<VfsFolderInfo*>(i->second)->printFolders(content);
	
}	

void VfsRepository :: copyIn(string ext_path , string host_path)
{
	
	if(node_pointers.find(host_path) != node_pointers.end())//target is already written
		throw VFS_COPYIN_02;
	PathString p (host_path,"/");
	string new_path = p.excludeLast();
	string last = p.getLast();
	
	if(new_path =="")
		new_path = "/";
	
	map <string,VfsNodeInfo* > :: iterator i;
	i = node_pointers.find(new_path);
	
	if(i == node_pointers.end())
		throw VFS_COPYIN_01;// target folder not found
	
	if(i != node_pointers.end())
	{
		VfsFileInfo *f = new VfsFileInfo(last, new_path); // ask for size nad offset
		node_pointers[host_path] = f;
		dynamic_cast<VfsFolderInfo*>(i->second)->addConnection(f);//write function for connecting
		fstream filestr;
		filestr.open(repo_file_path, ios::in | ios::out | ios::binary);
		f->save(ext_path,filestr);
		
	}
	
	
}


void VfsRepository::copyOut(string host_path, string ext_path)
{
	//fstream temp;
	//temp.open(ext_path, ios::out | ios::binary);
	map <string,VfsNodeInfo* > :: iterator i;
	i = node_pointers.find(host_path);	
	if(i == node_pointers.end())
		throw VFS_COPYOUT_01;
	VfsFileInfo *file_info = dynamic_cast <VfsFileInfo *> (node_pointers[host_path]);
	fstream repo_handler;
	repo_handler.open(repo_file_path, ios::in | ios::out | ios::binary);
	if(repo_handler.fail())
		throw VFS_COPYOUT_02;
	file_info->load(ext_path, repo_handler);
}
	
