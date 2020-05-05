#include <bits/stdc++.h>
using namespace std;


struct Matrix{
	float value;
	int basic;
};

int _count = 1;

int row_min_index(Matrix** T, int row, int nc){
    int first = INT_MAX, ind = 0;  
    for (int i = 0; i < nc ; i ++){
        if (T[row][i].value < first){
            first = T[row][i].value; 
            ind = i; 
        }  
    }
    return ind;
}

int col_min_index(Matrix** T, int col, int nr){
    int first = INT_MAX, ind = 0;  
    for (int i = 0; i < nr ; i ++){
        if (T[i][col].value < first){
            first = T[col][i].value; 
            ind = i; 
        }  
    }
    return ind;
} 

int find_penalty_row(Matrix** T, int row, int nc){
    int first = INT_MAX, second = INT_MAX;  
    for (int i = 0; i < nc ; i ++){
        if (T[row][i].value < first){  
            second = first;  
            first = T[row][i].value;  
        }  
        else if (T[row][i].value < second) second = T[row][i].value;
    }
    return second - first;
}

int find_penalty_col(Matrix** T, int col, int nr){
    int first = INT_MAX, second = INT_MAX;  
    for (int i = 0; i < nr ; i ++){
        if (T[i][col].value < first){  
            second = first;  
            first = T[i][col].value;  
        }  
        else if (T[i][col].value < second) second = T[i][col].value;
    }
    return second - first;
}

void printMatrix(int m, int n, Matrix **A){
	for (int i = 0; i < m; i++){
		for (int j = 0; j < n; j++){
			if(A[i][j].value >= 0) std::cout << setw(10) << setprecision(5) << A[i][j].value;
            else std::cout << setw(10) << setprecision(5) << 0;
        }
		std::cout << endl;
	}
}

void solution(Matrix **T, float **cmatrix, int nr, int nc){
	float value;
	for (int i = 0; i < nr; i++){
		for (int j = 0; j < nc; j++){
			if (T[i][j].basic) value = value + cmatrix[i][j] * T[i][j].value;
        }
    }

	std::cout << endl << "Value is " << value << endl;
}

void northWest(int nr, int nc, Matrix** T){
	int cx = 0, cy = 0;

	while(1){
		T[cx][cy].basic = 1;
		if (cx > nr - 1 || cy > nc - 1) break;
		if (T[nr][cy].value <= T[cx][nc].value){
			T[cx][cy].value = T[nr][cy].value;
			T[cx][nc].value = T[cx][nc].value - T[nr][cy].value;
			T[nr][cy].value = 0;
			cy++;
		}
		else{
			T[cx][cy].value = T[cx][nc].value;
			T[nr][cy].value = T[nr][cy].value - T[cx][nc].value;
			T[cx][nc].value = 0;
			cx++;
		}
	}
	std::cout<<"Initial Solution :: using North West \n";
	printMatrix(nr + 1, nc + 1, T);
}

void matrixMinima(int nr, int nc, Matrix** T, float **cmatrix){
    set<int> rows, cols;
    for(int i = 0 ; i < nr; i++) rows.insert(i);
    for(int i = 0 ; i < nc; i++) cols.insert(i);
    int basicvars = 0;
    while(rows.size() && cols.size()){
        int min_val = INT_MAX;
        pair<int, int> min_ind;
        for(int i = 0 ; i < nr; i++){
            for(int j = 0 ; j < nc; j++){
                if(rows.count(i) && cols.count(j)){
                    int val = cmatrix[i][j];
                    if(val < min_val){
                        min_val = val;
                        min_ind = {i, j};
                    }
                }
            }
        }
        T[min_ind.first][min_ind.second].basic = 1;
        basicvars++;
        if (T[nr][min_ind.second].value <= T[min_ind.first][nc].value){
			T[min_ind.first][min_ind.second].value = T[nr][min_ind.second].value;
			T[min_ind.first][nc].value -= T[nr][min_ind.second].value;
			T[nr][min_ind.second].value = 0;
            cols.erase(min_ind.second);
            if(!cols.size()) rows.erase(min_ind.first);
		}
		else{
			T[min_ind.first][min_ind.second].value = T[min_ind.first][nc].value;
			T[nr][min_ind.second].value -= T[min_ind.first][nc].value;
			T[min_ind.first][nc].value = 0;
			rows.erase(min_ind.first);
            if(!rows.size()) cols.erase(min_ind.second);
		}

    }
    while(basicvars < nc + nr - 1){
        basicvars++;
        if(rows.size()){
            int row = *rows.begin(), min_val = INT_MAX, min_ind = 0;
            for(int i = 0; i < nc; i++){
                if(cmatrix[row][i] < min_val && !T[row][i].basic){
                    min_val = cmatrix[row][i];
                    min_ind = i;
                }
            }
            T[row][min_ind].basic = 1;
        }
        else{
            int col = *cols.begin(), min_val = INT_MAX, min_ind = 0;
            for(int i = 0; i < nr; i++){
                if(cmatrix[i][col] < min_val && !T[i][col].basic){
                    min_val = cmatrix[i][col];
                    min_ind = i;
                }
            }
            T[min_ind][col].basic = 1;
        }
    }
    std::cout<<"Initial Solution :: using MatrixMinima \n";
	printMatrix(nr, nc, T);

}



