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
	cout<<endl;
	
	DIR *dir;
	struct dirent *entry;
	dir = opendir(buf[0]);
	if(!dir){
		perror("diropen");
		exit(1);
	}	

	while((entry = readdir(dir))!=NULL){
		cout <<entry->d_ino<<" "<<entry->d_name<<" "<<entry->d_type<<endl; 
	}
	closedir(dir);
	
	return 0;	
}
