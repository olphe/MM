#include "iostream"
#include "vector"
#include "algorithm"
#include "ctime"

using namespace std;

const int MOD = 1003;

int N, M, P, Q;
vector<vector<int>>edge;
vector<pair<int, int>>ans;
vector<int>flavor;

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
	Candidate(int n, int cy, int cx, bool ex) {
		num = n;
		up = 0;
		y = cy;
		x = cx;
		extend = ex;
	}
	bool operator <(const Candidate &c) const {
		if (up < c.up) {
			return true;
		}
		if (up > c.up) {
			return false;
		}
		if (extend && !c.extend) {
			return true;
		}
		else {
			return false;
		}
	}
	bool operator >(const Candidate &c) const {
		if (up > c.up) {
			return true;
		}
		if (up < c.up) {
			return false;
		}
		if (!extend && c.extend) {
			return true;
		}
		else {
			return false;
		}
	}
};

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

int dir_y[] = { 1,1,1,0,-1,-1,-1,0 };
int dir_x[] = { -1,0,1,1,1,0,-1,-1 };

int main() {
	XorShift xs;
	///**/FILE *ofile;
	///**/FILE *ifile;
	///**/ofile = fopen("problem1_toolkit_JP/scripts/result.out", "w");
	///**/ifile = fopen("problem1_toolkit_JP/scripts/testcase.in", "r");
	/**/scanf("%d %d", &N, &M);
	///**/fscanf(ifile, "%d %d", &N, &M);
	edge.resize(N, vector<int>(N));
	flavor.resize(N, 0);
	for (int i = 0; i < M; i++) {
		int a, b, c;
		/**/scanf("%d %d %d", &a, &b, &c);
		///**/fscanf(ifile, "%d %d %d", &a, &b, &c);
		a--;
		b--;
		edge[a][b] = c;
		edge[b][a] = c;
		flavor[a] += c;
		flavor[b] += c;
	}
	/**/scanf("%d %d", &P, &Q);
	///**/fscanf(ifile, "%d %d", &P, &Q);
	for (int i = 2; i <= 60; i++) {
		if (i*i == P) {
			P = i;
			break;
		}
	}
	for (int i = 0; i < Q; i++) {
		int a, b;
		/**/scanf("%d %d", &a, &b);
		///**/fscanf(ifile, "%d %d", &a, &b);
	}
	for (int i = 0; i < N; i++) {
		flavor[i] = flavor[i] * MOD + i;
	}
	sort(flavor.begin(), flavor.end());
	reverse(flavor.begin(), flavor.end());
	ans.resize(N, { -1,-1 });
	int score = 0;
	int lft, up;

	int loop_num = 0;
	int max_loop = 0;
	int bef = clock();

	for (auto cn : flavor) {
		loop_num++;
		int c_score = 0;
		vector<vector<int>>c_field(P * 2 + 1, vector<int>(P * 2 + 1, -1));
		vector<pair<int, int>>place(N, { -1,-1 });
		int c_lft = P, c_rht = P, c_up = P, c_bottom = P;
		vector<int>used;
		place[cn%MOD] = { P,P };
		c_field[P][P] = cn%MOD;
		used.push_back(cn%MOD);
		for (int loop = 1; loop < N; loop++) {
			Candidate c;
			for (int i = 0; i < N; i++) {
				if (place[i].first != -1) {
					continue;
				}
				for (auto j : used) {
					if (!edge[i][j]&&(c.num>0||(c.num==0&&c.extend))) {
						continue;
					}
					int cy = place[j].first;
					int cx = place[j].second;
					for (int k = 0; k < 8; k++) {
						int ny = cy + dir_y[k];
						int nx = cx + dir_x[k];
						if (abs(ny - c_up) >= P || abs(ny - c_bottom) >= P || abs(nx - c_lft) >= P || abs(nx - c_rht) >= P) {
							continue;
						}
						if (c_field[ny][nx] != -1) {
							continue;
						}
						Candidate c_box(i, ny, nx, c_up > ny || c_bottom<ny || c_lft > nx || c_rht < nx);
						for (int l = 0; l < 8; l++) {
							if (c_field[ny + dir_y[l]][nx + dir_x[l]] != -1) {
								c_box.up += edge[i][c_field[ny + dir_y[l]][nx + dir_x[l]]];
							}
						}
						c = max(c, c_box);
					}
				}
			}
			c_field[c.y][c.x] = c.num;
			place[c.num] = { c.y,c.x };
			used.push_back(c.num);
			c_lft = min(c_lft, c.x);
			c_rht = max(c_rht, c.x);
			c_up = min(c_up, c.y);
			c_bottom = max(c_bottom, c.y);
			c_score += c.up;
		}
		if (score < c_score) {
			ans = place;
			up = c_up;
			lft = c_lft;
			score = c_score;
		}
		int c_time = clock();
		max_loop = max(max_loop, c_time - bef);
		bef = c_time;
		///**/if (bef + max_loop > 9900) {
		///**/	break;
		///**/}
		/**/if (bef+max_loop > 9900000) {
		/**/	break;
		/**/}
	}
	bef = clock();
	max_loop = 0;
	vector<vector<int>>field(P * 2 + 1, vector<int>(P * 2 + 1, -1));
	for (int i = 0; i < N; i++) {
		field[ans[i].first][ans[i].second] = i;
	}
	///**/while (bef + max_loop < 9900) {
		/**/while (bef<9900000){
		for (int i = 0; i < 500000; i++) {
			int a, b;
			a = xs.rand() % N;
			b = xs.rand() % N;
			while (b == a) {
				b = xs.rand() % N;
			}
			int ay = ans[a].first;
			int ax = ans[a].second;
			int by = ans[b].first;
			int bx = ans[b].second;
			int dif = 0;
			for (int j = 0; j < 8; j++) {
				if (field[ay + dir_y[j]][ax + dir_x[j]] != -1) {
					dif += edge[field[ay][ax]][field[ay + dir_y[j]][ax + dir_x[j]]];
				}
				if (field[by + dir_y[j]][bx + dir_x[j]] != -1) {
					dif += edge[field[by][bx]][field[by + dir_y[j]][bx + dir_x[j]]];
				}
			}
			swap(field[ay][ax], field[by][bx]);
			for (int j = 0; j < 8; j++) {
				if (field[ay + dir_y[j]][ax + dir_x[j]] != -1) {
					dif -= edge[field[ay][ax]][field[ay + dir_y[j]][ax + dir_x[j]]];
				}
				if (field[by + dir_y[j]][bx + dir_x[j]] != -1) {
					dif -= edge[field[by][bx]][field[by + dir_y[j]][bx + dir_x[j]]];
				}
			}
			if (dif >= 0) {
				swap(field[ay][ax], field[by][bx]);
			}
			else {
				swap(ans[a], ans[b]);
			}
		}
		int c_time = clock();
		max_loop = max(max_loop, c_time - bef);
		bef = c_time;
	}
	///**/cout << N << " " << loop_num << " " << score << endl;
	for (int i = 0; i < N; i++) {
		/**/printf("%d %d\n", i + 1, (ans[i].first - up)*P + ans[i].second - lft + 1);
		///**/fprintf(ofile, "%d %d\n", i + 1, (ans[i].first - up)*P + ans[i].second - lft + 1);
	}
	///**/fclose(ofile);
	///**/fclose(ifile);
	return 0;
}