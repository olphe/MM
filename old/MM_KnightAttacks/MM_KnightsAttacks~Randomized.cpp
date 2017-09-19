#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>


using namespace std;

class XorShift {
	unsigned int x, y, z, w, t;
public:
	XorShift() {
		x = 133553533;
		y = 314867339;
		z = 664298413;
		w = 999999937;
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
		loop = 160000000 / (S*S);
		int ratio;
		ratio = 250000;
		int MOD = 100000000;
		vector<string> ret(S, string(S, '.'));
		vector<vector<int>>attacked(S, vector<int>(S, 0));
		for (int i = 0; i < S; ++i) {
			for (int j = 0; j < S; ++j) {
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
		for (int a = 0; a < loop; a++) {
			ratio -= ratio / (loop - a);
			for (int i = 0; i < S; i++) {
				for (int j = 0; j < S; j++) {
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
					if (dif > 0 || (dif == 0 && xs.rand() % 2) || (dif<0 && xs.rand() % MOD<ratio)) {
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
		}
		for (int a = 0; a < loop / 10; a++) {
			for (int i = 0; i < S; i++) {
				for (int j = 0; j < S; j++) {
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
					if (dif > 0 || (dif == 0 && xs.rand() % 2)) {
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