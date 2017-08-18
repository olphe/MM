// C++11
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;
const long long int MOD = 1000000000000;

class ConnectedComponent {
public:
	vector<int> permute(vector<int> matrix) {
		int S = (int)sqrt(matrix.size());
		sort(matrix.begin(), matrix.end());
		vector<long long int>flavor(S,0);
		vector<long long int>row(S,0);
		vector<long long int>column(S,0);
		for (int i = 0; i < S; i++) {
			for (int j = 0; j < S; j++) {
				if (matrix[i*S + j] != 0) {
					row[i] += (matrix[i*S + j] + 20)*abs(i - S / 2);
					column[j] += (matrix[i*S + j] + 20)*abs(j - S / 2);
				}
				else{
					row[i] -= 20 * abs(i - S / 2);
					column[j] -= 20 * abs(j - S / 2);
				}
			}
		}
		for (int i = 0; i < S; i++) {
			flavor[i] += row[i];
			flavor[i] += column[i];
			flavor[i] += 800 * S*S;
			flavor[i] = flavor[i] * MOD + i;
		}
		sort(flavor.begin(), flavor.end());
		reverse(flavor.begin(), flavor.end());
		list<int>l;
		for (int i = 0; i < S; i++) {
			if (i % 2)l.push_back(flavor[i] % MOD);
			else l.push_front(flavor[i] % MOD);
		}
		vector<int> ret(l.begin(),l.end());
		return ret;
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