#ifndef __VANILLA_EVALUATOR_H__
#define __VANILLA_EVALUATOR_H__

#include <pthread.h>

#include "IEvaluator.h"

struct ThreadData {
    int start;
    int stop;
    int w;
    int h;
    Board* boards;
    double* results;
    IEvaluator* eval;
};

class VanillaEvaluator : public IEvaluator {
private:
    pthread_t* threads;
    ThreadData* threadData;
    int threadCount;
    double* prodMult;
    void applyMult(double* mults, int i, int xOff, int yOff, int w, int h, int building);
public:
    VanillaEvaluator(int _threadCount);
    ~VanillaEvaluator();
    double* batchEvaluate(Board* boards, int w, int h, int k);
    static void* pthreadEvaluate(void* threadarg);
    double evaluate(Board board, int w, int h);
    enum class Units {
        Empty,
        Prod,
        Box,
        Knight,
        Arrow,
        Wall,
        UnitCount,
    };
    enum class Rotations {
        Up,
        Down,
        Left,
        Right,
        RotationCount,
    };
    int getUnitCount() { return (int)Units::UnitCount; };
    std::string unitToString(int unit, int rot);
};

#endif //__VANILLA_EVALUATOR_H__