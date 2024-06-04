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
#define START_BET 50 
#define END_BET 100
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
int findIndex(int **pipefd, int N, int n, struct epoll_event *events);
void printBet(double buf[3]);
//print ставка {от кого, кому, сколько}
//print результат обмена {ставка}
//print статус игрока?? бюджет игрока??


int main(){
	int N;
	cout << "Введите количество игроков: ";
	cin>>N;
	pid_t pid = 1;
	pid_t main_pid = getpid();
	int **pipefd = new int*[N];
	for(int i = 0;i<N;i++){
		pipefd[i] = new int[2];
	}
	double start_money = _rand(START_BET,END_BET)/1.0;
	cout <<"Стартовый капитал: "<<start_money<<endl;
	for(int i = 0;i<N && pid > 0;i++){
		if(pid>0){
			//cout <<"Parent make "<< i+1<<" child"<<endl;
			if(main_pid == getpid()) {pipe(pipefd[i]);};
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
//передать дескрипторы, записать в структуру????
void _gamer(Gamer gamer, int *pipefd){
	//gamer.printGamer();
	//cout <<endl;
	
	double buf_sale[3];
	//double buf_pur[2];
	//double buf_res;
	int res;
	while(gamer.checkBankrupt()){
		//gamer.printGamer();
		if(!gamer.status && gamer.checkBankrupt()){//продаем
			//gamer.printGamer();
			buf_sale[0] = gamer.id;
			res = gamer.id;
			while(res == gamer.id){
				res = _rand(1, gamer.gamers_count);
				//cout <<res<<endl;
			}
			buf_sale[1] = res;
			buf_sale[2] = _rand(MIN_BET, MAX_BET)/100.0;
			//cout << "Игрок "<<buf_sale[0]<<" продает Игроку "<<buf_sale[1]<<" ставка "<< buf_sale[1]<<endl;
			res = write(pipefd[1], buf_sale, sizeof(buf_sale));
			fcntl( pipefd[0], F_SETFD, O_NONBLOCK);
			if((res = read(pipefd[0], &buf_sale, sizeof(buf_sale))) == -1){
				cout <<"0 byte read"<<endl;
			}
			else {
				printBet(buf_sale);
				cout <<endl;
				gamer.money+=buf_sale[2];
				
				cout<<endl <<"Игрок "<<gamer.id<<": "<<gamer.money<<" валюты"<<endl;
				if(!gamer.checkBankrupt()){
					cout <<"Банкрот"<<endl;
				}
				else {
					cout <<"Еще в игре"<<endl;
					gamer.status = !gamer.status;
				}	
			}
			
		}
		else{//покупаем
			fcntl( pipefd[0], F_SETFD, O_NONBLOCK);
                	while (read(pipefd[0], &buf_sale, sizeof(buf_sale))){
                	        cout <<"Игрок "<<gamer.id<< " wait..."<<endl;
                	}
			
			cout << "Игрок "<<buf_sale[0]<<" продает Игроку "<<gamer.id<<" ставка "<< buf_sale[1]<<endl;
			double bet = _rand(MIN_BET,MAX_BET)/100.0; 
			double buf_res = bet - buf_sale[1];
			if(buf_res < 0){//забирает себе возвр отриц
				gamer.money-= buf_sale[2];

			}
			else if(buf_res > 0){//отнимает себе возвр полож
				gamer.money -= buf_sale[2];			
			}	
			write(pipefd[1], &buf_sale, sizeof(buf_sale));
		}
		sleep(2);
		gamer.status = !gamer.status;
	}
	
	//if stat true 
	//продаем и ждем, получаем, проверяем банкротство, меняем статус
	//if stat false
	//ждем обрабатываем ставку и отправляем, меняем статус

}
int _rand(int start, int end) {
        srand(time(0));
	return rand() % (end - start + 1) + start;
}

//логика родителя
void cur_market(int N,int **pipefd){
	static int round_num = 1;
	cout <<"- - - * Раунд " <<round_num++ << " * - - -"<<endl;
	//int status;
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
		
	struct epoll_event events[N];
		
	//int n = 0;

			//cout <<"AAAAAAAAAA parent"<<endl;
		
		int n = epoll_wait(epoll_fd, events, N, 1000); // -1 означает бесконечное ожидание
                if (n == -1) {//если сюда, то чета стопнулос
	              	perror("epoll_wait");
			cout <<"AAAAAAAAAA parent END"<<endl;

                                //close(pipefd[0][0]);
                                //close(pipefd[0][1]);
                        return;
                }
                else{
			for(int i = 0;i<n;i++){
			//cout <<"Yes parent"<<endl;
			index_come = findIndex(pipefd, N, n, events);
                        //cout <<"***"<<index_come<<"***"<<endl;
				
			//sleep(1);
			//fcntl( pipefd[index_come][0], F_SETFD, O_NONBLOCK);
			//cout <<"AAAAAAAAAA parent"<<endl;
			read(pipefd[index_come][0], &buf_res, sizeof(buf_res));
			//cout <<"AAAAAAAAAA parent"<<endl;
			printBet(buf_res);
			index_go = buf_res[1];
			//fcntl( pipefd[int(index_go)][1], F_SETFD, O_NONBLOCK);
			write(pipefd[(int)index_go][1], &buf_res, sizeof(buf_res));
			}
                }

	}



//логика родителя
//получит массив дескрипторов, epoll - отслеживание событий
//
int findIndex(int **pipefd, int N, int n, struct epoll_event *events){
	for(int i = 0;i<n;i++){
		for(int j = 0;j<n;j++){
			if(pipefd[i][0]==events[j].data.fd){
				return i;
			}
		}
	}
	return -1;
}

void printBet(double buf[3]){
	cout <<"От Игрока "<<buf[0]<<" Игроку "<<buf[1]<<" Ставка: "<<buf[2]; 
}
