#include <bits/stdc++.h>
using namespace std;
#define M 10000
#define ll long long int

ll _count = 0;

void get_solution(vector<vector<double>> matrix, vector<string> col_headers, vector<string> row_headers, ll n){
    ll final_col = matrix[0].size() - 1;
    cout << "Optimal Solution" << endl;
    for(ll i = 0; i < col_headers.size(); i++){
        if(col_headers[i][0]== 'x'  && col_headers[i][1] - '0' <= n) cout << col_headers[i] << "* = 0, ";
    }
    for(ll i = 0; i < row_headers.size(); i++){
        if(row_headers[i][0]== 'x'  && row_headers[i][1] - '0' <= n) cout << row_headers[i] << "* = " << matrix[i][final_col] << ", ";
    }
    cout << "Z* = " << matrix[matrix.size() - 1][final_col];
    cout << endl;
    return;
}

ll most_negative_col(vector<vector<double>> matrix, vector<string> row_headers){
    ll last_row_ind = matrix.size() - 1;
    ll num_cols = matrix[0].size() - 1;
    double min_val = INT_MAX;
    ll min_ind = 0;
    for(ll i = 0; i < num_cols; i++){
        min_val = min(min_val, matrix[last_row_ind][i]);
        if(matrix[last_row_ind][i] == min_val) min_ind = i;
    }
    if(min_val >= 0) {
        bool flag = false;
        ll count = 0;
        for(int i = 0; i < last_row_ind; i++){
            if(row_headers[i][0] == 'z' && matrix[i][num_cols] > 0){
                count++;
                flag = true;
                break;
            }
        }
        if(!flag && count) return -1;
        else return -2;
    }
    else return min_ind;
}

ll get_row(vector<vector<double>> matrix, ll col){
    ll size = matrix.size() - 1;
    ll cols = matrix[0].size() - 1;
    double min_val = INT_MAX;
    ll row = 0;
    bool flag = false;
    for(ll i = 0; i < size; i++){
        if(matrix[i][col] > 0){
            flag = true;
            double val = matrix[i][cols] / matrix[i][col];
            min_val = min(val, min_val);
            if(val == min_val) row = i; 
        }
    }
    if(!flag) return -1;
    else return row;
}

void simplex(vector<vector<double>> matrix, vector<string> col_headers, vector<string> row_headers, ll n){
    cout << endl << endl;
    ll size = matrix.size();
    ll num_cols = matrix[0].size();
    for(ll i = 0 ; i < num_cols - 1; i++){
        cout << "\t" << col_headers[i];
    }
    cout << endl;
    for(ll i = 0; i < size; i++){
        cout << row_headers[i] << "\t";
        for(ll j = 0; j < num_cols; j++){
            cout << fixed << setprecision(2) << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    ll min_col = most_negative_col(matrix, row_headers);
    if(min_col == -1){
        cout << "Problem is infeasible." << endl; return;
    }
    if(min_col == -2) {
        get_solution(matrix, col_headers, row_headers, n); return;
    }
    
    ll min_row = get_row(matrix, min_col);
    if(min_row == -1){
        cout << "Problem is unbounded." << endl;return;
    }

    double pivot = matrix[min_row][min_col];
    string temp = row_headers[min_row];
    row_headers[min_row] = col_headers[min_col];
    col_headers[min_col] = temp;

    // cout << "minrow : " << min_row << " mincol : " << min_col << endl;
    for(ll i = 0; i < size; i++){
        for(ll j = 0; j < num_cols; j++){
            if(i != min_row && j != min_col) matrix[i][j] -= matrix[i][min_col] * matrix[min_row][j] / pivot;
        }
    }
    for(ll i = 0; i < size; i++) matrix[i][min_col] /= -pivot;
    for(ll i = 0; i < num_cols; i++) matrix[min_row][i] /= pivot;
    matrix[min_row][min_col] = 1 / pivot;
    
    if(_count++ < 10) simplex(matrix, col_headers, row_headers, n);

}

int main(){
    vector<vector<double>> matrix;
    vector<string> col_headers, row_headers;
    ll n, m, geq;
    double temp;
    cout << "Number of constraints : " << endl;
    std::cin >> m;
    cout << "Number of variables : " << endl;
    std::cin >> n;
    // cout << "Please enter the number of '>=' constraints : ";
    // std::cin >> geq;
    geq = 0;
    cout << "Enter constraint equations (Please enter the '>=' contraints first):" << endl;
    for(ll i = 0; i < n + geq; i++){
        col_headers.push_back("x" + to_string(i + 1));
    }

    cout << "Enter maximizing statement : " << endl;
    vector<double> last_row(n + geq + 1);
    for(ll i = 0 ; i < n; i++){
        cout << "Enter coefficient of variable " << i + 1 << ": ";
        std::cin >> last_row[i];
        last_row[i] *= -1;
    }
    for(ll i = 0; i < geq; i++){
        last_row[n + i] = M;
    }
    cout << "Enter constant: ";
    std::cin >> last_row[n + geq];
    last_row[n + geq] *= -1;
    ll geq_count = 0;
    for(ll i = 0; i < m; i++){
        cout << "Equation " << i + 1 << endl;
        ll ineq_type = -1;
        // cout << "Enter type of inequality(-1 for '<=', 1 for '>=', 0 for '='): ";
        std::cin >> ineq_type;
        vector<double> temp_row(n + geq + 1);
        for(ll j = 0; j < n; j++){
            cout << "Enter coefficient of variable " << j + 1 << ": ";
            std::cin >> temp_row[j];
            if(ineq_type > -1) last_row[j] -= temp_row[j] * M;
        }
        
        for(int j = 0; j < geq; j++){
            if(geq_count < geq && j == geq_count) temp_row[n + j] = -1;
            else temp_row[n + j] = 0;
        }
        geq_count++;
        cout << "Enter constant: ";
        std::cin >> temp_row[n + geq];
        if(ineq_type > -1) last_row[n + geq] -= temp_row[n + geq] * M;

        ll size = row_headers.size();
        row_headers.push_back("z" + to_string(size + 1));

        matrix.push_back(temp_row);
    }
    row_headers.push_back("Z");
    matrix.push_back(last_row);
    simplex(matrix, col_headers, row_headers, n);
    
}