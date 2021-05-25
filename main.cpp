#include <dlfcn.h>
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <map>
#include <string>
#include <ctype.h>
using namespace std;
typedef void (*TP_PRINT)(void);
typedef int (*TP_GETID)(void);
typedef string (*TP_GETFUNCTION)(void);
void useage(void){
	cout<<"useage:command help"<<endl;
	cout<<"       command ID [ID]..."<<endl;
}
int main(int argc,char **argv){
	if(argc < 2){	
		useage();	
		return 0;
	}

	map<int,string> Map_dl;
	map<int,string> Map_function;

	TP_PRINT Print;
	TP_GETID GetID;
	TP_GETFUNCTION GetFunction;

	DIR *pDir;
	void* Hd;
	if( !(pDir = opendir("./plugin")) ){
		cout<<"opendir fail!"<<endl;
		return 0;
	}
	struct dirent *pDirent;
	while((pDirent = readdir(pDir)) != NULL){
		if(pDirent->d_type == DT_REG){
			string buf = "./plugin/";
			buf = buf + pDirent->d_name;
			Hd = dlopen(buf.c_str(),RTLD_LAZY);
			if(Hd == 0){
				cout<<"dlopen fail!"<<endl;
				return 0;
			}
			GetID = (TP_GETID)dlsym(Hd,"GetID");
			if(GetID == NULL){
				cout<<dlerror()<<endl;
				return 0;
			}
			int ID = GetID();
			Map_dl[ID] = buf;
			GetFunction = (TP_GETFUNCTION)dlsym(Hd,"GetFunction");
			if(GetFunction == NULL){
				cout<<dlerror()<<endl;
				return 0;
			}
			Map_function[ID] = GetFunction();
			dlclose(Hd);	
		}
	}
	closedir(pDir);

	if(!strcmp(argv[1],"help")){
		map<int,string>::iterator it;
    		for(it = Map_function.begin();it != Map_function.end();it++)
         		 cout<<"ID: "<<it->first<<"\tfunction: "<<it->second<<endl;
		return 0;
	}

	for(int i= 1;i<argc;i++){
		int ID=atoi(argv[i]);
		Hd = dlopen(Map_dl[ID].c_str(),RTLD_LAZY);
		if(Hd == 0){
			cout<<"dlopen fail!"<<endl;
			return 0;
		}
		Print = (TP_PRINT)dlsym(Hd,"Print");
		if(Print == NULL){
			cout<<dlerror()<<endl;
			return 0;
		}
		Print();
		dlclose(Hd);
	}

	return 0;
}