int move(Matrix **T, int dir, int x, int y, int s, int d, vector< pair < int, int> > &loop){
	int res;
	int temp_x = x, temp_y = y;
	pair<int,int> temp;
	if (loop.size() > 1 && loop.back().first == loop[0].first && loop.back().second == loop[0].second) return 1;

	if (dir==1){
		if (!x){
			res = move(T, 2, x, y, s, d, loop);
			if (res == 1) return 1;
		}
		while (x-- > 0){
			if (T[x][y].basic){
				temp.first = x;
				temp.second = y;
				loop.push_back(temp);
				res = move(T, 2, x, y, s, d, loop);
				if (res == 1) return 1;
			}
		}
		if (x == 0) loop.pop_back();
	}
	if (dir == 2){
		if (y == d - 1){
			res = move(T, 3, x, y, s, d, loop);
			if (res==1) return 1;
		}
		while (y < d - 1){
            y++;
			if (T[x][y].basic){
				temp.first = x;
				temp.second = y;
				loop.push_back(temp);
				res = move(T, 3, x, y, s, d, loop);
				if (res == 1) return 1;
			}
		}
		if (y == d - 1) loop.pop_back();
	}

	if (dir == 3){
		if (x == s - 1){
			res = move(T, 4, x, y, s, d, loop);
			if (res==1) return 1;
		}
		while (x < s - 1){
			x++;
			if (T[x][y].basic){
				temp.first = x;
				temp.second = y;
				loop.push_back(temp);
				res=move(T, 4, x, y, s, d, loop);
				if (res==1) return 1;
			}
		}
		if (x == s - 1) loop.pop_back();
	}

	if (dir == 4){
		if (!y){
			res = move(T, 1, x, y, s, d, loop);
			if (res == 1) return 1;
		}
		while (y > 0){
			y--;
			if (T[x][y].basic){
				temp.first = x;
				temp.second = y;
				loop.push_back(temp);
				res = move(T, 1, x, y, s, d, loop);
				if (res == 1) return 1;
			}
		}
		if (!y) loop.pop_back();
	}
}

void recursefind(Matrix **T, int ex, int ey, int s, int d){
	int x = ex, y = ey, lx, ly;
	vector<pair<int, int>> loop;
	pair<int,int> temp;
	temp.first = x;
	temp.second = y;
	loop.push_back(temp);
	move(T, 1, x, y, s, d, loop);
    //Leaving Variable
    if (T[loop[1].first][loop[1].second].value < T[loop[3].first][loop[3].second].value){
    	lx = loop[1].first;
    	ly = loop[1].second;
    	T[loop[3].first][loop[3].second].value = T[loop[3].first][loop[3].second].value - T[lx][ly].value;
    } 
    else{
    	lx = loop[3].first;
    	ly = loop[3].second;
    	T[loop[1].first][loop[1].second].value = T[loop[1].first][loop[1].second].value - T[lx][ly].value;
    }
    T[loop[2].first][loop[2].second].value = T[lx][ly].value + T[loop[2].first][loop[2].second].value;
    T[lx][ly].basic = 0;
    T[x][y].value = T[lx][ly].value;
    T[lx][ly].value = 0;
 	// printMatrix(s + 2, d + 2, T);	   
}

void next_iteration(Matrix **T, float **cmatrix, int s, int d){
	int flag = 1;
	for (int i = 0; i < s; i++) T[i][d + 1].value = -1111;
	for (int j = 0; j < d; j++) T[s + 1][j].value = -1111;
	// Multipliers calculation
	T[0][d + 1].value = 0;
	while(flag){
		flag = 0;
		for (int i = 0; i < s; i++){
			for (int j = 0; j < d; j++){
				if (T[i][j].basic == 1){
					if (T[i][d + 1].value == -1111 && T[s + 1][j].value == -1111) flag = 1;
					else if (T[i][d + 1].value == -1111) T[i][d + 1].value = cmatrix[i][j] - T[s + 1][j].value;
					else if (T[s + 1][j].value == -1111) T[s + 1][j].value=cmatrix[i][j] - T[i][d + 1].value;
				}
			}
		}
	}

	int evar_x = 0;
	int evar_y = 0;
	float temp = -10000;
	for (int i = 0; i < s; i++){
		for (int j = 0; j < d; j++){
			if (T[i][j].basic != 1){
				T[i][j].value = T[i][d + 1].value + T[s + 1][j].value - cmatrix[i][j];
				if (temp < T[i][j].value){
					temp = T[i][j].value;
					evar_y = j;
					evar_x = i;
				}
			}
		}
	}
	if (temp <= 0){
        std::cout << "Final Allocated Matrix : " << endl;
	    printMatrix(s, d, T);
		return;
	}
	T[evar_x][evar_y].basic = 1;
	recursefind(T, evar_x, evar_y, s, d);
    std::cout << "Solution after iteration " << _count++ << " ";
	solution(T, cmatrix, s, d);
    std::cout << endl;
	next_iteration(T, cmatrix, s, d);
}


int main(){
	int s, d;
	float **cmatrix;
	Matrix **T;
	std::cout << "Enter no of source and destination" << endl;
	cin >> s >> d;
    cmatrix = new float* [s + 1]; 

    for (int i = 0; i < s + 1; i++) cmatrix[i] = new float [d + 1];

    std::cout << "Enter the cmatrix matrix with supply and demand" << endl;
	for (int i = 0; i < s + 1; i++){
		for (int j = 0; j < d + 1; j++){
			cin >> cmatrix[i][j];
		}
	}

    T = new Matrix* [s + 2];
    for (int i = 0; i < s + 2; i++) T[i]= new Matrix [d + 2];

    for (int i = 0; i < s; i++) T[i][d].value = cmatrix[i][d];
    for (int i = 0; i < d; i++) T[s][i].value = cmatrix[s][i];

    northWest(s, d, T);
    // matrixMinima(s, d, T, cmatrix);
    solution(T, cmatrix, s, d);
    std::cout << endl;
    next_iteration(T, cmatrix, s, d);
    
}