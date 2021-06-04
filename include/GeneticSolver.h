#ifndef __GENETIC_SOLVER_H__
#define __GENETIC_SOLVER_H__

#include "ISolver.h"

class GeneticSolver : public ISolver {
public:
    GeneticSolver(int _popSize, int _nMutateStates, int* _mutationCounts, int* _mutationIters, int seed);
    Board solve(IEvaluator* eval, bool* mask, int w, int h);
private:
    int popSize;
    int nMutateStates;
    int* mutationCounts;
    int* mutationIters;
    Board randomBoard(bool* mask, int length, int unitCount);
};

#endif //__GENETIC_SOLVER_H__