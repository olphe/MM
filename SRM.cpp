#include "iostream"
#include "vector"
#include "random"
#include "climits"
using namespace std;

int N;
vector<int>x;
vector<int>y;
vector<int>order;

long double dis(int a, int b) {
	if (a < 0 || b < 0 || a > N || b > N)return 0;
	return sqrt((long double)(x[a] - x[b]) * (long double)(x[a] - x[b]) + (long double)(y[a] - y[b]) * (long double)(y[a] - y[b]));
}

int main() {

	random_device rnd;
	mt19937_64 mt(rnd());
	cin >> N;
	x.resize(N + 2, INT_MAX);
	y.resize(N + 2, INT_MAX);
	order.resize(N + 2, INT_MAX);
	x[0] = 0;
	y[0] = 0;
	order[0] = 0;
	for (int i = 1; i <= N; i++)cin >> y[i] >> x[i];
	vector<bool>flag(N + 2, true);
	long double dist = 0;
	flag[0] = false;
	for (int i = 1; i <= N; i++) {
		order[i] = i;
		int box = 0;
		for (int j = 1; j <= N; j++) {
			if (flag[j]) {
				if (!box)box = j;
				if (dis(order[i - 1], box) > dis(order[i - 1], j)) box = j;
			}
		}
		order[i] = box;
		dist += dis(order[i - 1], order[i]);
		flag[box] = false;
	}
	int a, b;
	for (int i = 0; i < 30000000; i++) {
		a = mt() % N + 1;
		b = mt() % N + 1;
		if (a == b)continue;
		long double bef = 0;
		long double aft = 0;
		if (abs(a - b) == 1) {
			if (a > b)swap(a, b);
			bef += dis(order[a], order[a - 1]);
			bef += dis(order[b], order[b + 1]);
			aft += dis(order[b], order[a - 1]);
			aft += dis(order[a], order[b + 1]);
			if (aft < bef)swap(order[a], order[b]);
			continue;
		}
		bef += dis(order[a], order[a - 1]);
		bef += dis(order[a], order[a + 1]);
		bef += dis(order[b], order[b - 1]);
		bef += dis(order[b], order[b + 1]);
		aft += dis(order[b], order[a - 1]);
		aft += dis(order[b], order[a + 1]);
		aft += dis(order[a], order[b - 1]);
		aft += dis(order[a], order[b + 1]);
		if (aft < bef)swap(order[a], order[b]);
	}
	for (int i = 1; i <= N; i++) {
		cout << y[order[i]] << " " << x[order[i]] << endl;
	}

	return 0;
}
