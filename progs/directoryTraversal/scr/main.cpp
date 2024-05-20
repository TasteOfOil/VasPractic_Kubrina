#include <iostream>
#include <ctime>
#include <signal.h>
#include <cstring>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

void readDir(int fd, int count, bool flag);

int main(int argc, char *argv[]){
	if (argc==1){
		return 0;
	}
	bool flag = true;
	string path;
	if(argc == 3){
		flag = stoi(argv[1]);
		path = argv[2];	
	}
	else{
		path = argv[1];
	}
	int fd ;
	if((fd = open(path.data(),O_RDONLY)) == -1){
		perror("Error open file");
		exit(1);
	}
      	struct stat buf;
	int res;
	if((res = fstat(fd, &buf))==-1){
		perror("Error");
		exit(1);
	}
	unsigned int start_time =  clock();
	if(S_ISDIR(buf.st_mode)){
		try{
                  	readDir(fd,0, flag);
                }
                catch(const char* error_mes){
                        cout <<error_mes<<endl;
                        exit(1);
		}
		unsigned int end_time = clock();
    		unsigned int search_time = end_time - start_time;
		cout <<"Время работы программы: "<<search_time<<endl;
	}
	else {
		cout <<"This is not a catalog!"<<endl;
	}
	
	close(fd);
	return 0;	
}



void readDir(int fd, int count, bool flag){
	pid_t pid;
	int status;

	if(count > 20){
		throw "Error::looping";
	}	
	++count;
	struct stat buf;
	int res;
	DIR * d_fd;
	if((d_fd = fdopendir(fd)) == NULL){
		perror("Error open dir");
		exit(1);
	}
	struct dirent *entry;
	
	//cout <<"PID: "<<getpid()<<endl;
	while ((entry = readdir(d_fd)) != NULL){
		if(entry->d_name[0]=='.'){
			continue;
		}	
		switch(entry->d_type){
			case DT_DIR:
				if(flag && count>1){
					pid = fork();
				}
				else{
					pid = -1;
				}
				if(pid == 0||pid==-1){//уходим на рекурсию в ребенке
					readDir(openat(fd,entry->d_name,O_RDONLY), count, flag);
					if(pid == 0)_exit(status);
				}
				break;
			case DT_LNK:
				res = openat(fd,entry->d_name, O_RDONLY);
				if(fstat(res,&buf) == -1){
					perror("Error:");
				}
				if(S_ISREG(buf.st_mode)){
					cout <<count<<" "<<entry->d_name<<endl;
				}
				else if(S_ISLNK(buf.st_mode)||S_ISDIR(buf.st_mode)){
					if(flag&& count >1){
						pid = fork();
					}
					else{
						pid = -1;	
					}
					if(pid == 0 || pid == -1){
						readDir(res,count, flag);
						if(pid==0)_exit(status);
					}
				}
				break;
			case DT_REG:
				cout <<entry->d_name<<endl;
				break;
		}
	}
	if(pid!=0 && flag && pid!=-1){
		//cout <<"PID "<<pid <<"end"<<endl;
		waitpid(pid, &status, 0);
		
		
	}	
	closedir(d_fd);
}
