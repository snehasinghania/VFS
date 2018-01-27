#ifndef VFSREPOSITORY_H 
#define VFSREPOSITORY_H
#include<iostream>
#include<string>
#include<map>
#include<vector>
using namespace std;

class VfsNodeInfo;

const int MAX_NAME_LEN = 200;
const int MAX_FOLDER_LEN = 200;
const int MAX_NODES = 2000;
	

struct HeaderRecord
{
	char node_name[MAX_NAME_LEN];
	int node_type;
	int offset;
	long size;
	char Folder_Path[MAX_FOLDER_LEN];
};

class VfsRepository
{	
	string repo_name;
	enum repo_status{OPEN,CLOSED} repostat;
	string repo_file_path;
	int num_node;
	map <string,VfsNodeInfo*> node_pointers;
public:
	VfsRepository();
	VfsRepository(string name);
	void create (string external_path, string path);
	void open(string n, string p); // open the binary file, read one header and create the instant of the correspoding node and add to the map
	void close(); // save the node info back to the repository
	void makeDir(string dir_path, string dir_name);
	void list(string path, vector<string> &content);
	void loadHeader();
	void saveHeader(); 
	void copyIn(string host_path , string ext_path);
	string getpath();
	void copyOut(string host_path , string ext_path);
};

#endif
