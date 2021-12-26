#include <vector>
#include <cmath>
using namespace std;
/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled
*/
void correlate(int ny, int nx, const float *data, float *result) {
    vector<double> square_sum(ny, 0.0);
    vector<double> sum(ny, 0.0);
    vector<vector<double>> production(ny, vector<double> (ny, 0.0));

    for (int y = 0; y < ny; y ++) {
        for (int x = 0; x < nx; x ++) {
            sum[y] += (double)data[y*nx + x];
            square_sum[y] += (double)data[y*nx + x]*data[y*nx + x];
        }
    }

    for (int y_1 = 0; y_1 < ny; y_1 ++) {
        for (int y_2 = 0; y_2 < ny; y_2 ++) {
            if (y_2 < y_1) {
                production[y_1][y_2] = production[y_2][y_1];
            } else {
                for (int x = 0; x < nx; x ++)
                    production[y_1][y_2] += (double)data[y_1*nx + x] * (double)data[y_2*nx + x];
            }
        }
    }

    for (int y_1 = 0; y_1 < ny; y_1 ++) {
        for (int y_2 = 0; y_2 < ny; y_2 ++) {
            if (y_2 < y_1) {
                result[y_2*ny + y_1] = result[y_1*ny + y_2];
            } else {
                result[y_2*ny + y_1] = (nx*production[y_1][y_2] - sum[y_1]*sum[y_2])/
                sqrt((nx*square_sum[y_1] - sum[y_1]*sum[y_1])*(nx*square_sum[y_2] - sum[y_2]*sum[y_2]));
            }
        }
    }
}
