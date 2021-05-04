#ifndef __RANDOM_SOLVER_H__
#define __RANDOM_SOLVER_H__

#include "ISolver.h"

class RandomSolver : public ISolver {
public:
    RandomSolver(int _iters, int seed);
    Board solve(IEvaluator* eval, bool* mask, int w, int h);
private:
    int iters;
};

#endif //__RANDOM_SOLVER_H__