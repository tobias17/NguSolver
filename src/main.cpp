#include <iostream>
#include <chrono>

#include "VanillaEvaluator.h"
#include "GeneticSolver.h"

#define NUM_THREADS 24

using namespace std;

int main() {
    IEvaluator* eval = new VanillaEvaluator(NUM_THREADS);

    int counts[5] = { 10, 5, 3, 2, 1 };
    int iters[5] = { 500, 300, 200, 200, 200 };
    ISolver* solver = new GeneticSolver(4000, 5, counts, iters, 500);

    int w = 20;
    int h = 17;

    bool mask[w*h] = {
        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0,
        1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0,
        1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
        1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
        1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
        1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
        1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
        1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
        1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    };

    auto t1 = chrono::high_resolution_clock::now();
    Board bestBoard = solver->solve(eval, mask, w, h);
    auto t2 = chrono::high_resolution_clock::now();

    cout << "Best Board:" << endl;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            cout << eval->unitToString(bestBoard.units[x+y*w], bestBoard.rots[x+y*w]) << "\t";
        }
        cout << endl;
    }
    cout << "Evaluated to: " << eval->evaluate(bestBoard, w, h) << endl;
    cout << "Time taken: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count()/1000.0 << "s" << endl;

    pthread_exit(NULL);
}