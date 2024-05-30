#include <iostream>
#include <time.h>
#include <unistd.h>
using namespace std;

struct Gamer {
	int id;
	int gamers_count;
	double money;
	bool status; // 1 - buy, 0 - sale	

	void printGamer(){
		cout << "id: "<<id<<" money: "<<money<<" status: "<<status;
	}
};

int _rand(int start, int end);

void _gamer(Gamer gamer);
//print ставка {от кого, кому, сколько}
//print результат обмена {ставка}
//print статус игрока?? бюджет игрока??

//проверка на проигрыш

int main(){
	int N;
	cout << "Введите количество игроков: ";
	cin>>N;
	pid_t pid = 1;
	for(int i = 0;i<N && pid > 0;i++){
		if(pid>0){
			cout <<"Parent make "<< i+1<<" child"<<endl;
			pid = fork();
			if(pid == 0){
				Gamer g;
				g.id = i;
				g.gamers_count = N;
				g.money = _rand (1000, 10000);
				if(i%2==0){g.status = true;}
				else{ g.status = false; }
				_gamer(g);
			}
		}
	}

	return 0;
}

//логика детей
//передать дескрипторы, записать в структуру????
void _gamer(Gamer gamer){
	gamer.printGamer();
	cout <<endl;
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
//получит массив дескрипторов, epoll - отслеживание событий
//
