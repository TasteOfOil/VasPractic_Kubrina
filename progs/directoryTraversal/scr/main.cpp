#include <iostream>
#include <cstring>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

using namespace std;

void readDir(DIR *dir, struct dirent *entry, string path);

int main(int argc, char *argv[]){
	
	if (argc==1){
		return 0;
	}

	string path;
	path = argv[1];
	cout <<path[0];
	cout<<endl<<"--------------------"<<endl;
	
	DIR *dir;
	struct dirent *entry;
	dir = opendir(path.data());
	//cout <<dir<<endl;
	readDir(dir,entry, path);
	closedir(dir);
	
	return 0;	
}


void readDir(DIR *dir, struct dirent *entry, string path){
	if(!dir){
		perror("diropen");
		exit(1);
	}
		
	
	string tempStr;
	while((entry = readdir(dir))!=NULL){
		if(entry->d_type == DT_DIR && entry->d_name[0] != '.'){
			cout <<"Catalog: "<<entry->d_name<<endl;
			tempStr = path + "/" + entry->d_name;
			//tempDir = opendir(tempStr.data());
			readDir(opendir(tempStr.data()),entry,tempStr);	
		}
		else if(entry->d_type == DT_REG){
			cout <<entry->d_name<<" "<<endl;
		}
	}
	//closedir(tempDir);
}
