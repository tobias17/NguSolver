#include <cstdlib>

#include "RandomSolver.h"

RandomSolver::RandomSolver(IEvaluator* _eval, int _iters, int seed) {
    eval = _eval;
    iters = _iters;
    srand(seed);
}

int* RandomSolver::solve(bool* mask, int w, int h) {
    int** boards = new int*[iters];
    int length = w*h;

    for (int iter = 0; iter < iters; iter++) {
        boards[iter] = new int[length];
        for (int index = 0; index < length; index++) {
            if (mask[index]) {
                boards[iter][index] = rand() % (eval->getBuildingCount()-1) + 1; // 1 upto buildingCount
            } else {
                boards[iter][index] = 0;
            }
        }
    }

    double* evals = eval->batchEvaluate(boards, w, h, iters);

    double bestEval = 0;
    int* bestBoard = nullptr;
    for (int i = 0; i < iters; i++) {
        if (evals[i] > bestEval) {
            if (bestBoard != nullptr) {
                delete[] bestBoard;
            }
            bestEval = evals[i];
            bestBoard = boards[i];
        } else {
            delete[] boards[i];
        }
    }
    delete[] boards;

    return bestBoard;
}