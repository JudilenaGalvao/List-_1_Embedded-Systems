#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

#define n 1000000

double calculatePI() {
    double factor = 1.0;
    double sum = 0.0;
    for (int i = 0; i < n; i++, factor = -factor) {
        sum += factor / (2 * i + 1);
    }
    return 4.0 * sum;
}

int main() {
    clock_t tstart, tend;
    tstart = clock();

    double pi = calculatePI();

    tend = clock();
    double trun = (double)(tend - tstart) / CLOCKS_PER_SEC;

    cout.precision(12);
    cout << fixed << "Estimated value of PI: " << pi << endl;
    cout << "Time: " << trun << " seconds" << endl;

    return 0;
}