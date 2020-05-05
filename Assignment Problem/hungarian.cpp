#include<iostream>
#include<cstring>
using namespace std;

#define N 50
#define INF 10000000

int n;
void tree_insert(int, int);
void update_cost_matrix();

int cost[N][N]; 
int max_match  = 0;
int label_x[N], label_y[N];
int xy[N]; 
int yx[N];
bool S[N], T[N]; 
int slack[N]; 
int slackx[N]; 
int _prev[N];


void initiate() {
    memset(label_x, 0, sizeof(label_x));
    memset(label_y, 0, sizeof(label_y));
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            label_x[x] = max(label_x[x], cost[x][y]);
        }
    }
}

void iterate() {
    if (max_match == n) return;
    int x, y, root;
    int q[n], wr = 0, rd = 0;
    memset(S, false, sizeof(S));
    memset(T, false, sizeof(T));
    memset(_prev, -1, sizeof(_prev));

    for (x = 0; x < n; x++) {
        if (xy[x] == -1) {
            q[wr++] = root = x;
            _prev[x] = -2;
            S[x] = true;
            break;
        }
    }
    for (y = 0; y < n; y++) {
        slack[y] = label_x[root] + label_y[y] - cost[root][y];
        slackx[y] = root;
    }

    while (true) {
        while (rd < wr) {
            x = q[rd++];
            for (y = 0; y < n; y++) {
                if (cost[x][y] == label_x[x] + label_y[y] && !T[y]) {
                    if (yx[y] == -1) break;
                    T[y] = true; 
                    q[wr++] = yx[y]; 
                    tree_insert(yx[y], x);
                    }
                }
                if (y < n) break;
        }
        if (y < n) break;

        update_cost_matrix();
        wr = rd = 0;
        for (y = 0; y<n; y++) {
            if (!T[y] && slack[y] == 0) {
                if (yx[y] == -1) {
                    x = slackx[y];
                    break;
                }
                else {
                    T[y] = true;
                    if (!S[yx[y]]) {
                        q[wr++] = yx[y];
                        tree_insert(yx[y], slackx[y]);
                    }
                }
            }
        }
        if (y < n) break;
    }

    if (y < n) {
        max_match++;
        for (int cx = x, cy = y, ty; cx != -2; cx = _prev[cx], cy = ty) {
            ty = xy[cx];
            yx[cy] = cx;
            xy[cx] = cy;
        }
        iterate();
    }
}

void update_cost_matrix() {
    int x, y, delta = INF;
    for (y = 0; y < n; y++) {
        if (!T[y])  delta = min(delta, slack[y]);
    }
    for (x = 0; x < n; x++) if (S[x]) label_x[x] -= delta;
    for (y = 0; y < n; y++) if (T[y]) label_y[y] += delta; 
    for (y = 0; y < n; y++)
    if (!T[y]) slack[y] -= delta;
}


void tree_insert(int x, int _prevx) {
    S[x] = true;
    _prev[x] = _prevx; 
    for (int y = 0; y < n; y++) {
        if (label_x[x] + label_y[y] - cost[x][y] < slack[y]) {
            slack[y]  = label_x[x] + label_y[y] - cost[x][y];
            slackx[y] = x;
        }
    }
}



int main() {
    cin >> n;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            cin >> cost[row][col];
        }
    }
    int largest = cost[0][0];
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (cost[row][col] >= largest) {
                largest = cost[row][col];
            }
        }
    }
    
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            cost[row][col] = largest - cost[row][col];
        }
    }

    int result = 0;
    max_match  = 0;
    memset(xy, -1, sizeof(xy));
    memset(yx, -1, sizeof(yx));
    initiate();
    iterate();
    for (int x = 0; x<n; x++) {
        cout << "(" << x << ", " << xy[x] << ")" << endl;
        result += cost[x][xy[x]];
    }
    cout << "The minimum cost is: " << n * largest - result << endl;
    return 0;
}