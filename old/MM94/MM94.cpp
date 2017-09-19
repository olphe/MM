#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <set>
#include <queue>
#include <random>

using namespace std;
const long long int MOD = 1000000007;

vector<int>ma;
int S;
int dir[] = { 1,0,-1,0,1 };

class Beam {
public:
	vector<int>v;
	vector<bool>order;
	vector<vector<int>>field;
	vector<vector<bool>>used;
	long double score;
	int num;
	int sum;
	queue<pair<int, int>>  Q;
	Beam (int n) {
		order.resize(S, true);
		field.resize(S + 2, vector<int>(S + 2, 0));
		used.resize(S + 2, vector<bool>(S + 2, true));
		num = 1;
		sum = ma[n*S+n];
		score = ma[n*S + n];
		v.push_back(n);
		Q.push({ 1,1 });
		used[1][1] = false;
		field[1][1] = ma[n*S + n];
		order[n] = false;
		return;
	}
	void Push(int n) {
		order[n] = false;
		v.push_back(n);
		int m = v.size();
		for (int i = 1; i <= m; i++) {
			field[i][m] = ma[v[i - 1] * S + v[m - 1]];
			field[m][i] = ma[v[m - 1] * S + v[i - 1]];
		}
		while (!Q.empty()) {
			int cy = Q.front().first;
			int cx = Q.front().second;
			for (int i = 0; i < 4; i++) {
				int my = cy + dir[i];
				int mx = cx + dir[i+1];
				if (field[my][mx] && used[my][mx]) {
					Q.push({ my,mx });
					used[my][mx] = false;
					num++;
					sum += field[my][mx];
				}
			}
			Q.pop();
		}
		for (int i = 1; i <= m; i++) {
			if (!used[i][m])Q.push({ i,m });
			if (!used[m][i]&&i!=m)Q.push({ m,i });
		}
		score = (long double)sum*(long double)sqrt(num);
		return;
	}
	bool operator < (const Beam& b) const{
		return score < b.score;
	}
	bool operator > (const Beam& b) const{
		return score > b.score;
	}
	bool Can_Push(int n) {
		return order[n];
	}
	vector<int> GetV() {
		return v;
	}
};

class ConnectedComponent {
public:
	vector<int> permute(vector<int> matrix) {
		ma = matrix;
		S = (int)sqrt(matrix.size());
		int ms = min(100, max(2, 10000000 / S / S / S));
		vector<long long int>flavor(S);
		long long int by = ((long long int)S*(long long int)S*(long long int)S*(long long int)S*(long long int)S*(long long int)S) / 40000000000000;
		if (S > 300) {
			vector<int>ret;
			long double score = 0;
			for (int loop = 8; loop < 15; loop++) {
				vector<int>res;
				vector<bool>flag(S, true);
				while (res.size() < S) {
					long long int box = 0;
					for (int i = 0; i < S; i++) {
						if (!flag[i])continue;
						long long int bag = 0;
						for (int j = 0; j < S; j++) {
							if (!flag[j] || i == j) {
								if (matrix[i*S + j] != 0) {
									bag += (matrix[i*S + j] + loop);
								}
								if (matrix[j*S + i] != 0) {
									bag += (matrix[j*S + i] + loop);
								}
							}
						}
						if (matrix[i*S + i] != 0) {
							bag -= (matrix[i*S + i] + loop);
						}
						if (box <= bag*MOD + i) {
							box = bag*MOD + i;
						}
					}
					res.push_back(box%MOD);
					flag[box%MOD] = false;
				}
				vector<vector<int>>field(S + 2, vector<int>(S + 2, 0));
				vector<vector<bool>>used(S + 2, vector<bool>(S + 2, true));
				int num = 0;
				int sum = 0;
				long double cscore = 0;
				for (int i = 1; i <= S; i++) {
					for (int j = 1; j <= S; j++) {
						field[i][j] = ma[res[i - 1] * S + res[j - 1]];
					}
				}
				for (int i = 1; i <= S; i++) {
					for (int j = 1; j <= S; j++) {
						if (field[i][j] && used[i][j]) {
							queue<pair<int, int>>Q;
							Q.push({ i,j });
							num = 1;
							sum = field[i][j];
							used[i][j] = false;
							while (!Q.empty()) {
								int cy = Q.front().first;
								int cx = Q.front().second;
								for (int k = 0; k < 4; k++) {
									int ny = cy + dir[k];
									int nx = cx + dir[k + 1];
									if (field[ny][nx] && used[ny][nx]) {
										used[ny][nx] = false;
										num++;
										sum += field[ny][nx];
										Q.push({ ny,nx });
									}
								}
								Q.pop();
							}
							cscore = max(cscore, (long double)sum*sqrt(num));
						}
					}
				}
				if (score < cscore) {
					ret = res;
					score = cscore;
				}
			}
			return ret;
		}
		if (S > 200) {
			by = max((int)by, 2);
			int mi = 0;
			for (int i = 0; i < S; i++) {
				for (int j = 0; j < S; j++) {
					flavor[i] += ma[i*S + j];
					flavor[i] += ma[j*S + i];
				}
				flavor[i] -= ma[i*S + i];
				mi = min(mi, (int)flavor[i]);
			}
			for (int i = 0; i < S; i++) {
				flavor[i] -= mi;
				flavor[i] = flavor[i] * MOD + i;
			}
			sort(flavor.begin(), flavor.end());
			reverse(flavor.begin(), flavor.end());
		}

		multiset<Beam>s[2];

		for (int i = 0; i < S; i++) {
			if (!ma[i*S + i])continue;
			Beam beam(i);

			s[0].insert(beam);
			if (s[0].size() > ms) {
				s[0].erase(s[0].begin());
			}

		}
		for (int i = 1; i < S; i++) {

			if (!s[i % 2].empty())s[i % 2].clear();
			for (auto j : s[(i + 1) % 2]) {

				if (S > 200) {
					int box = 0;
					for (int k = 0; k < S; k++) {
						//if (j.Can_Push(flavor[k] % MOD)) {
						if(j.Can_Push(k)){
							Beam b = j;
							b.Push(k);

							s[i % 2].insert(b);
							if (s[i % 2].size() > ms) {
								s[i % 2].erase(s[i % 2].begin());
							}
							box++;
							if (box >= (S - i) / by)break;
						}
					}
				}
				else {
					for (int k = 0; k < S; k++) {
						if (j.Can_Push(k)) {
							Beam b = j;
							b.Push(k);

							s[i % 2].insert(b);
							if (s[i % 2].size() > ms) {
								s[i % 2].erase(s[i % 2].begin());
							}

						}
					}
				}
			}
		}

		auto sc = *s[(S - 1) % 2].begin();

		pair<Beam, long double>ret = { sc,sc.score };

		for (auto i : s[(S - 1) % 2]) {

			long double csc = i.score;
			if (ret.second < csc) {
				ret = { i,csc };
			}
		}
		auto ans = ret.first;
		auto res=ans.GetV();
		return res;
	}
};


// -------8<------- end of solution submitted to the website -------8<-------

template<class T> void getVector(vector<T>& v) {
	for (int i = 0; i < v.size(); ++i)
		cin >> v[i];
}

int main() {
	ConnectedComponent cc;
	int M;
	cin >> M;
	vector<int> matrix(M);
	getVector(matrix);

	vector<int> ret = cc.permute(matrix);
	cout << ret.size() << endl;
	for (int i = 0; i < (int)ret.size(); ++i)
		cout << ret[i] << endl;
	cout.flush();
}