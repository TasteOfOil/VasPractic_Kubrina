#include <iostream>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

using namespace std;

#define MIN_BET 25 
#define MAX_BET 50
#define START_BET 60 
#define END_BET 70
struct Gamer {
	int id;
	int gamers_count;
	double money;
	bool status; // 1 - buy, 0 - sale	
	
	//вывод игрока
	void printGamer(){
		cout << "id: "<<id<<" money: "<<money<<" status: "<<status;
	}
	//проверка на проигрыш
	bool checkBankrupt(){
		return money<=0?false:true; // 1-еще играет 0 - проиграл
	}
};

int _rand(int start, int end);

void _gamer(Gamer gamer, int *pipefd);
void cur_market(int N,int **pipefd);
int findIndex(int **pipefd, int N, struct epoll_event *events);
void printBet(double buf[3]);


int main(){
	int N;
	cout << "Введите количество игроков: ";
	cin>>N;
	pid_t pid = 1;
	int res = 0;
	pid_t main_pid = getpid();
	int **pipefd = new int*[N];
	for(int i = 0;i<N;i++){
		pipefd[i] = new int[2];
	}
	double start_money = _rand(START_BET,END_BET)/100.0;
	cout <<"Стартовый капитал: "<<start_money<<endl;
	for(int i = 0;i<N && pid > 0;i++){
		if(pid>0){
			//cout <<"Parent make "<< i+1<<" child"<<endl;
			if((res = pipe(pipefd[i]))==-1 ){
				perror("pipe");
				return -1;
			}
			fcntl( pipefd[i][0], F_SETFD, O_NONBLOCK);
			pid = fork();
			if(pid == 0){
				Gamer g;
				g.id = i;
				g.gamers_count = N;
				g.money = start_money;
				if(i%2==0){g.status = true;}
				else{ g.status = false; }
				_gamer(g,pipefd[i]);
			}
		}
	}
	if(getpid() == main_pid){
		cur_market(N, pipefd);
		for(int i = 0;i<N;i++){
			close(pipefd[i][0]);
			close(pipefd[i][1]);
		}
		for(int i = 0;i<N;i++){
			wait(NULL);
		}
	}
	return 0;
}

//логика детей
void _gamer(Gamer gamer, int *pipefd){
	
	double buf_sale[3];
	double buf_res[3];
	int res;
	while(gamer.checkBankrupt()){
		if(!gamer.status && gamer.checkBankrupt()){//продаем
			buf_sale[0] = gamer.id;
			res = gamer.id;
			while(res == gamer.id){
				res = _rand(1, gamer.gamers_count) - 1 ;
			}
			buf_sale[1] = res;
			buf_sale[2] = _rand(MIN_BET, MAX_BET)/100.0;
			//cout <<endl <<"- - - * Раунд " << " * - - -"<<endl;
			
			//cout << "Игрок "<<buf_sale[0]<<" продает Игроку "<<buf_sale[1]<<" ставка "<< buf_sale[1]<<endl;
			res = write(pipefd[1], &buf_sale, sizeof(buf_sale));
			//fcntl( pipefd[0], F_SETFD, O_NONBLOCK);
			sleep(2);
			//while((res = read(pipefd[0], &buf_sale, sizeof(buf_sale))) <= 0){
			//cout <<"do read "<<buf_sale[0]<<" "<<buf_sale[1]<<" "<<buf_sale[2]<<endl;
			cout <<endl<<"* * * Продажа * * *"<<endl;
			printBet(buf_sale);
			if((res = read(pipefd[0], &buf_res, sizeof(buf_res))) <= 0){
				cout <<"0 byte read"<<endl;
			}
			//}
			else {
			//cout <<"res "<<res<<endl;
			//cout <<"after read"<<buf_res[0]<<" "<<buf_res[1]<<" "<<buf_res[2]<<endl;
				//printBet(buf_sale);
				//cout <<endl;
				gamer.money+=buf_res[2];
				
				cout<<endl <<"Игрок "<<gamer.id<<": "<<gamer.money<<" валюты"<<gamer.status<<endl;
				if(!gamer.checkBankrupt()){
					cout <<"Банкрот"<<endl;
				}
				else {
					cout <<"Еще в игре"<<endl;
					//gamer.status = !gamer.status;
				}	
			}
			
		}
		else if(gamer.status){//покупаем
			//fcntl( pipefd[0], F_SETFD, O_NONBLOCK);
                	sleep(3);
			while (read(pipefd[0], &buf_sale, sizeof(buf_sale))==-1){
                	        cout <<"Игрок "<<gamer.id<< " wait..."<<endl;
                	}
			//cout <<" GAMER "<<gamer.id<<endl;
			
			cout <<endl <<"- - - * Раунд " << " * - - -"<<endl;
			//cout << "Игрок "<<buf_sale[0]<<" продает Игроку "<<gamer.id<<" ставка "<< buf_sale[2]<<endl;
			
			double bet = _rand(MIN_BET,MAX_BET)/100.0; 
			double buf_res = bet - buf_sale[2];
			if(buf_res < 0){//забирает себе возвр отриц
				gamer.money-= buf_sale[2];

			}
			else if(buf_res > 0){//отнимает себе возвр полож
				gamer.money -= buf_sale[2];			
			}
			cout<<endl <<"Игрок "<<gamer.id<<": "<<gamer.money<<" валюты "<<gamer.status<<endl;
                                if(!gamer.checkBankrupt()){
                                        cout <<"Банкрот"<<endl;
                                }
                                else {
                                        cout <<"Еще в игре"<<endl;
                                        //gamer.status = !gamer.status;
                                }
			int temp = buf_sale[1];
			buf_sale[1] = buf_sale[0];
			buf_sale[0] = temp;	
			cout <<endl<<"* * * Покупка * * *"<<endl;
			printBet(buf_sale);
			res = write(pipefd[1], &buf_sale, sizeof(buf_sale));
		
		}
		sleep(2);
		gamer.status = !gamer.status;
	}
}

