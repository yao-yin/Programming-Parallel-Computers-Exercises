#include <cmath>
#include <vector>
#include <stdlib.h>
#include <iostream>

using namespace std;
/*
This is the function you need to implement. Quick reference:
- input rows: 0 <= y < ny
- input columns: 0 <= x < nx
- element at row y and column x is stored in data[x + y*nx]
- correlation between rows i and row j has to be stored in result[i + j*ny]
- only parts with 0 <= j <= i < ny need to be filled
*/

typedef double double4_t __attribute__ ((vector_size (4 * sizeof(double))));
//typedef double double4_t __attribute__ ((ext_vector_type (4)));

static double4_t* double4_alloc(size_t n) {
    void* tmp = 0;
    if (posix_memalign(&tmp, sizeof(double4_t), sizeof(double4_t) * n)) {
        cout << "bad alloc" << endl;
        throw bad_alloc();
    }
    return (double4_t*) tmp;
}


void final_step(const vector<vector<double>> & data, float *result, int ny, int nx) {
    // store the date in float8
    int mx = (nx + 3) /4;
    double4_t* da = double4_alloc(ny*mx);
    
    for (int i = 0; i < ny; i ++) {
        for (int j = 0; j < mx; j ++) {
            for (int k = 0; k < 4; k ++) {
                if (j*4 + k < nx) {
                    da[i*mx + j][k] = data[i][j*4+k];
                } else {
                    da[i*mx + j][k] = 0.0;
                }
            }
        }
    }
    
    for (int y = 0; y < ny; y ++) {
        for (int x = y; x < ny; x ++) {
            // Calculation on row y and row x
            double4_t curr = {0.0, 0.0, 0.0, 0.0};
            for (int i = 0; i < mx; i ++) {
                curr += (da[x*mx + i] * da[y*mx + i]);
            }
            result[y*ny + x] = curr[0] + curr[1] + curr[2] + curr[3];
            //cout  << result[y*ny + x] << " ";
        }
        //cout << endl;
    }
    
    free(da);
}

void correlate(int ny, int nx, const float *data, float *result) {
    vector<vector<double>> zero_mean_data(ny, vector<double> (nx));
    vector<vector<double>> normalized_data(ny, vector<double> (nx));
    
    for (int y = 0; y < ny; y ++) {
        double row_avg = 0.0;
        for (int x = 0; x < nx; x ++) {
            row_avg += (double)data[y*nx + x];
        }
        row_avg /= nx;
        for (int x = 0; x < nx; x ++) {
            zero_mean_data[y][x] = (double)data[y*nx + x] - row_avg;
        }
    }
    
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
    
    final_step(normalized_data, result, ny, nx);
    
}
