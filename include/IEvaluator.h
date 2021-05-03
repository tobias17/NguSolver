#ifndef __IEVALUATOR_H__
#define __IEVALUATOR_H__

#include <string>

class IEvaluator {
public:
    enum class Buildings;
    virtual double* batchEvaluate(int** board, int w, int h, int k) = 0;
    virtual double evaluate(int* board, int w, int h) = 0;
    virtual int getBuildingCount() = 0;
    virtual std::string buildingToString(int building) = 0;
};

#endif //__IEVALUATOR_H__