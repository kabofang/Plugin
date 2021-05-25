#include <dlfcn.h>
int main(){
	void* Hd = dlopen("./libprint.so",RTLD_LAZY);
	void (*print)(void) =( void (*)(void) )dlsym(Hd,"print");
	print();
	dlclose(Hd);
	return 0;
}
