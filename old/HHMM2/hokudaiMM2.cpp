#include "iostream"
#include "vector"
#include "set"
#include "algorithm"
#include "ctime"

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

class CCoordinate {
public:
	int y;
	int x;
};

int main() {
	XorShift xs;
	int N, M, P, Q;
	int TL_rate;
	/**/FILE *ifile;
	/**/FILE *ofile;
	/**/ifile = fopen("problem2_toolkit_JP/scripts/testcase.in", "r");
	/**/ofile = fopen("problem2_toolkit_JP/scripts/result.out", "w");
	/**/fscanf(ifile, "%d %d", &N, &M);
	/**/TL_rate = 1;
	//**/TL_rate = 1000;
	//**/scanf("%d %d", &N, &M);
	vector<vector<int>>edge(N);
	vector<pair<int, int>>all_edge(M);
	for (int i = 0; i < M; i++) {
		int a, b;
		/**/fscanf(ifile, "%d %d", &a, &b);
		//**/scanf("%d %d", &a, &b);
		a--;
		b--;
		edge[a].push_back(b);
		edge[b].push_back(a);
		all_edge[i] = { a,b };
	}
	/**/fscanf(ifile, "%d %d", &P, &Q);
	//**/scanf("%d %d", &P, &Q);
	for (int i = 2; i <= 60; i++) {
		if (i*i == P) {
			P = i;
			break;
		}
	}
	for (int i = 0; i < Q; i++) {
		int a, b;
		/**/fscanf(ifile, "%d %d", &a, &b);
		//**/scanf("%d %d", &a, &b);
	}
	cout << N << " " << M << " " << P << endl;
	vector<int>node(N, 1);
	int sum_weight = M;
	sum_weight *= 2;
	int plane = P*P - N;
	if (sum_weight) {
		for (int i = 0; i < N; i++) {
			node[i] += (P*P - N)*edge[i].size() / sum_weight;
			plane -= (P*P - N)*edge[i].size() / sum_weight;
		}
	}
	while (plane) {
		plane--;
		node[xs.rand() % N]++;
	}
	vector<vector<int>>field(P, vector<int>(P, -1));
	vector<vector<CCoordinate>>place(N);
	for (int i = 0; i < N; i++) {
		int sy = xs.rand() % P;
		int sx = xs.rand() % P;
		while (field[sy][sx] != -1) {
			sy = xs.rand() % P;
			sx = xs.rand() % P;
		}
		field[sy][sx] = i;
		place[i].push_back({ sy,sx });
	}
	int dir_y[] = { 1,1,-1,-1,-1,0,1,0 };
	int dir_x[] = { -1,1,1,-1,0,1,0,-1 };
	vector<int>perm(N);
	for (int i = 0; i < N; i++) {
		 perm[i] = i;
	}
	for (int i = 0; i < N * 2; i++) {
		int a, b;
		a = xs.rand() % N;
		b = xs.rand() % N;
		swap(perm[a], perm[b]);
	}
	for (auto i:perm) {
		int index = 0;
		int b = 8;
		int cy = place[i][index].y;
		int cx = place[i][index].x;
		int check = 0;
		for (int j = 1; j < node[i]; j++) {
			if (b < 4) {
				int ny = cy + dir_y[b];
				int nx = cx + dir_x[b];
				if (ny >= 0 && ny < P && nx >= 0 && nx < P) {
					if (field[ny][nx] == -1) {
						field[ny][nx] = i;
						cy = ny;
						cx = nx;
						continue;
					}
				}
			}
			int ny = 1000;
			int nx = 1000;
			for (int k = 0; k < 4; k++) {
				int nny = cy + dir_y[k];
				int nnx = cx + dir_x[k];
				if (nny < 0 || nny >= P || nnx < 0 || nnx >= P) {
					continue;
				}
				if (field[nny][nnx] == -1) {
					if (abs(place[i][index].y - ny) + abs(place[i][index].x - nx) > abs(place[i][index].y - nny) + abs(place[i][index].x - nnx)) {
						ny = nny;
						nx = nnx;
						b = k;
					}
				}
			}
			if (ny == 1000) {
				if (check == 4) {
					check = -1;
					if (index + 1 < place[i].size()) {
						index++;
					}
					else {
						break;
					}
				}
				cy = place[i][index].y;
				cx = place[i][index].x;
				b = 8;
				check++;
				j--;
			}
			else {
				field[ny][nx] = i;
				place[i].push_back({ ny,nx });
				cy = ny;
				cx = nx;
			}
		}
	}
	while (clock() < 29900 * TL_rate) {
		int y = xs.rand();
		int x = xs.rand();

	}
	int placed = 0;
	vector<vector<int>>coordinate(N);
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < P; j++) {
			if (field[i][j] >= 0) {
				coordinate[field[i][j]].push_back(i*P + j + 1);
				placed++;
			}
		}
	}
	for (int i = 0; i < N; i++) {
		/**/fprintf(ofile, "%d", coordinate[i].size());
		//**/printf("%d", coordinate[i].size());
		for (auto j : coordinate[i]) {
			/**/fprintf(ofile, " %d", j);
			//**/printf(" %d", j);
		}
		/**/fprintf(ofile, "\n");
		//**/printf("\n");
	}
	/**/fclose(ifile);
	/**/fclose(ofile);
	return 0;
}