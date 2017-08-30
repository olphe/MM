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
						child[i + diry[k]][j + dirx[k]] += (int)(board[i][j] - '0');
						parent[i + diry[k]][j + dirx[k]] += all[i][j];
					}
				}
			}
		}

		for (int i = 0; i < S; ++i) {
			for (int j = 0; j < S; ++j) {
				if (parent[i][j]<child[i][j]*2) {
					ret[i][j] = 'K';
				}
				if (parent[i][j] == child[i][j] * 2) {
					if (xs.rand() % 2) {
						ret[i][j] = 'K';
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