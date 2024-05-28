#include <iostream>
#include <ctime>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
//#include <time.h>
#include <cstring>
using namespace std;

void readDir(int fd);
//void readDir(int fd, DIR *d_fd, struct dirent *entry);
int main(int argc, char *argv[]){
	string path;
	clock_t start = clock();
	 double sec = 0;
	if(argc<=1){
		cout <<"Enter path to catalog: ";
		cin.clear();
		getline(cin, path);
	}
	else if(argc > 1){
		path = "";
		for(int i = 1;i<argc;i++){
			path += argv[i]; //на случай если путь введен через пробел
		}
	}
	//path = argv[1];
	cout <<path.data()<<endl;
	int fd;
	if((fd = open(path.data(),O_RDONLY))==-1){
		cout <<"Please pass the full directory path as a parameter (no spaces)"<<endl;
		perror("Open file");
		exit(1);
	}
	struct stat buf;
	int res;
	if((res = fstat(fd, &buf))==-1){
		cout <<"Please pass the full directory path as a parameter (no spaces)"<<endl;
		perror("Status error");
		exit(1);
	}
	if(S_ISDIR(buf.st_mode)){
		pid_t pid = fork();
		if(pid==0){
			readDir(fd);
		}
	}
	else{
		cout <<"Please pass the full directory path as a parameter (no spaces)"<<endl;
	}
	clock_t end = clock();
	sec += (double)(end + start) / CLOCKS_PER_SEC;
	cout <<"Время работы программы: "<<sec<<endl;

	close(fd);
	return 0;

}


void readDir(int fd){
	DIR * d_fd;
	if((d_fd = fdopendir(fd))==NULL){
		perror("Error open dir");
		return;
	}
	struct dirent *entry;
	int temp_fd;
	DIR* temp_dfd;
	pid_t pid = 1;
	int child_count = 0;
        while((entry=readdir(d_fd))!=NULL && pid > 0){
		if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name,"..")!=0){
			switch(entry->d_type){
				case DT_DIR:
					temp_fd = openat(fd,entry->d_name,O_RDONLY);
					if((temp_dfd = fdopendir(temp_fd))==NULL){
						perror("Error open dir");
						break;
					}	
					pid = fork();
					if(pid == 0){
						readDir(temp_fd);
						pid = -1;
						break;
					}
					else{
					child_count++;
					}
					break;
				case DT_LNK:
					cout <<entry->d_name<<endl;
					break;
				case DT_REG:
					cout <<entry->d_name<<endl;
	
					break;
			}
		}	
        
	}
	if(pid>0){
		for(int i = 0;i<child_count;i++){
			wait(NULL);
		}
	}
}

