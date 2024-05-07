#include <iostream>
using namespace std;

void printMatrix(int size[2], double **matrix);
void setMatrix(int size[2], double **matrix);
void sumMatrix(int size_1[2], int size_2[2], double **matrix1, double **matrix2);
void multMatrix(int size_1[2], double **matrix1, int size_2[2], double **matrix2);


int main(){
	int size_1[2];
	int size_2[2]; 
	cout << "Enter size for matrix1 (nxm): ";
	for(int i = 0;i<2;i++){
		cin>>size_1[i];
	}	
	
	cout << "Enter size for matrix2 (nxm): ";
	for(int i = 0;i<2;i++){
		cin>>size_2[i];
	}

	double **matrix1 = new double*[size_1[0]];
	double **matrix2 = new double*[size_2[0]];

	for(int i = 0;i<size_1[0];i++){
		matrix1[i] = new double[size_1[1]];
	}
	for(int i = 0;i<size_2[0];i++){
		matrix2[i] = new double[size_2[1]];
	}

	

	int sym = -1;
	while(sym!=0){
		cout <<"1. Set matrix1 ("<<size_1[0]<<"x"<<size_1[1]<<")"<<endl;
		cout <<"2. Set matrix2 ("<<size_2[0]<<"x"<<size_2[1]<<")"<<endl;
		cout <<"3. Sum matrix"<<endl;
		cout <<"4. Multiply matrix"<<endl;
		cout <<"0. Exit"<<endl;
		cout <<"Choose operation: ";
		cin>> sym;
		
		switch(sym){
			case 1:
				setMatrix(size_1, matrix1);
				break;
			case 2:
				setMatrix(size_2, matrix2);
				break;
			case 3:
				sumMatrix(size_1, size_2, matrix1, matrix2);
				break;
			case 4:
				multMatrix(size_1, matrix1, size_2, matrix2);
				break;
			case 0:
				cout<<"Exit"<<endl;
				break;
		}

	}
	
	for(int i = 0;i<size_1[0];i++){
		delete[] matrix1[i];
	}
	for(int i = 0;i<size_2[0];i++){
		delete[] matrix2[i];
	}
	
	delete[] matrix1;
	delete[] matrix2;

	return 0;
	
}

void setMatrix(int size[2], double **matrix){
	for(int i = 0;i<size[0];i++){
		for(int j = 0;j<size[1];j++){
			cin>> matrix[i][j]; 
		}
	}	
}

void printMatrix(int size[2], double **matrix){
	for(int i = 0;i<size[0];i++){
		for(int j = 0;j<size[1];j++){
			cout <<matrix[i][j]<<" ";
		}
		cout <<endl;
	}

}


void sumMatrix(int size_1[2],int size_2[2], double **matrix1, double **matrix2){
	if(size_1[0]!=size_2[0] || size_1[1]!=size_2[1]){
		cout <<"Error: matrix_size_1 != matrix_size_2"<<endl;
		return;
	}
	int size_temp[2] = {size_1[0], size_1[1]};
	double **temp = new double*[size_temp[0]];
	for(int i = 0; i<size_temp[0];i++){
		temp[i] = new double[size_1[1]];
	}

	for(int i = 0;i<size_1[0];i++){
		for(int j = 0;j<size_1[1];j++){
			temp[i][j] = matrix1[i][j] + matrix2[i][j];
		}
	}
	printMatrix(size_temp, temp);

	for(int i = 0;i<size_temp[0];i++){
		delete[] temp[i];
	}	
	delete[] temp;
}


void multMatrix(int size_1[2], double **matrix1, int size_2[2], double **matrix2){
	if(size_1[1]!=size_2[0]){
		cout <<"Error: incorrect matrix size"<<endl;
		return;
	}
	double **temp = new double*[size_1[0]];
       	for(int i = 0;i<size_1[0];i++){
		temp[i] = new double[size_2[1]];
	}
	int res = 0;
	for(int i = 0;i<size_1[0];i++){
		for(int j = 0;j<size_2[1];j++){
			for(int k = 0;k<size_1[1];k++){
				res += matrix1[i][k] * matrix2[k][j];
			}
			temp[i][j] = res;
			res = 0;
		}
	}
	int size_temp[2] = {size_1[0], size_2[1]};
	printMatrix(size_temp, temp);
	
	for(int i = 0; i < size_temp[0];i++){
		delete[] temp[i];
	}
	delete[] temp;

}
