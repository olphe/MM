#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


using namespace std;

class XorShift {
	unsigned int x, y, z, w, t;
public:
	XorShift(){
		x = 123456789;
		y = 512821133;
		z = 756587678;
		w = 176232445;
		t = 0;
	}
	unsigned int rand() {
		t = x ^ (x << 11);
		x = y;
		y = z;
		z = w;
		w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
		return w & 0x7fffffff;
	}
};

class KnightsAttacks {
	int loop=1000000;
	int diry[8] = { 1,2,2,1,-1,-2,-2,-1 };
	int dirx[8] = { -2,-1,1,2,2,1,-1,-2 };
	bool In(int h, int w, int y, int x) {
		if (y >= 0 && y < h&&x >= 0 && x < w) {
			return true;
		}
		else {
			return false;
		}
	}
public:
	XorShift xs;
	vector<string> placeKnights(vector<string> board) {
		int S = board.size();
		vector<string> ret(S, string(S, '.'));
		vector<vector<int>> all(S, vector<int>(S, 0));
		vector<vector<int>>child(S, vector<int>(S, 0));
		vector<vector<int>>parent(S, vector<int>(S, 0));
		vector<vector<int>>attacked(S, vector<int>(S, 0));
		for (int i = 0; i < S; i++) {
			for (int j = 0; j < S; j++) {
				for (int k = 0; k < 8; k++) {
					if (In(S, S, i + diry[k], j + dirx[k])) {
						all[i][j]++;
					}
				}
			}
		}
		for (int i = 0; i < S; i++) {
			for (int j = 0; j < S; j++) {
				for (int k = 0; k < 8; k++) {
					if (In(S, S, i + diry[k], j + dirx[k])) {
						child[i + diry[k]][j + dirx[k]] += board[i][j] - '0';
						parent[i + diry[k]][j + dirx[k]] += all[i][j];
					}
				}
			}
		}
		for (int i = 0; i < S; ++i) {
			for (int j = 0; j < S; ++j) {
				if (parent[i][j]<child[i][j]*2) {
					ret[i][j] = 'K';
					for (int k = 0; k < 8; k++) {
						if (In(S, S, i + diry[k], j + dirx[k])) {
							attacked[i + diry[k]][j + dirx[k]]++;
						}
					}
				}
				if (parent[i][j] == child[i][j] * 2) {
					if (xs.rand() % 2) {
						ret[i][j] = 'K';
						for (int k = 0; k < 8; k++) {
							if (In(S, S, i + diry[k], j + dirx[k])) {
								attacked[i + diry[k]][j + dirx[k]]++;
							}
						}
					}
				}
			}
		}
		for(int a=0;a<10;a++){
			for (int i = 0; i < loop; i++) {
				int y, x;
				y = xs.rand() % S;
				x = xs.rand() % S;
				int add = 1;
				if (ret[y][x] == 'K') {
					add = -1;
				}
				int dif = 0;
				for (int j = 0; j < 8; j++) {
					if (In(S, S, y + diry[j], x + dirx[j])) {
						dif += (abs(attacked[y + diry[j]][x + dirx[j]] - board[y + diry[j]][x + dirx[j]] + '0') - abs(attacked[y + diry[j]][x + dirx[j]] + add - board[y + diry[j]][x + dirx[j]] + '0'));
					}
				}
				if (dif > 0||(dif == 0 && xs.rand() % 2)) {
					if (ret[y][x] == 'K') {
						ret[y][x] = '.';
					}
					else {
						ret[y][x] = 'K';
					}
					for (int j = 0; j < 8; j++) {
						if (In(S, S, y + diry[j], x + dirx[j])) {
							attacked[y + diry[j]][x + dirx[j]] += add;
						}
					}
				}
			}
			for (int i = 0; i < loop; i++) {
				int y, x;
				y = xs.rand() % S;
				x = xs.rand() % S;
				int dif[10][10] = {};
				vector<bool>change(8, false);
				for (int k = 0; k < 8; k++) {
					if (!In(S, S, y + diry[k], x + dirx[k]))continue;
					if (xs.rand() % 2)continue;
					change[k] = true;
					int cy = y + diry[k];
					int cx = x + dirx[k];
					int add = 1;
					if (ret[cy][cx] == 'K') {
						add = -1;
					}
					for (int j = 0; j < 8; j++) {
						if (In(S, S, cy + diry[j], cx + dirx[j])) {
							dif[diry[k] + diry[j] + 4][dirx[k] + dirx[j] + 4] += add;
						}
					}
				}
				int sum = 0;
				for (int j = 0; j < 9; j++) {
					for (int k= 0; k < 9; k++) {
						if (!dif[j][k])continue;
						if (In(S, S, y + j-4, x + k-4)) {
							sum += (abs(attacked[y + j-4][x + k-4] - board[y + j-4][x + k-4] + '0') - abs(attacked[y + j-4][x + k-4] + dif[j][k] - board[y + j-4][x + k-4] + '0'));
						}
					}
				}
				if (sum > 0||(sum == 0 && xs.rand() % 2)) {
					for (int k = 0; k < 8; k++) {
						if (!change[k])continue;
						int cy = y + diry[k];
						int cx = x + dirx[k];
						int add = 1;
						if (ret[cy][cx] == 'K') {
							add = -1;
						}
						if (ret[cy][cx] == 'K') {
							ret[cy][cx] = '.';
						}
						else {
							ret[cy][cx] = 'K';
						}
						for (int j = 0; j < 8; j++) {
							if (In(S, S, cy + diry[j], cx + dirx[j])) {
								attacked[cy + diry[j]][cx + dirx[j]] += add;
							}
						}
					}
				}
			}
		}
		return ret;
	}
};
// -------8<------- end of solution submitted to the website -------8<-------

template<class T> void getVector(vector<T>& v) {
	for (int i = 0; i < v.size(); ++i)
		cin >> v[i];
}

int main() {
	KnightsAttacks ka;
	int S;
	cin >> S;
	vector<string> board(S);
	getVector(board);

	vector<string> ret = ka.placeKnights(board);
	cout << ret.size() << endl;
	for (int i = 0; i < (int)ret.size(); ++i)
		cout << ret[i] << endl;
	cout.flush();
}