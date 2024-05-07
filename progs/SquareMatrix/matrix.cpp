#include <iostream>
using namespace std;

const int _size = 3;

void printMatrix(double matrix[][_size]);
void sumMatrix(double matrix1[][_size], double matrix2[][_size]);
void multMatrix(double matrix[][_size], double matrix2[][_size]);

int main(){
	//const int size = 3;
	double matrix1[_size][_size];
	double matrix2[_size][_size];
	
	for(int i = 0;i<_size;i++){
		for(int j = 0;j<_size; j++){
			matrix1[i][j] = i+j;
			matrix2[j][i] = i-j;
		}
	}
	cout <<"First matrix:"<<endl;
	printMatrix(matrix1);
	cout <<endl<<"--------------"<<endl;
	cout <<"Second matrix: "<<endl;
	printMatrix(matrix2);	
	cout<<endl<<"---------------" <<endl;
	cout <<"Sum matrix: "<<endl;
	sumMatrix(matrix1,matrix2);
	cout <<endl<<"-----------"<<endl;
	cout <<"Multiply matrix: "<<endl;
	multMatrix(matrix1, matrix2);
	
	return 0;
}


void printMatrix(double matrix[][_size]){
	for(int i = 0;i<_size;i++){
		for(int j = 0;j<_size;j++){
			cout << matrix[i][j]<<" ";
		}
		cout << endl;
	}
}


void sumMatrix(double matrix1[][_size], double matrix2[][_size]){
	double temp[_size][_size];
	for(int i = 0;i<_size;i++){
		for(int j = 0;j<_size;j++){
			temp[i][j] = matrix1[i][j]+matrix2[i][j];
		}
	}
	printMatrix(temp);
}


void multMatrix(double matrix1[][_size], double matrix2[][_size]){
	double temp[_size][_size];
	double res = 0;
	for(int i = 0; i<_size;i++){
		for(int j = 0; j<_size;j++){
			for(int k = 0; k<_size;k++){
				res += matrix1[i][k] * matrix2[k][j];
			}
			temp[i][j] = res;
			res = 0;
		}
	}
	printMatrix(temp);
}
