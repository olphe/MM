// C++11
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

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

class ConstrainedPermutation {
public:
	XorShift xs;
	const long long int MOD = 1000000007;
	vector<int> permute(int N, vector<string> constraints) {
		int K = constraints.size();
		vector<int> ret(N);
		vector<int>first_to(N);
		vector<int>first_from(N);
		vector<int>second_to(N);
		vector<int>second_from(N);
		vector<vector<int>>to(5, vector<int>(N));
		vector<vector<int>>from(5, vector<int>(N));
		vector<pair<int, int>>num(K);
		vector<vector<int>>check(N);
		for (int i = 0; i < K; i++) {
			string s, t;
			bool flag = true;
			for (auto j : constraints[i]) {
				if (j == ' ') {
					flag = false;
					continue;
				}
				if (flag) {
					s += j;
					continue;
				}
				t += j;
			}
			int box = 0;
			int bag = 0;
			for (auto k : s) {
				box *= 10;
				box += k - '0';
			}
			for (auto k : t) {
				bag *= 10;
				bag += k - '0';
			}
			num[i] = { box,bag };
			check[box].push_back(i);
			check[bag].push_back(i);
		}
		for (int i = 0; i < K; i++) {
			from[0][num[i].second]++;
			to[0][num[i].first]++;
		}
		for (int i = 1; i < 5; i++) {
			for (int j = 0; j < K; j++) {
				from[i][num[j].second] += from[i - 1][num[j].first];
				to[i][num[j].first] += to[i - 1][num[j].second];
			}
		}
		long long int ten[5] = { 1000,1000000,1000000000,1000000000000 ,1000000000000000 };
		vector<long long int>flavor(N);
		for (int i = 0; i < N; i++) {
			flavor[i] += from[0][i];
			flavor[i] -= to[0][i];
			flavor[i] += ten[0];
			flavor[i] = flavor[i] * 1007 + i;
		}
		sort(flavor.begin(), flavor.end());
		vector<int> box(N);
		for (int i = 0; i < N; i++) {
			box[flavor[i] % 1007] = i;
		}
		ret = box;
		int sum = 0;
		for (int k = 0; k < K; k++) {
			if (box[num[k].first] < box[num[k].second]) {
				sum++;
			}
		}
		for (int i = 1; i < 5; i++) {
			for (int j = 0; j < N; j++) {
				flavor[j] = from[i][j];
				flavor[j] -= to[i][j];
				flavor[j] += ten[i];
				flavor[j] = flavor[j] * 1007 + j;
			}
			sort(flavor.begin(), flavor.end());
			for (int j = 0; j < N; j++) {
				box[flavor[j] % 1007] = j;
			}
			int bag = 0;
			for (int k = 0; k < K; k++) {
				if (box[num[k].first] < box[num[k].second]) {
					bag++;
				}
			}
			if (bag > sum) {
				sum = bag;
				ret = box;
			}
		}
		long long int loop = 0;
		long long int begin = 5000;
		long long int div = 100000000;
		long long int max_loop = 50000000;
		while (loop < max_loop) {
			int c_num = xs.rand() % 100;
			if (c_num < 5) {
				c_num = 3;
			}
			else {
				c_num = 2;
			}
			vector<int>cont(c_num);
			for (int i = 0; i < c_num; i++) {
				cont[i] = xs.rand() % N;
			}
			long long int temp = begin*(max_loop * 8 / 10 - loop) / max_loop;
			set<int>s;
			for (int i = 0; i < c_num; i++) {
				for (auto j : check[cont[i]]) {
					s.insert(j);
				}
			}
			loop += s.size();
			int change = 0;
			int bef = 0;
			int aft = 0;
			for (auto i : s) {
				if (ret[num[i].first] < ret[num[i].second]) {
					bef++;
				}
			}
			for (int i = 0; i < c_num - 1; i++) {
				swap(ret[cont[i]], ret[cont[i + 1]]);
			}
			for (auto i : s) {
				if (ret[num[i].first] < ret[num[i].second]) {
					aft++;
				}
			}
			change = aft - bef;
			if (change == 0) {
				if (loop % 2) {
					change++;
				}
				else {
					change--;
				}
			}
			if (change < 0&&(xs.rand()%div>=temp)) {
				for (int i = c_num - 2; i >= 0; i--) {
					swap(ret[cont[i]], ret[cont[i + 1]]);
				}
			}
		}
		return ret;
	}
};
// -------8<------- end of solution submitted to the website -------8<-------

int main() {
	ConstrainedPermutation cp;
	int N, K;
	cin >> N >> K;
	vector<string> constraints(K);
	for (int k = 0; k < K; ++k) {
		int i, j;
		cin >> i >> j;
		constraints[k] = to_string(i) + " " + to_string(j);
	}

	vector<int> ret = cp.permute(N, constraints);
	cout << ret.size() << endl;
	for (int i = 0; i < (int)ret.size(); ++i)
		cout << ret[i] << endl;
	cout.flush();
}