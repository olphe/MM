#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


using namespace std;

class XorShift {
	unsigned int x, y, z, w, t;
public:
	XorShift() {
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
	int loop;
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
		loop = 15000000 / (S*S);
		int ratio;
		if (S < 250) {
			ratio = 100;
		}
		else if (S < 400) {
			ratio = 50;
		}
		else {
			ratio = 25;
		}
		int MOD = 10000;
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
				if (parent[i][j] < child[i][j] * 2) {
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
		for (int a = 0; a < loop; a++) {
			ratio -= ratio / (loop - a);
			for (int i = 0; i < S; i++) {
				for (int j = 0; j < S; j++){
					int add = 1;
					if (ret[i][j] == 'K') {
						add = -1;
					}
					int dif = 0;
					for (int k = 0; k < 8; k++) {
						if (In(S, S, i + diry[k], j + dirx[k])) {
							dif += (abs(attacked[i + diry[k]][j + dirx[k]] - board[i + diry[k]][j + dirx[k]] + '0') - abs(attacked[i + diry[k]][j + dirx[k]] + add - board[i + diry[k]][j + dirx[k]] + '0'));
						}
					}
					if ((dif > 0&&xs.rand()%MOD>ratio)||(dif==0&&xs.rand()%2&&xs.rand()>ratio)||(dif<0&&xs.rand()%MOD<ratio)) {
						if (ret[i][j] == 'K') {
							ret[i][j] = '.';
						}
						else {
							ret[i][j] = 'K';
						}
						for (int k = 0; k < 8; k++) {
							if (In(S, S, i + diry[k], j + dirx[k])) {
								attacked[i + diry[k]][j + dirx[k]] += add;
							}
						}
					}
				}
			}
			for (int i = 0; i < S; i++) {
				for (int j = 0; j < S; j++) {
					int dif[10][10] = {};
					vector<bool>change(8, false);
					for (int k = 0; k < 8; k++) {
						if (!In(S, S, i + diry[k], j + dirx[k]))continue;
						if (xs.rand() % all[i][j] < (board[i][j] - '0')) {
							change[k] = true;
							if (ret[i + diry[k]][j + dirx[k]] == 'K')continue;
							int cy = i + diry[k];
							int cx = j + dirx[k];
							int add = 1;
							for (int l = 0; l < 8; l++) {
								if (In(S, S, cy + diry[l], cx + dirx[l])) {
									dif[diry[k] + diry[l] + 4][dirx[k] + dirx[l] + 4] += add;
								}
							}
						}
						else {
							if (ret[i + diry[k]][j + dirx[k]] != 'K')continue;
							change[k] = false;
							int cy = i + diry[k];
							int cx = j + dirx[k];
							int add = -1;
							for (int l = 0; l < 8; l++) {
								if (In(S, S, cy + diry[l], cx + dirx[l])) {
									dif[diry[k] + diry[l] + 4][dirx[k] + dirx[l] + 4] += add;
								}
							}
						}
					}
					int sum = 0;
					for (int l = 0; l < 9; l++) {
						for (int k = 0; k < 9; k++) {
							if (!dif[l][k])continue;
							if (In(S, S, i + l - 4, j + k - 4)) {
								sum += (abs(attacked[i + l - 4][j + k - 4] - board[i + l - 4][j + k - 4] + '0') - abs(attacked[i + l - 4][j + k - 4] + dif[l][k] - board[i + l - 4][j + k - 4] + '0'));
							}
						}
					}
					if (sum > 0||(sum==0&&xs.rand()%2)) {
						for (int k = 0; k < 8; k++) {
							if (change[k]) {
								int cy = i + diry[k];
								int cx = j + dirx[k];
								if (ret[cy][cx] == 'K')continue;
								int add = 1;
								ret[cy][cx] = 'K';
								for (int l = 0; l < 8; l++) {
									if (In(S, S, cy + diry[l], cx + dirx[l])) {
										attacked[cy + diry[l]][cx + dirx[l]] += add;
									}
								}
							}
							else {
								int cy = i + diry[k];
								int cx = j + dirx[k];
								if (ret[cy][cx] != 'K')continue;
								int add = -1;
								ret[cy][cx] = '.';
								for (int l = 0; l < 8; l++) {
									if (In(S, S, cy + diry[l], cx + dirx[l])) {
										attacked[cy + diry[l]][cx + dirx[l]] += add;
									}
								}
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