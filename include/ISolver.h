#ifndef __ISOLVER_H__
#define __ISOLVER_H__

#include "IEvaluator.h"

class ISolver {
public:
    virtual Board solve(IEvaluator* eval, bool* mask, int w, int h) = 0;
};

#endif //__ISOLVER_H__