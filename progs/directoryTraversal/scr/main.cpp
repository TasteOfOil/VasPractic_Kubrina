#include <iostream>
#include <cstring>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

void readDir(int fd, int count);

int main(int argc, char *argv[]){
	
	if (argc==1){
		return 0;
	}

	string path;
	path = argv[1];
	
	int fd ;
	if((fd = open(path.data(),O_RDONLY)) == -1){
		perror("Error open file");
		exit(1);
	}
      	struct stat buf;
	int res ;
	if((res = fstat(fd, &buf))==-1){
		perror("Error");
		exit(1);
	}
	if(S_ISDIR(buf.st_mode)){
		try{
			readDir(fd,0);	
		}
		catch(const char* error_mes){
			cout <<error_mes<<endl;
			exit(1);
		}
	}
	close(fd);
	return 0;	
}



void readDir(int fd, int count){
	++count;
	if(count > 20){
		//cout <<"END"<<endl;
		throw "Error::looping";
	}	
      	DIR * d_fd;
	if((d_fd = fdopendir(fd)) == NULL){
		perror("Error open dir");
		exit(1);
	}
	struct dirent *entry;
	while ((entry = readdir(d_fd)) != NULL){
		if(entry->d_name[0]=='.'){
			continue;
		}	
		switch(entry->d_type){
			case DT_DIR:
				//cout <<"DIR: "<<count <<" "<<entry->d_name<<endl;
				cout <<entry->d_name<<endl;
				readDir(openat(fd,entry->d_name,O_RDONLY), count);
				break;
			case DT_LNK:
				cout <<"link"<<endl;
				//readDir(openat(fd,entry->d_name, O_RDONLY), count);
				break;
			case DT_REG:
				//cout <<"file: "<<count<<" "<<entry->d_name<<endl;
				cout <<entry->d_name<<endl;
				break;
		}
	}	
	closedir(d_fd);	
}
