// C++11
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;
const long long int MOD = 1000000007;

class ConnectedComponent {
	
public:
	vector<int> permute(vector<int> matrix) {
		int S = (int)sqrt(matrix.size());
		vector<long long int>flavor(S,0);
		vector<long long int>row(S,0);
		vector<long long int>column(S,0);
		vector<int>ret;
		vector<bool>flag(S, true);
		while (ret.size() < S) {
			long long int box = 0;
			for (int i = 0; i < S; i++) {
				if (!flag[i])continue;
				long long int bag = 0;
				for (int j = 0; j < S; j++) {
					if (!flag[j]||i==j) {
						if (matrix[i*S + j]!=0) {
							bag += (matrix[i*S + j] + 10);
						}
						if (matrix[j*S + i]!=0) {
							bag += (matrix[j*S + i] + 10);
						}
					}
				}
				if (matrix[i*S + i]!=0) {
					bag -= (matrix[i*S + i] + 10);
				}
				if (box <= bag*MOD + i) {
					box = bag*MOD + i;
				}
			}
			ret.push_back(box%MOD);
			flag[box%MOD] = false;
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