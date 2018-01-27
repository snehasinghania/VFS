#ifndef VFSNODEINFO_H 
#define VFSNODEINFO_H

#include<string>
#include"VfsRepository.h"
using namespace std;

//class VfsRepository;

class VfsNodeInfo
{
	string node_name;
	enum node_type{FILE,FOLDER};
	VfsRepository* repo_pointer;
public :
	VfsNodeInfo(string nstring);
	string retSTRING();
	virtual void getHeader(HeaderRecord &obj) = 0 ;
	//virtual string path_name();
};

#endif
