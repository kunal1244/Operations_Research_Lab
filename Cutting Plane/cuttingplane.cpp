#include <bits/stdc++.h>
using namespace std;

int min_index(float *fin_row, int length){
	int index = 0;
	for (int i = 1; i < length; i++){
		if (fin_row[i] < fin_row[index]) index = i;
	}
	if (fin_row[index] >= 0) return -1;
	else return index;
}

int min_col_index(float *temp, int length){
	int index = 0;
	for (int i = 1; i < length; i++) {
		if (temp[i] < temp[index]) index = i;
	}
	return index;
}

int max_index(float *temp, int length){
	int index = 0;
	for (int i = 1; i < length; i++){
		if (temp[i] > temp[index]) index = i;
	}
	return index;
}

void print(int n, int m, float **A){
	for (int i = 0; i < m; i++){
		for (int j = 0; j < n; j++)
			cout << setw(10) << setprecision(3) << A[i][j];
		cout<<endl;
	}
}

void simplex(float **tableau, int n, int m, int max_min){
	float fin_row[n + 1], *temp;
	int pc = 0, pr, count;
	bool stop = true, unbounded = false;

	for (int i = 0; i < n; i++){
		if (tableau[m][i] <= 0) stop = false;
	}

	if (stop == true){
		if (max_min == 1) cout << endl << "Optimal Solution is " << tableau[m][n] <<endl;
		if (max_min == 2) cout << endl << "Optimal Solution is " << (-1) * tableau[m][n] << endl;
		for (int i = 0; i < m; i++){
			if (tableau[i][n + 1] < 50 && tableau[i][n + 1] > 0) cout << "x" << tableau[i][n + 1] << " is " << tableau[i][n] << endl;
		}

		for (int i = 0; i < n; i++){
			if (tableau[m + 2][i] < 50 && tableau[m + 2][i] > 0) cout << "x" << tableau[m + 2][i] << " is " << 0 << endl;
		}
		return;
	}

	pc = min_index(tableau[m], n);

	if (!tableau[m][pc]){
		if (max_min == 1) cout << endl << "Alternate Solution Exists. Optimal Solution is " << tableau[m][n] << endl;
		if (max_min == 2) cout << endl << "Alternate Solution Exists. Optimal Solution is " << (-1) * tableau[m][n] << endl;
		for (int i = 0; i < m; i++){
			if (tableau[i][n + 1] < 50 && tableau[i][n + 1] > 0) cout << "x" << tableau[i][n + 1] << " is " << tableau[i][n] << endl;
		}

		for (int i = 0; i < n; i++){
			if (tableau[m + 2][i] < 50 && tableau[m + 2][i] > 0) cout << "x" << tableau[m + 2][i] << " is " << 0 << endl;
		}
		return;
	}


	count = 0;
	for (int i = 0; i < m; i++){
		if (tableau[i][pc] <= 0) count++;
	}
	if (count == m){
		cout << "Solution is Unbounded" << endl;
		return;
	}

	temp = new float [m];
	for (int i = 0; i < m; i++){
		if (tableau[i][pc] > 0) temp[i] = tableau[i][n] / tableau[i][pc];
		else temp[i] = 1000000;
	}
	pr = min_col_index(temp, m);
	float swap;
	swap = tableau[m + 2][pc];
	tableau[m + 2][pc] = tableau[pr][n + 1];
	tableau[pr][n + 1] = swap;

	float pivot = tableau[pr][pc];
	for (int i = 0; i <= m + 1; i++){
		for(int j = 0; j <= n; j++){
			if (i != pr && j != pc){
				tableau[i][j] = (tableau[i][j] * tableau[pr][pc] - tableau[pr][j] * tableau[i][pc]) / tableau[pr][pc];
			}
		}
	}

	for (int i = 0; i <= n; i++) tableau[pr][i] = tableau[pr][i] / pivot;

	for (int i = 0; i <= m + 1; i++) tableau[i][pc] = -tableau[i][pc] / pivot;
	
    tableau[pr][pc] = 1 / pivot;
	
	simplex(tableau, n, m, max_min);
}

