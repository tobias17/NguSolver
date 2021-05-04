#include <cstdlib>

#include "RandomSolver.h"
#include "Board.h"

RandomSolver::RandomSolver(int _iters, int seed) {
    iters = _iters;
    srand(seed);
}

Board RandomSolver::solve(IEvaluator* eval, bool* mask, int w, int h) {
    Board* boards = new Board[iters];
    int length = w*h;

    for (int iter = 0; iter < iters; iter++) {
        boards[iter].units = new int[length];
        boards[iter].rots = new int[length];
        for (int index = 0; index < length; index++) {
            if (mask[index]) {
                boards[iter].units[index] = rand() % (eval->getUnitCount()-1) + 1;; // 1 upto buildingCount
                boards[iter].rots[index] = rand() % 4;
            } else {
                boards[iter].units[index] = 0;
            }
        }
    }

    double* evals = eval->batchEvaluate(boards, w, h, iters);

    double bestEval = 0;
    Board bestBoard = {
        .units = nullptr,
        .rots = nullptr,
    };
    for (int i = 0; i < iters; i++) {
        if (evals[i] > bestEval) {
            if (bestBoard.units != nullptr) {
                delete[] bestBoard.units;
                delete[] bestBoard.rots;
            }
            bestEval = evals[i];
            bestBoard = boards[i];
        } else {
            delete[] boards[i].units;
            delete[] boards[i].rots;
        }
    }
    delete[] boards;

    return bestBoard;
}