#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
using namespace std;

int main(int argc, char *argv[]){
	
	if (argc==1){
		return 0;
	}
	int size = argc - 1;
	char **buf = new char*[size];
	for(int i = 0;i<size;i++){
		buf[i] = argv[i+1]; 
	}
	
	for(int i = 0;i<size;i++){
	cout << buf[0];
	}
	cout<<endl<<"--------------------"<<endl;
	
	DIR *dir;
	struct dirent *entry;
	dir = opendir(buf[0]);
	cout <<dir<<endl;
	if(!dir){
		perror("diropen");
		exit(1);
	}	
	//вынести в рекурсию
	while((entry = readdir(dir))!=NULL){
		if(entry->d_type == DT_DIR){
		//Входим в подкаталог
		}
		
		cout <<entry->d_name<<" "<<entry->d_type<<endl;
			
	}
	closedir(dir);
	
	return 0;	
}

