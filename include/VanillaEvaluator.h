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
    double* batchEvaluate(int** board, int w, int h, int k);
    double evaluate(int* board, int w, int h);
    enum class Buildings {
        Empty,
        Prod,
        Box,
        Knight,
        Arrow,
        BuildingCount,
    };
    int getBuildingCount() { return (int)Buildings::BuildingCount; };
    std::string buildingToString(int building);
};

#endif //__VANILLA_EVALUATOR_H__