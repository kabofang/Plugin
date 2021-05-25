#include <dlfcn.h>
#include <iostream>
using namespace std;
int main(){
	void* Hd = dlopen("./libprint.so",RTLD_LAZY);
	if(Hd == 0){
		cout<<"dlopen fail!"<<endl;
		return 0;
	}
	void (*print)(void) =( void (*)(void) )dlsym(Hd,"print");
	if(print == NULL){
		cout<<dlerror()<<endl;
		return 0;
	}
	print();
	dlclose(Hd);
	return 0;
}
