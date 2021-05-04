#ifndef __IEVALUATOR_H__
#define __IEVALUATOR_H__

#include <string>

#include "Board.h"

class IEvaluator {
public:
    enum class Buildings;
    virtual double* batchEvaluate(Board* board, int w, int h, int k) = 0;
    virtual double evaluate(Board board, int w, int h) = 0;
    virtual int getUnitCount() = 0;
    virtual std::string unitToString(int unit, int rot) = 0;
};

#endif //__IEVALUATOR_H__