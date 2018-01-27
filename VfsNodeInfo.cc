#include"VfsRepository.h"
#include"VfsNodeInfo.h"
#include<string>

using namespace std;

VfsNodeInfo :: VfsNodeInfo(string nstring)
	{
	node_name =  nstring;
	}
	

string VfsNodeInfo :: retSTRING()
{
	return node_name;
}

//virtual HeaderRecord* getHeader(){}  
