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
void correlate(const int ny, const int nx, const float *data, float *result) {
    vector<vector<double>> zero_mean_data(ny, vector<double> (nx));
    vector<vector<double>> normalized_data(ny, vector<double> (nx));
    
    #pragma omp parallel for
    for (int y = 0; y < ny; y ++) {
        double row_avg = 0.0;
        for (int x = 0; x < nx; x ++) {
            row_avg += data[y*nx + x];
        }
        row_avg /= nx;
        for (int x = 0; x < nx; x ++) {
            zero_mean_data[y][x] = data[y*nx + x] - row_avg;
        }
    }
    
    #pragma omp parallel for
    for (int y = 0; y < ny; y ++) {
        double sum = 0.0;
        for (int x = 0; x < nx; x ++) {
            sum += zero_mean_data[y][x] * zero_mean_data[y][x];
        }
        sum = sqrt(sum);
        for (int x = 0; x < nx; x ++) {
            normalized_data[y][x] = zero_mean_data[y][x] / sum;
        }
    }
    
    #pragma omp parallel for
    for (int y = 0; y < ny; y ++) {
        for (int x = y; x < ny; x ++) {
            double res = 0.0;
            for (int i = 0; i < nx; i ++) 
                res += normalized_data[y][i]*normalized_data[x][i];
            result[y*ny + x] = res;
        }
    }
}
