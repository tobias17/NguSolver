#ifndef __VANILLA_EVALUATOR_H__
#define __VANILLA_EVALUATOR_H__

#include "IEvaluator.h"

class VanillaEvaluator : public IEvaluator {
private:
    double* prodMult;
    void applyMult(double* mults, int i, int x, int y, int length, int building);
public:
    VanillaEvaluator();
    ~VanillaEvaluator();
    double* batchEvaluate(Board* board, int w, int h, int k);
    double evaluate(Board board, int w, int h);
    enum class Units {
        Empty,
        Prod,
        Box,
        Knight,
        Arrow,
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