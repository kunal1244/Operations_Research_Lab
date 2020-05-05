#include <bits/stdc++.h>
using namespace std;

const double EPSILON = 1e-10;

typedef double T;
typedef vector<T> VT;
typedef vector<VT> VVT;

VVT transpose(VVT b){
   	VVT trans_vec;
	VT temp;
    for (int i = 0; i < b[i].size(); i++){
        for (int j = 0; j < b.size(); j++){
		   	temp.push_back(b[j][i]);
		}
		trans_vec.push_back(temp);
		temp.clear();    
	}
	return trans_vec;
	
}

int rref(VVT &a) {
 	int n = a.size();
  int m = a[0].size();
  int r = 0;
  for (int c = 0; c < m; c++) {
    int j = r;
    for (int i = r + 1; i < n; i++) {
      	if (fabs(a[i][c]) > fabs(a[j][c])){
			j = i;
		}
	}
    if (fabs(a[j][c]) < EPSILON){
		continue;
	}
    swap(a[j], a[r]);
   
    T s = 1.0 / a[r][c];
    for (int j = 0; j < m; j++) a[r][j] *= s;
    for (int i = 0; i < n; i++) if (i != r) {
      T t = a[i][c];
      for (int j = 0; j < m; j++) a[i][j] -= t * a[r][j];
    }
    r++;
  }
  return r;
}

void combinations_util(vector <vector<int> > &ans, vector<int> &tmp, int n, int left, int k) { 
	if (k == 0) { 
		ans.push_back(tmp); 
		return; 
	} 
	for (int i = left; i <= n; ++i) { 
		tmp.push_back(i); 
		combinations_util(ans, tmp, n, i + 1, k - 1); 
		tmp.pop_back(); 
	} 
} 

vector <vector<int> > combinations(int n, int k) { 
	vector <vector<int> > ans; 
	vector<int> tmp; 
	combinations_util(ans, tmp, n, 1, k); 
	return ans; 
}


void basic_solutions(VVT &aug_matrix, int aug_rank, int num_var, int num_eqn){
	vector <vector<int> > combs = combinations(num_var, aug_rank);
	VVT req_col;
	int num = 0;
	aug_matrix = transpose(aug_matrix);
	for(int i = 0; i < combs.size(); i++){
		for(int j = 0; j < combs[i].size(); j++){
			req_col.push_back(aug_matrix[combs[i][j] - 1]);
		}
		req_col.push_back(aug_matrix[num_var]);
		req_col = transpose(req_col);
		rref(req_col);	
		cout << "Solutions are : " << endl;
		int i1 = 0;
		for(int j = 0; j < combs[i].size(); j++){
			if(j == combs[i][j] - 1){
				cout << req_col[i1][aug_rank] / req_col[i1][i1] << " ";
				i1++;
			}
			else{
				cout << "0 ";
			}
		}
		req_col.clear();
		cout << endl;
	}
	aug_matrix = transpose(aug_matrix);
}

int main(){
	VVT aug_matrix, coeff_matrix;
	VT temp;
	int val, num_var, num_eqn;
	cout << "Enter number of variables : " << endl;
	cin >> num_var;
	cout << "Enter number of equations : " << endl;
	cin >> num_eqn;
	for(int i = 0; i < num_eqn; i++){
		cout << "Enter coeffiecients for equation " << i + 1 << endl;
		for(int j = 0; j < num_var; j++){
			cout << "Enter coeffiecient " << j + 1 << " : ";
			cin >> val;
			temp.push_back(val);
		}
		coeff_matrix.push_back(temp);
		cout << "Enter RHS constant : ";
		cin >> val;
		temp.push_back(val);
		aug_matrix.push_back(temp);		
		temp.clear();
	}
	if(num_eqn < num_var + 1){
		for(int i = 0; i < num_var - num_eqn + 1; i++){
			for(int j = 0; j < num_var + 1; j++){
				temp.push_back(0);
			}
		} 
		aug_matrix.push_back(temp);
		temp.clear();
		for(int i = 0; i < num_var - num_eqn + 1; i++){
			for(int j = 0; j < num_var; j++){
				temp.push_back(0);
			}
		} 
		coeff_matrix.push_back(temp);
		temp.clear();
	}
	int aug_rank = rref(aug_matrix);
	int coeff_rank = rref(coeff_matrix);
	if(coeff_rank == aug_rank){
		if(coeff_rank == num_var){
			cout << "Solutions are : ";
			for(int i = 0; i < num_eqn; i++){
				cout << aug_matrix[i][num_var] / aug_matrix[i][i] << " ";
			}
			cout << endl;
		}
		else{
			basic_solutions(aug_matrix, aug_rank, num_var, num_eqn);
		}
	}
	else if(coeff_rank < aug_rank){
		cout << "The system has no solutions" << endl;
	}
	return 0;
}
