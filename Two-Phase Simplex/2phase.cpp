#include <bits/stdc++.h>

using namespace std;

int min_index(float *fin_row, int length){
    int index = 0;
    for (int i = 1; i < length; i++) {
        if (fin_row[i] < fin_row[index]) index = i;
    }
    if (fin_row[index] >= 0) return -1;
    else return index;
}

int min_col_index(float *temp, int length){
    int index = 0;
    for (int i = 1; i < length; i++){
        if (temp[i] < temp[index]) index = i;
    }
    return index;
}

void print(int n, int m, float **A){
    cout << endl << endl;
    for (int i = 0; i < m; i++){
        for (int j = 0; j < (n); j++)
            cout << setw(10) << setprecision(5) << A[i][j];
        cout << endl;
    }
}

void simplex(float **tableau, int n, int m, int *index, int max_min){
    float *temp;
    int i, j, pc=0, pr, count;
    bool stop = true, unbounded = false;

    for (i = 0; i < n; i++){
        if (tableau[m][i] <= 0) stop=false;
    }

    if (stop == true){
        if (max_min == 1) cout << endl << "Optimal Solution is " << tableau[m][n] << endl;
        if (max_min == 2) cout << endl << "Optimal Solution is " << (-1) * tableau[m][n] << endl;
        
        float *sol_;
        sol_ = new float [n];
        for (i = 0; i < m; i++){
            if (index[i] >= 0) sol_[index[i]] = tableau[i][n];
        }
        for (i=0; i<n; i++){
            cout << "x" << i + 1 << " is " << sol_[i] << endl;
        }
        return;
    }

    pc = min_index(tableau[m], n);
    
    if (!tableau[m][pc]){
        if (max_min==1) cout << endl << "Alternate Solution Exists. Optimal Solution is " << tableau[m][n] << endl;
        if (max_min==2) cout << endl << "Alternate Solution Exists. Optimal Solution is " << (-1) * tableau[m][n] << endl;
        float *sol_;
        sol_ = new float [n];
        for (i = 0; i < m; i++){
            if (index[i] >= 0) sol_[index[i]] = tableau[i][n];
        }
        for (i = 0; i < n; i++){
            cout << "x" << i + 1 << " is " << sol_[i] << endl;
        }
        return;
    }


    count = 0;
    for (i = 0; i < m; i++){
        if (tableau[i][pc] <= 0) count++;
    }
    if (count == m){
        cout << "Solution is Unbounded" << endl;
        return;
    }


    temp = new float [m];
    for (i = 0; i < m; i++){
        if (tableau[i][pc] > 0) temp[i] = tableau[i][n] / tableau[i][pc];
        else temp[i] = 1000000;
    }
    pr = min_col_index(temp, m);
    cout << "Row Column" << pr << "  " << pc << endl;

    index[pr] = pc;
    float swap;
    swap = tableau[m + 2][pc];
    tableau[m + 2][pc] = tableau[pr][n + 1];
    tableau[pr][n + 1] = swap;


    float pivot = tableau[pr][pc];
    for (i = 0; i <= m + 1; i++){
        for(j = 0; j <= n; j++){
            if (i!= pr && j!= pc){
                tableau[i][j] = (tableau[i][j] * tableau[pr][pc] - tableau[pr][j] * tableau[i][pc]) / tableau[pr][pc];
            }
        }
    }

    for (i = 0; i <= n; i++) tableau[pr][i] = tableau[pr][i] / pivot;

    for (i = 0; i <= m + 1; i++) tableau[i][pc] = -tableau[i][pc] / pivot;
    tableau[pr][pc]=1 / pivot;
     
    print(n + 1, m + 1, tableau);

    simplex(tableau, n, m, index, max_min);
}


int main(){
    int n, m, i, j, *sol_index, geq = 0, leq = 0, eq = 0, M = 1000, max_min;
    float **A, **tableau, *z, temp, *b;

    float c[100], b1[100], a[100][100];
    int ies[100];

    cout << "Enter number of equations (m) : ";
    cin >> m;
    cout << "Enter number of variables (n) : ";
    cin >> n;
    cout << "\nEnter the constraint equations:\n";
	cout << "Please enter constraints in the order, >=, =, then <=" << endl;
    for(int i = 0; i < m; i++){
        cout << "(Coefficients of constraint equation - " << (i + 1) << ")\n";
        for(int j = 0; j < n; j++){
            cout << "Input for matrix A's equation " << (i + 1) << " coefficient of x" << (j + 1) << " = ";
            cin >> a[i][j];
        }
        cout << "\nType of equation \n -1 : Ax<=B \n 0 : Ax=B \n 1 : Ax>=B \n Enter your option : ";
        cin >> ies[i];
        if(ies[i] == 1) geq++;
        else if(ies[i] == 0) eq++;
        else leq++;
        cout << "Input for matrix B's constant for equation " << (i + 1) << " : ";
        cin >> b1[i];
        cout << endl;
    }

    cout << "\nEnter the coefficients of the maximizing equation (enter -ve coefficients for minimising function) :\n";
    for(int i = 0; i < n; i++)
    {
        cout << "\nInput for objective function's coefficient of x" << (i + 1) << " : ";
        cin >> c[i];
    }


    if (!geq && !eq) M=0;

    A = new float* [m]; 
    tableau = new float* [m + 3];

    for (i = 0; i < m; i++){
        A[i]= new float [n];
        tableau[i]= new float [n + geq + 2];
    }
    tableau[m]= new float [n + geq + 1];
    tableau[m + 1]= new float [n + geq + 1];
    tableau[m + 2]= new float [n + geq + 1];

    cout << "Enter the matrix A in order : \n";
    for (i = 0; i < m; i++){
        for (j =0 ; j < n; j++){
            tableau[i][j] = a[i][j];
        }
    }

    for (i = 0; i < geq; i++){
        for (j = 0; j < m; j++){
            if (i == j) tableau[j][n + i] = -1;
            else tableau[j][n + i] = 0;
        }
    }

    cout << "Enter the column vector B in same order as A : \n";
    b = new float [m];
    for (i = 0; i < m; i++) tableau[i][n + geq] = b1[i];

    for (i = 0; i < geq + eq; i++){
        for (j = 0; j < n + geq + 1; j++)
            tableau[m][j] -= tableau[i][j];
    }

    for (i = 0; i < n + geq; i++) tableau[m + 2][i] = i;

    for (i=0; i<m; i++) tableau[i][n + geq + 1] = 50 + i;

    cout << "Enter the choice : " << endl << "(1) Maximize, (2) Minimize" << endl << "Enter your choice : ";
    cin >> max_min;

    cout << "Enter the objective function z :\n";
    z = new float [n];
    for (i = 0 ; i < n; i++){
        if (max_min == 1) tableau[m + 1][i] = -c[i];
        if (max_min == 2) tableau[m + 1][i] = z[i];
    }

    sol_index = new int[m];

    for (i = 0; i < m; i++) sol_index[i] = -1;

    print(n + geq + 1, m + 1, tableau);
    
    cout<<"Phase I starts"<<endl;
    simplex(tableau, n + geq, m, sol_index, max_min);

    cout<<"Phase I complete"<<endl;

    for (i=0; i <= n + geq; i++) tableau[m][i] = tableau[m + 1][i];

    for (i=0; i < n + geq; i++){
        if (tableau[m + 2][i] >= 50){
            for (j = 0; j < m + 2; j++) tableau[j][i] = 0;
        }
    }

    simplex(tableau, n + geq, m, sol_index, max_min);
    return 0;
}
