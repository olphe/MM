// C++11
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>

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

class Candidate {
public:
	int y;
	int x;
	int r;
	int color;
	int score;
	Candidate() {
		y = 0;
		x = 0;
		r = 0;
		color = 0;
		score = 0;
		return;
	}
	bool operator < (const Candidate& c) const {
		return score < c.score;
	}

	bool operator > (const Candidate& c) const {
		return score > c.score;
	}
	bool operator == (const Candidate& c) const {
		return score == c.score;
	}
};

class CirclesMix {
public:
	vector<int> ret;
	void add(vector<int> t) {
		ret.insert(ret.end(), t.begin(), t.end());
	}
	int Red(int c) {
		return (c >> 16);
	}
	int Green(int c) {
		return (c >> 8) & 0xff;
	}
	int Blue(int c) {
		return c & 0xff;
	}
	vector<int> drawImage(int H, vector<int> pixels, int N) {
		int W = pixels.size() / H;
		int div = min(H, W) / sqrt(N*min(H, W) / max(H, W) / 5);
		vector<vector<pair<int, int>>>search(div + 1);
		vector<int>sumsum(div + 1);
		vector<int> C(pixels.size(), 0x000000);
		for (int i = -div; i <= div; i++) {
			for (int j = -div; j <= div; j++) {
				if ((int)(sqrt(i*i + j*j)) > div) {
					continue;
				}
				search[(int)sqrt(i*i + j*j)].push_back({ i,j });
			}
		}
		sumsum[0] = search[0].size();
		for (int i = 1; i <= div; i++) {
			sumsum[i] = sumsum[i - 1] + search[i].size();
		}
		XorShift xs;
		long double m_loop = 0;
		long long int looop = 0;
		for (int loop = 0; loop < N; loop++) {
			Candidate box;
			m_loop += (long double)1250000000 / N;
			for (; looop < m_loop; ) {
				int y, x;
				y = xs.rand() % H;
				x = xs.rand() % W;
				int r = div*(N - loop - 1) / N + xs.rand() % (xs.rand() % 18 + 5);
				r = min(r, div);
				looop += sumsum[r];
				int red = 0, blue = 0, green = 0;
				int px = 0;
				Candidate candidate;
				for (int radius = 0; radius <= r; radius++) {
					for (auto i : search[radius]) {
						int cy = y + i.first;
						int cx = x + i.second;
						if (cy<0 || cy >= H || cx<0 || cx >= W) {
							continue;
						}
						px++;
						red += Red(pixels[cy*W + cx]) * 2 - Red(C[cy*W + cx]);
						green += Green(pixels[cy*W + cx]) * 2 - Green(C[cy*W + cx]);
						blue += Blue(pixels[cy*W + cx]) * 2 - Blue(C[cy*W + cx]);
						candidate.score -= abs(Red(C[cy*W + cx]) - Red(pixels[cy*W + cx]));
						candidate.score -= abs(Green(C[cy*W + cx]) - Green(pixels[cy*W + cx]));
						candidate.score -= abs(Blue(C[cy*W + cx]) - Blue(pixels[cy*W + cx]));
					}
				}
				red = red / px + !!(red%px);
				green = green / px + !!(green%px);
				blue = blue / px + !!(blue%px);
				red = max(0, min(255, red));
				green = max(0, min(255, green));
				blue = max(0, min(255, blue));
				int color = (red << 16) + (green << 8) + blue;
				candidate.x = x;
				candidate.y = y;
				candidate.r = r;
				candidate.color = color;
				for (int radius = 0; radius <= r; radius++) {
					for (auto i : search[radius]) {
						int cy = y + i.first;
						int cx = x + i.second;
						if (cy<0 || cy >= H || cx<0 || cx >= W) {
							continue;
						}
						red = Red(color) + Red(C[cy*W + cx]);
						red /= 2;
						green = Green(color) + Green(C[cy*W + cx]);
						green /= 2;
						blue = Blue(color) + Blue(C[cy*W + cx]);
						blue /= 2;
						candidate.score += abs(red - Red(pixels[cy*W + cx]));
						candidate.score += abs(green - Green(pixels[cy*W + cx]));
						candidate.score += abs(blue - Blue(pixels[cy*W + cx]));
					}
				}
				if (box.score > candidate.score) {
					box = candidate;
				}
			}
			add({ box.y,box.x,box.r,box.color });
			for (int radius = 0; radius <= box.r; radius++) {
				for (auto i : search[radius]) {
					int cy = box.y + i.first;
					int cx = box.x + i.second;
					if (cy<0 || cy >= H || cx<0 || cx >= W) {
						continue;
					}
					int red, blue, green;
					red = Red(box.color) + Red(C[cy*W + cx]);
					red /= 2;
					green = Green(box.color) + Green(C[cy*W + cx]);
					green /= 2;
					blue = Blue(box.color) + Blue(C[cy*W + cx]);
					blue /= 2;
					C[cy*W + cx] = (red << 16) + (green << 8) + blue;
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
	CirclesMix cm;
	int H;
	cin >> H;
	int S;
	cin >> S;
	vector<int> pixels(S);
	getVector(pixels);

	int N;
	cin >> N;

	vector<int> ret = cm.drawImage(H, pixels, N);
	cout << ret.size() << endl;
	for (int i = 0; i < (int)ret.size(); ++i)
		cout << ret[i] << endl;
	cout.flush();
}