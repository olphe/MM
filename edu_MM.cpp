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
			first_from[num[i].second]++;
			first_to[num[i].first]++;
		}
		for (int i = 0; i < K; i++) {
			second_from[num[i].second] += first_from[num[i].first];
			second_to[num[i].first] += first_to[num[i].second];
		}
		vector<long long int>flavor(N);
		for (int i = 0; i < N; i++) {
			flavor[i] += second_from[i];
			flavor[i] -= second_to[i];
			flavor[i] += 1000000;
			flavor[i] = flavor[i] * 1007 + i;
		}
		sort(flavor.begin(), flavor.end());
		for (int i = 0; i < N; i++) {
			ret[flavor[i] % 1007] = i;
		}
		long long int loop = 0;
		long long int begin = 0;
		long long int div = 100000000;
		long long int max_loop = 1000000000;
		while (loop < max_loop) {
			long long int temp = begin*(max_loop * 8 / 10 - loop) / 100000000;
			int a = xs.rand() % N;
			int b = xs.rand() % N;
			loop += check[a].size();
			loop += check[b].size();
			int change = 0;
			for (auto i : check[a]) {
				if (ret[num[i].first] > ret[num[i].second]) {
					change++;
				}
				else {
					change--;
				}
			}
			for (auto i : check[b]) {
				if (ret[num[i].first] > ret[num[i].second]) {
					change++;
				}
				else {
					change--;
				}
			}
			if (change == 0) {
				if (loop % 2) {
					change++;
				}
				else {
					change--;
				}
			}
			if (change > 0 || (xs.rand() % div < temp)) {
				swap(ret[a], ret[b]);
			}
		}
		loop = 0;
		max_loop /= 100;
		while (loop < max_loop) {
			int a = xs.rand() % N;
			int b = xs.rand() % N;
			loop += check[a].size();
			loop += check[b].size();
			int change = 0;
			set<int>s;
			for (auto i : check[a]) {
				s.insert(i);
			}
			for (auto i : check[b]) {
				s.insert(i);
			}
			for (auto i : s) {
				if (ret[num[i].first] > ret[num[i].second]) {
					change++;
				}
				else {
					change--;
				}
			}
			if (change == 0) {
				if (loop % 2) {
					change++;
				}
				else {
					change--;
				}
			}
			if (change > 0) {
				swap(ret[a], ret[b]);
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