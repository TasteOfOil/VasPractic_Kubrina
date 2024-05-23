#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <cstring>
using namespace std;

void readDir(int fd);
void readDir(int fd, DIR *d_fd, struct dirent *entry);

int main(int argc, char *argv[]){
	if(argc==1){
		return 0;
	}
	string path;
	if(argc == 2){
		path = argv[1];
	}
	cout <<path<<endl;
	int fd;
	if((fd = open(path.data(),O_RDONLY))==-1){
		perror("Open file");
		exit(1);
	}
	struct stat buf;
	int res;
	if((res = fstat(fd, &buf))==-1){
		perror("Status error");
		exit(1);
	}
	if(S_ISDIR(buf.st_mode)){
		unsigned int start_time =  clock();
		readDir(fd);
		unsigned int end_time = clock();
                unsigned int search_time = end_time - start_time;
                cout <<"Время работы программы: "<<search_time<<endl;
	}
	else{
		cout <<"This is not a catalog!"<<endl;
	}


	close(fd);
	return 0;
}


void readDir(int fd){
	struct stat buf;
	int res;
	DIR * d_fd;
	if((res = fstat(fd, &buf))==-1){
                perror("Error");
                exit(1);
        }
	if((d_fd = fdopendir(fd))==NULL){
		perror("Error open dir");
		return;
	}
	struct dirent *entry;
	if((entry = readdir(d_fd))!=NULL){
		readDir(fd, d_fd, entry);
	}
}

void readDir(int fd, DIR *d_fd, struct dirent *entry){
	int temp_fd;
	DIR* temp_dfd;
	struct dirent *temp_entry;
	if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name,"..")!=0){
        	switch(entry->d_type){
			case DT_DIR:
				temp_fd = openat(fd,entry->d_name,O_RDONLY);
				if((temp_dfd = fdopendir(temp_fd))==NULL){
					perror("Error open dir");
					return;
				}	
				temp_entry = readdir(temp_dfd);
				readDir(temp_fd, temp_dfd, temp_entry);
				break;
			case DT_LNK:
				cout <<entry->d_name<<endl;
				break;
			case DT_REG:
				cout <<entry->d_name<<endl;
				break;
		}        
        
	}
	if((entry=readdir(d_fd))!=NULL){
		readDir(fd,d_fd,entry);
	}

}
