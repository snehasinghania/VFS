#include"VfsNodeInfo.h"
#include"VfsFolderInfo.h"
#include"VfsRepository.h"
#include"VfsFileInfo.h"
#include<cstring>
#include<vector>

VfsFolderInfo :: VfsFolderInfo(string fpath, string fname) : VfsNodeInfo(fname)
{
	
	Folder_path = fpath;
}

string VfsFolderInfo :: retPath()
{
	return Folder_path;
}

void VfsFolderInfo :: getHeader(HeaderRecord &obj)
{
	string Nname = retSTRING();
	//cout << "name  "  << Nname << endl;
	strcpy(obj.node_name,Nname.c_str()); // string to char*
	obj.node_type = 1;
	obj.offset = 0;
	obj.size = 0;
	string path = Folder_path + "/" + Nname ;
	strcpy(obj.Folder_Path ,path.c_str());
} 

void VfsFolderInfo :: addChild(VfsFolderInfo* f)
{
	//cout<<"adding child"<<endl;
	sub_folders.push_back(f);
}

void VfsFolderInfo :: addConnection(VfsFileInfo* f)
{
	parent_folder = this;
	file_pointers.push_back(f);
}


void VfsFolderInfo :: setParent(VfsFolderInfo* p)
{
	parent_folder = p;
}

void VfsFolderInfo :: printFolders(vector<string> &content)
{
	//cout<<sub_folders.size()<<endl;
	for(int i=0; i < sub_folders.size(); i++)
	{	
		//cout << sub_folders[i]->retSTRING() << endl;
		//cout << sub_folders[i]->retPath() << endl ;
		string path = sub_folders[i]->retPath() + "/" + sub_folders[i]->retSTRING() + "/";
		content.push_back(path);
		//cout<< "hello\t"<<content[i];
		
	}
	for(int i=0; i < file_pointers.size(); i++)
	{	
		string path = file_pointers[i]->retfilepath() + "/" + file_pointers[i]->retSTRING() ;
		content.push_back(path);
	}
	
}

