#include <iostream>
using namespace std;

int counterStatic();
void counterLocal();

int main(int argc, const char* argv[])
{
        int num;
	cout <<"Enter number: ";
	cin>> num;
	int *mas = new int[num];
	cout <<"Demonstration of using a static memory:"<<endl;
	for(int i = 0;i<num;i++){
		//counterStatic();
		mas[num-1-i] = counterStatic();
	}

	cout <<endl<<"Demonstration of using a local memory: "<<endl;
	for(int i = 0;i<num;i++){
		counterLocal();
	}
	cout << endl;
	
	cout<<"Demonstration of using dynamic memory:"<<endl;
	for(int i = 0;i<num;i++){
		cout <<mas[i]<<" ";
	}
	cout <<endl;

	return 0;
}


int counterStatic(){
	static int count = 0;
	cout <<count++;
	return count;
}

void counterLocal(){
	int count = 0;
	cout << count++;
}