//рандомим ставку
int _rand(int start, int end) {
        srand(time(0));
	return rand() % (end - start + 1) + start;
}

//логика родителя
void cur_market(int N,int **pipefd){
	static int round_num = 1;
	int res = 0;
	int index_come;
	int index_go;
	double buf_res[3];
	//экземпляр epoll
	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1) {
      	 	perror("epoll_create1");
      		return;
    	}
	struct epoll_event event[N];
	for(int i = 0;i<N;i++){
		event[i].events = EPOLLIN;
		event[i].data.fd = pipefd[i][0];
		epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pipefd[i][0], &event[i]);
	}
	
	int n = -2;
	while(n!=-1){
	/*struct epoll_event event[N];
        for(int i = 0;i<N;i++){ 
                event[i].events = EPOLLIN;
                event[i].data.fd = pipefd[i][0];
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pipefd[i][0], &event[i]);
        }
	*/
	struct epoll_event events[N];
		int n = epoll_wait(epoll_fd, events, N, 5000); // -1 означает бесконечное ожидание
		if (n == -1) {//если сюда, то чета стопнулос
	              	perror("epoll_wait");
			cout <<"AAAAAAAAAA parent END"<<endl;
                        return;
                }
                else{
			for(int i = 0;i<n;i++){
				//cout <<"- - - * Раунд " <<round_num <<" n "<< n<< " * - - -"<<endl;
                        	//round_num++;
				index_come = findIndex(pipefd, N, events);
				//cout<<"index_come "<<index_come<<endl;
			//sleep(1);
			//fcntl( pipefd[index_come][0], F_SETFD, O_NONBLOCK);
				//cout <<"parent read"<<endl;
				if ((res = read(pipefd[index_come][0], &buf_res, sizeof(buf_res))) == -1 ){
					perror("read_market");
				}
				//cout <<"parent end read"<<endl;
				printBet(buf_res);
				//sleep(1);
				index_go = buf_res[1];
			//fcntl( pipefd[int(index_go)][1], F_SETFD, O_NONBLOCK);
				//cout <<"parent write "<<index_go<<endl;
				if((res = write(pipefd[(int)index_go][1], &buf_res, sizeof(buf_res))) == -1){
					perror("write_market");
				}
				//cout <<endl<<"parent end write"<<endl;
				}
                }
	}
}


//ищет индекс в массиве pipefd по дескриптору
int findIndex(int **pipefd, int N, struct epoll_event *events){
	for(int j = 0;j<N;j++){
		if(pipefd[j][0]==events[0].data.fd){
			cout <<"j "<<j<<endl;
			return j;
		}
	}
	
	return -1;
}

//печатает ставку в виде от кого кому и сколько
void printBet(double buf[3]){
	cout <<"От Игрока "<<buf[0]<<" Игроку "<<buf[1]<<" Ставка: "<<buf[2]; 
}
