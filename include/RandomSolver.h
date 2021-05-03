#ifndef __RANDOM_SOLVER_H__
#define __RANDOM_SOLVER_H__

#include "ISolver.h"

class RandomSolver : public ISolver {
public:
    RandomSolver(IEvaluator* _eval, int _iters, int seed);
    int* solve(bool* mask, int w, int h);
private:
    IEvaluator* eval;
    int iters;
};

#endif //__RANDOM_SOLVER_H__