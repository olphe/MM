#include "iostream"
#include "vector"
#include "algorithm"

using namespace std;

int N, M, P, Q;
vector<vector<int>>edge;
vector<vector<int>>field;

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

class Candidate {
public:
	int num;
	int up;
	int y;
	int x;
	bool extend;
	Candidate() {
		num = -1;
		up = -1;
		y = -1;
		x = -1;
		extend = false;
	}
	Candidate(int n, int cy, int cx,bool ex) {
		num = n;
		up = 0;
		y = cy;
		x = cx;
		extend = ex;
	}
	bool operator <(const Candidate &c) const{
		return (up < c.up || (up == c.up&&extend && !c.extend));
	}
	bool operator >(const Candidate &c) const{
		return (up > c.up || (up == c.up && !extend && c.extend));
	}
};

int dir_y[] = { 1,1,1,0,-1,-1,-1,0 };
int dir_x[] = { -1,0,1,1,1,0,-1,-1 };

int main() {
	XorShift xs;
	/**/FILE *ofile;
	/**/FILE *ifile;
	/**/ofile = fopen("problem1_toolkit_JP/scripts/result.out", "w");
	/**/ifile = fopen("problem1_toolkit_JP/scripts/testcase.in", "r");
	///**/cin >> N >> M;
	/**/fscanf(ifile, "%d %d", &N, &M);
	edge.resize(N, vector<int>(N));
	field.resize(200, vector<int>(200, -1));
	for (int i = 0; i < M; i++) {
		int a, b, c;
		///**/cin >> a >> b >> c;
		/**/fscanf(ifile, "%d %d %d", &a, &b, &c);
		a--;
		b--;
		edge[a][b] = c;
		edge[b][a] = c;
	}
	///**/cin >> P >> Q;
	/**/fscanf(ifile, "%d %d", &P, &Q);
	for (int i = 2; i <= 60; i++) {
		if (i*i == P) {
			P = i;
			break;
		}
	}
	for (int i = 0; i < Q; i++) {
		int a, b;
		///**/cin >> a >> b;
		/**/fscanf(ifile, "%d %d", &a, &b);
	}


	vector<pair<int, int>>place(N, { -1,-1 });

	int lft = 100, rht = 100, up = 100, bottom = 100;
	vector<int>used;
	place[0] = { 100,100 };
	field[100][100] = 0;
	used.push_back(0);
	for (int loop = 1; loop < N; loop++) {
		Candidate c;
		for (int i = 0; i < N; i++) {
			if (place[i].first != -1) {
				continue;
			}
			for (auto j : used) {
				int cy = place[j].first;
				int cx = place[j].second;
				for (int k = 0; k < 8; k++) {
					int ny = cy + dir_y[k];
					int nx = cx + dir_x[k];
					if (abs(ny - up) >= P || abs(ny - bottom) >= P || abs(nx - lft) >= P || abs(nx - rht) >= P) {
						continue;
					}
					if (field[ny][nx] != -1) {
						continue;
					}
					Candidate c_box(i, ny, nx, up>ny || bottom<ny || lft > nx || rht < nx);
					for (int l = 0; l < 8; l++) {
						if (field[ny + dir_y[l]][nx + dir_x[l]] != -1) {
							c_box.up += edge[i][field[ny + dir_y[l]][nx + dir_x[l]]];
						}
					}
					c = max(c, c_box);
				}
			}
		}
		field[c.y][c.x] = c.num;
		place[c.num] = { c.y,c.x };
		used.push_back(c.num);
		lft = min(lft, c.x);
		rht = max(rht, c.x);
		up = min(up, c.y);
		bottom = max(bottom, c.y);
	}
	for (int i = 0; i < N; i++) {
		///**/ cout << i + 1 << " " << (place[i].first - up)*P + place[i].second - lft + 1 << endl;
		/**/fprintf(ofile, "%d %d\n", i + 1, (place[i].first - up)*P + place[i].second - lft + 1);
	}
	/**/fclose(ofile);
	return 0;
}