#include <bits/stdc++.h>
using namespace std;

int iteration, usr_iter;

int min_index(float *fin_row, int length){
	int index=0;
	for (int i = 1; i < length; i++){
		if (fin_row[i] < fin_row[index]) index = i;
	}
	if (fin_row[index] >= 0) return -1;
	else return index;
}

int min_col_index(float *temp, int length){
	int index=0;
	for(int i = 1; i < length; i++){
		if(temp[i] < temp[index]) index = i;
	}
	return index;
}

void print(int n, int m, float **A){
	for (int i = 0; i < m; i++){
		for (int j = 0; j < (n); j++)
			cout << setw(10) << setprecision(5) << A[i][j];
		cout << endl;
	}
}

void simplex(float **tableau, int n, int m){
	float fin_row[n + 1], *temp;
	int i, j, pc = 0, pr, count;
	bool stop=true, unbounded = false;

	iteration++;

	for (i = 0; i < m; i++){
		if (tableau[i][n] < 0) stop = false;
	}

	if (stop == true){
		cout << endl << "Optimal Solution is " << (-1) * tableau[m][n] << endl;
		
		for (i = 0; i < m; i++){
			if (tableau[i][n + 1]<50) cout << "x" << tableau[i][n + 1] << " is " << tableau[i][n] << endl;
		}

		for (i = 0; i < n; i++){
			if (tableau[m + 1][i]<50) cout << "x" << tableau[m + 1][i] << " is " << 0 << endl;
		}
		return;
	}

	float temp1[m];
	for (i = 0; i < m; i++) temp1[i] = tableau[i][n];

	pr = min_index(temp1, m);

	count = 0;
	for (i = 0; i < n; i++){
		if (tableau[pr][i] >= 0) count++;
	}
	if(count == n){
		cout << "Solution is Unbounded" << endl;
		return;
	}


	temp = new float [m];
	for (i = 0; i < n; i++){
		if (tableau[pr][i] < 0) temp[i] = abs(tableau[m][i] / tableau[pr][i]);
		else temp[i]=1000000;
	}
	pc = min_col_index(temp, n);

	float swap;
	swap = tableau[m + 1][pc];
	tableau[m + 1][pc] = tableau[pr][n + 1];
	tableau[pr][n + 1]=swap;


	float pivot = tableau[pr][pc];
	for (i = 0; i <= m; i++){
		for(j = 0; j <= n; j++){
			if (i!= pr && j!= pc){
				tableau[i][j] = (tableau[i][j] * tableau[pr][pc] - tableau[pr][j] * tableau[i][pc]) / tableau[pr][pc];
			}
		}
	}

	for (i = 0; i <= n; i++) tableau[pr][i] = tableau[pr][i]/pivot;

	for (i = 0; i <= m; i++) tableau[i][pc]=-tableau[i][pc]/pivot;
	
	tableau[pr][pc] = 1 / pivot;

	if (iteration == usr_iter) print(n + 1, m + 1, tableau);	

	simplex(tableau, n, m);

}


int main(){
	int n, m, i, j;
	float **A, *b, **tableau, *z;

	cout << "Enter No of Equations : ";
	cin >> m;

	cout << "Enter No of Unknwons : ";
	cin >> n;

    A = new float* [m]; 
    tableau = new float* [m + 2];

    for (i = 0; i < m; i++){
        A[i] = new float [n];
        tableau[i] = new float [n + 2];
    }
    tableau[m]= new float [n + 2];
    tableau[m + 1]= new float [n + 2];


	cout << "Enter the matrix A : \n";
	for (i = 0; i < m; i++){
		for (j = 0; j < n; j++){
			cin >> A[i][j];
			tableau[i][j] = A[i][j];
		}
	}

	cout << "Enter the column vector 'b' : \n";
    b = new float [m];
	for (i = 0; i < m; i++){
		cin >> b[i];
		tableau[i][n] = b[i];
	}

	for (i = 0; i < n; i++) tableau[m + 1][i] = i + 1;

	for (i = 0; i < m; i++) tableau[i][n + 1] = 51 + i;

	int max_min = 2;

	cout << "Enter the objective function z :\n";
	z = new float [n];
	for (i = 0; i < n; i++){ 
		cin >> z[i];
		tableau[m][i] = z[i];
	}
	z = new float [n];
	cin >> tableau[m][n];


	print(n + 2, m + 2, tableau);

	float tempA[m + 2][n + 2];
	for (i = 0; i < m + 2; i++) for (j = 0; j < n + 2; j++) tempA[i][j] = tableau[i][j];

	
		
	print(n + 1, m + 1, tableau);

	simplex(tableau, n, m);
	for (i = 0; i < m + 2; i++) for (j = 0 ; j < n + 2; j++) tableau[i][j] = tempA[i][j];

	
	return 0;


}