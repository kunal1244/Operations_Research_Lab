#include <bits/stdc++.h>

#define INF 99999
#define MAX 30
#define FOR(i, a, b, n) for(i = a; i < b; i = i + n)
#define FORe(i, a, b, n) for(i = a; i <= b; i = i + n)

using namespace std;

double payoff_matrix[MAX][MAX];
double temp[MAX][MAX];
int eqn = 0, var = 0;
bool infinite, unbounded;

void printTableau(){
    static int no = 0;
    cout << "Tableau no." << (++no) << endl;
        for(int i = 0 ; i <= eqn ; i++) {
            for(int j = 0 ; j <= var; j++) {
                cout << fixed << setw(7) << setprecision(3) << setfill(' ') << payoff_matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void copy(){
    for(int i = 0 ; i <=eqn ; i++){
        for(int j = 0 ; j <= var; j++) payoff_matrix[i][j] = temp[i][j]; 
    }
}

int getPivotRow(int pivotCol){
	int ind = -1;
	float currMin = 100000;
	for(int i = 0 ; i < eqn; i++){
	    if(payoff_matrix[i][pivotCol] <= 0) continue;
	    if((payoff_matrix[i][var]/payoff_matrix[i][pivotCol]) < currMin){
	        ind = i;
	        currMin = (payoff_matrix[i][var]/payoff_matrix[i][pivotCol]);
	    }
	}
	return ind;
}

int getMinFromLastRow(){
    int ind = -1;
    float currMin = 0;
    for(int i = 0 ; i < var ; i++){
          if(payoff_matrix[eqn][i] > 0) continue;
          if(payoff_matrix[eqn][i] == 0){
               infinite = true;
               return -1;
          }
          if(payoff_matrix[eqn][i] < currMin){
               currMin = payoff_matrix[eqn][i];
               ind = i;
         }
    }
    return ind;
}

int getPivotCol(int pivotRow){
     int ind = -1;
     float currMin = 100000;
     for(int i = 0 ; i < var; i++){
          if(payoff_matrix[pivotRow][i] >= 0)
               continue;
          if(abs(payoff_matrix[eqn][i]/payoff_matrix[pivotRow][i]) < currMin){
               ind = i;
               currMin = abs(payoff_matrix[eqn][i]/payoff_matrix[pivotRow][i]);
           }
     }
     return ind;
}

int getMinFromLastCol(){
    int ind = -1;
    float currMin = 0;
    for(int i = 0 ; i < eqn ; i++){
          if(payoff_matrix[i][var] > 0)
               continue;
          if(payoff_matrix[i][var] == 0){
               infinite = true;
               return -1;
          }
          if(payoff_matrix[i][var] < currMin){
               currMin = payoff_matrix[i][var];
               ind = i;
         }
    }
    return ind;
}

void compute(){
	int pivotRow, pivotCol;
    while((pivotCol = getMinFromLastRow()) != -1){
    	printTableau();
      	if((pivotRow = getPivotRow(pivotCol)) == -1){
	           unbounded = true;
	           return;
	      }
	    for(int i = 0 ; i <= eqn ; i++) {
	    	for(int j = 0 ; j <= var; j++) {
	    		if(i == pivotRow && j == pivotCol)
	    			temp[i][j] = 1/payoff_matrix[i][j];
	    		else if(i == pivotRow)
	    			temp[i][j] = (payoff_matrix[i][j])/payoff_matrix[pivotRow][pivotCol];
	    		else if(j == pivotCol)
	    			temp[i][j] = ((-1)*payoff_matrix[i][j])/payoff_matrix[pivotRow][pivotCol];
	    		else 
	    			temp[i][j] = ((payoff_matrix[pivotRow][pivotCol]*payoff_matrix[i][j]) - (payoff_matrix[pivotRow][j]*payoff_matrix[i][pivotCol]))/payoff_matrix[pivotRow][pivotCol];
	           }
	       }
	       copy();
	   }
	   printTableau();
}

void compute_dual(){
     int pivotRow, pivotCol;
     while((pivotRow = getMinFromLastCol()) != -1){
          printTableau();
          if((pivotCol = getPivotCol(pivotRow)) == -1){
               unbounded = true;
               return;
          }
          for(int i = 0 ; i <= eqn ; i++){
               for(int j = 0 ; j <= var; j++){
                    if(i == pivotRow && j == pivotCol)
                         temp[i][j] = 1/payoff_matrix[i][j];
                    else if(i == pivotRow)
                         temp[i][j] = (payoff_matrix[i][j])/payoff_matrix[pivotRow][pivotCol];
                    else if(j == pivotCol)
                         temp[i][j] = ((-1)*payoff_matrix[i][j])/payoff_matrix[pivotRow][pivotCol];
                    else{
                        temp[i][j] = ((payoff_matrix[pivotRow][pivotCol]*payoff_matrix[i][j]) - (payoff_matrix[pivotRow][j]*payoff_matrix[i][pivotCol]))/payoff_matrix[pivotRow][pivotCol];
                    }
               }
          }
          copy();
     }
     printTableau();
}

int main() {
	int m, n;
	double A[MAX][MAX];
	cout << "Size of the payoff_matrix? (m, n):";
	cin >> m >> n;
	double min, max, gameValue;
	vector<double> minGain, maxLoss;
	cout << "Enter the payoff matrix" << endl;
	for (int i = 0; i < m; ++i) {
		min = INF;
		for (int j = 0; j < n; ++j) {
			cin >> A[i][j];
			// For given i
			if (A[i][j] < min) {
				min = A[i][j];
			}
		}
		minGain.push_back(min);
	}
	for (int j = 0; j < n; ++j) {
		max = -INF;
		for (int i = 0; i < m; ++i) {
			if (A[i][j] > max) {
				max = A[i][j];
			}
		}
		maxLoss.push_back(max);
	}
	double minimax = *min_element(maxLoss.begin(), maxLoss.end());
	double maximin = *max_element(minGain.begin(), minGain.end());
	cout << "Minimax value is " << minimax << endl;
	cout << "Maximin value is " << maximin << endl;
	assert(minimax >= maximin);
	if (minimax == maximin) {
		gameValue = minimax;
		cout << "The value of the game is:" << gameValue;
		exit(0);
	} else {
		cout << "The game is unstable. Proceeding on to solve using LPP method.";
	}
    
	var = n;
	eqn = m;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			payoff_matrix[i][j] = A[i][j];
		}
		payoff_matrix[i][n] = 1;
	}
	for (int j = 0; j < n; ++j) {
		payoff_matrix[m][j] = -1;
	}
	payoff_matrix[m][n] = 0;
	cout << "Solving by Primal Simplex Method" << endl;
    compute();
    cout << "Game Value:" << (1/payoff_matrix[m][n]) << endl << endl;
    
    var = m;
    eqn = n;
    for (int i = 0; i < n; ++i) {
    	for (int j = 0; j < m; ++j) {
    		payoff_matrix[i][j] = -A[j][i];
    	}
    	payoff_matrix[i][m] = -1;
    }
    for (int j = 0; j < m; ++j) {
    	payoff_matrix[n][j] = -1;
    }
    payoff_matrix[n][m] = 0;
	cout << "Solving by Dual Simplex Method" << endl;
    compute_dual();
    cout << "Game Value:" << (1/payoff_matrix[m][n]) << endl << endl;
	return 0;
}