void dualsimplex(float **tableau, int n, int m, int max_min, int choice){
	float fin_row[n + 1], *temp;
	int pc = 0, pr, count;
	bool stop = true, unbounded = false;

	for (int i = 0; i < m; i++){
		if (tableau[i][n] < 0) stop = false;
	}

	if (stop == true){
		if (choice == 4){
			if (max_min==1) cout << endl << "Optimal Solution is " << tableau[m][n] << endl;
			if (max_min==2) cout << endl << "Optimal Solution is " << (-1) * tableau[m][n] << endl;
			
			for (int i = 0; i < m; i++){
				if (tableau[i][n + 1] < 50 && tableau[i][n + 1] > 0) cout << "x" << tableau[i][n + 1] << " is " << tableau[i][n] << endl;
			}

			for (int i = 0; i < n; i++){
				if (tableau[m + 1][i] < 50 && tableau[m + 1][i] > 0) cout << "x" << tableau[m + 1][i] << " is " << 0 << endl;
			}
		}
		return;
	}

	float temp1[m];
	for (int i = 0; i < m; i++) temp1[i] = tableau[i][n];

	pr = min_index(temp1, m);

	count = 0;
	for (int i = 0; i < n; i++){
		if (tableau[pr][i] >= 0) count++;
	}
	if (count == n){
		if (choice == 4) cout << "Solution is Unbounded" << endl;
		return;
	}

	temp = new float [m];
	for (int i = 0; i < n; i++){
		if (tableau[pr][i] < 0) temp[i] = abs(tableau[m][i] / tableau[pr][i]);
		else temp[i] = 1000000;
	}
	pc = min_col_index(temp, n);

	float swap;
	swap = tableau[m + 1][pc];
	tableau[m + 1][pc] = tableau[pr][n + 1];
	tableau[pr][n + 1] = swap;

	float pivot = tableau[pr][pc];
	for (int i = 0; i <= m; i++){
		for(int j = 0; j <= n; j++){
			if (i != pr && j != pc){
				tableau[i][j] = (tableau[i][j] * tableau[pr][pc] - tableau[pr][j] * tableau[i][pc]) / tableau[pr][pc];
			}
		}
	}

	for (int i = 0; i <= n; i++) tableau[pr][i] = tableau[pr][i] / pivot;

	for (int i = 0; i <= m; i++) tableau[i][pc] = -tableau[i][pc] / pivot;
	tableau[pr][pc] = 1 / pivot;
	
	dualsimplex(tableau, n, m, max_min, choice);
}


int main(){
	int n, m, geq, leq, eq, M = 1000, max_min;
	float **A, *b, **tableau, *z, temp;

	cout << "Enter no of equations : ";
	cin >> m;
	cout << "Enter no of variables : ";
	cin >> n;

    cout << "Enter the number of >=, =, <= contraints : " << endl;
    cin >> geq >> eq >> leq;

    A = new float* [100]; 
    tableau = new float* [100]; 

    for (int i = 0; i < 100; i++){
        A[i] = new float [n];
        tableau[i] = new float [n + geq + 2];
    }

	cout << "Enter the coefficients of variables in the order '>=', '=' and '<=') : " << endl;
	for (int i = 0; i < m; i++){
		for (int j = 0; j < n; j++){
			cin >> A[i][j];
			tableau[i][j] = A[i][j];
		}
	}

	for (int i = 0; i < geq; i++){
		for (int j = 0; j < m; j++){
			if (i == j) tableau[j][n + i]=-1;
			else tableau[j][n+i]=0;
		}
	}

	cout << "Enter the column vector 'b' : " << endl; 
    b = new float [m];
	for (int i = 0; i < m; i++){
		cin >> b[i];
		tableau[i][n + geq] = b[i];
	}

	for (int i = 0; i< geq + eq; i++){
		for (int j = 0; j < n + geq + 1; j++)
			tableau[m][j] -= tableau[i][j];
	}

	for (int i = 0; i < n + geq; i++) tableau[m + 2][i] = i + 1;

	for (int i = 0; i < m; i++) tableau[i][n + geq + 1] = 51 + i;
    
    max_min = 1;

	cout << "Enter the objective function z :" << endl;
	z = new float [n];
	for (int i = 0; i < n; i++){
		cin >> z[i];
		tableau[m+1][i]=-z[i];
	}

	print(n + geq + 1, m + 1, tableau);
	cout << "Phase I " << endl;
	simplex(tableau, n + geq, m, max_min);

	for (int i = 0; i <= n + geq; i++) tableau[m][i] = tableau[m + 1][i];

	for (int i = 0; i < n + geq; i++){
		if (tableau[m + 2][i] >= 50){
			for (int j = 0; j < m + 2; j++) tableau[j][i] = 0;
		}
	}

	simplex(tableau, n + geq, m, max_min);

	float* frac;
	int check = 1;

	int k = 0;

	while(true){
		frac = new float[m];
		for (int i = 0; i < m; i++){
			frac[i] = tableau[i][n + geq] - (int)(tableau[i][n + geq]);
			if (frac[i] > 0.999 || frac[i] < 0.0001) frac[i] = 0;

			if (frac[i]) check=2;
		}

		if (check == 1) break;

		check = 1;

		int sel_ind = max_index(frac, m);

		for (int i = 0; i <= (n + geq); i++){
			temp = tableau[m][i];
			if(tableau[sel_ind][i] < 0){
				tableau[m][i] = -1 * (tableau[sel_ind][i] - (int)((tableau[sel_ind][i]) - 1));
				tableau[m + 1][i] = temp;
			}
			else{
				tableau[m][i] = -1 * (tableau[sel_ind][i] - (int)(tableau[sel_ind][i]));
				tableau[m + 1][i] = temp;
			}
		}

		print(n + geq + 2, m + 3, tableau);
		m++;

		dualsimplex(tableau, n + geq, m, max_min, 4);
		k++;

	}
	return 0;
}