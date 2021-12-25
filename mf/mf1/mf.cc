#include <vector>
#include <algorithm>
using namespace std;
/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in in[x + y*nx]
- for each pixel (x, y), store the median of the pixels (a, b) which satisfy
  max(x-hx, 0) <= a < min(x+hx+1, nx), max(y-hy, 0) <= b < min(y+hy+1, ny)
  in out[x + y*nx].
*/
double get_median(vector<double> & v, size_t tot) {
  if (tot & 1) {
    nth_element(v.begin(), v.begin() + tot/2, v.begin() + tot);
    return v[tot/2];
  }
  nth_element(v.begin(), v.begin() + tot/2, v.begin() + tot);
  double p1 = v[tot/2];
  nth_element(v.begin(), v.begin() + tot/2 - 1, v.begin() + tot);
  double p2 = v[tot/2 - 1];
  return (p1 + p2)/2;
}

void mf(int ny, int nx, int hy, int hx, const float *in, float *out) {
  int lb{0}, rb{0}, ub{0}, db{0}, idx{0};
  double median = 0.0;
  vector<double> v((2*hx+1)*(2*hy+1), 0.0);
  for (int y = 0; y < ny; y ++) {
    for (int x = 0; x < nx; x ++) {
      ub = std::max(y-hy, 0);
      db = std::min(y+hy+1, ny);
      lb = std::max(x-hx, 0);
      rb = std::min(x+hx+1, nx);
      idx = 0;
      for (int ry = ub; ry < db; ry ++) {
        for (int rx = lb; rx < rb; rx ++) {
          v[idx ++] = *(in + ry*nx + rx);
        }
      }
      median = get_median(v, idx);
      *(out + y*nx + x) = median; 
    }
  }
}