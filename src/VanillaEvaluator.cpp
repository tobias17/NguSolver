#include "VanillaEvaluator.h"

#include <iostream>
using namespace std;

VanillaEvaluator::VanillaEvaluator() {
    prodMult = new double[(int)Buildings::BuildingCount];

    prodMult[(int)Buildings::Empty]  = 0.0;
    prodMult[(int)Buildings::Prod]   = 0.0;
    prodMult[(int)Buildings::Box]    = 0.3;
    prodMult[(int)Buildings::Knight] = 0.35;
    prodMult[(int)Buildings::Arrow]  = 0.22;
}

VanillaEvaluator::~VanillaEvaluator() {
    delete[] prodMult;
}

// evaluates k boards with size w x h
double* VanillaEvaluator::batchEvaluate(int** board, int w, int h, int k) {
    double* results = new double[k];
    for (int i = 0; i < k; i++) {
        results[i] = evaluate(board[i], w, h);
    }
    return results;
}

// evaluates board of size w x h
double VanillaEvaluator::evaluate(int* board, int w, int h) {
    int length = w * h;

    // initialize multipliers
    double* mults = new double[length];
    for (int i = 0; i < length; i++) {
        mults[i] = 1.0;
    }

    // calculate multipliers
    for (int i = 0; i < length; i++) {
        if (board[i] == (int)Buildings::Box) {
            // Box
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    applyMult(mults, i, x, y*w, length, (int)Buildings::Box);
                }
            }
        } else if (board[i] == (int)Buildings::Knight) {
            // Knight
            for (int x = -1; x <= 1; x += 2) {
                for (int y = -2; y <= 2; y += 4) {
                    applyMult(mults, i, x, y*w, length, (int)Buildings::Knight);
                }
            }
            for (int x = -2; x <= 2; x += 4) {
                for (int y = -1; y <= 1; y += 2) {
                    applyMult(mults, i, x, y*w, length, (int)Buildings::Knight);
                }
            }
        } else if (board[i] == (int)Buildings::Arrow) {
            // Arrow
            for (int y = -5; y <= -3; y++) {
                for (int x = -(5+y); x <= 5+y; x++) {
                    applyMult(mults, i, x, y*w, length, (int)Buildings::Arrow);
                }
            }
            applyMult(mults, i, 0, -2*w, length, (int)Buildings::Arrow);
            applyMult(mults, i, 0, -w, length, (int)Buildings::Arrow);
        }
    }

    // apply multipliers to production
    double production = 0;
    for (int i = 0; i < length; i++) {
        production += (board[i] == (int)Buildings::Prod) * mults[i];
    }
    
    delete[] mults;
    
    return production;
}

// applies a building mult with given index i and offsets x, y
void VanillaEvaluator::applyMult(double* mults, int i, int x, int y, int length, int building) {
    int index = i + x + y;
    if (index >= 0 && index < length) {
        mults[index] += prodMult[building];
    }
}

std::string VanillaEvaluator::buildingToString(int building) {
    switch (building) {
        case (int)Buildings::Empty:
            return "Empty";
        case (int)Buildings::Prod:
            return "Prod";
        case (int)Buildings::Box:
            return "Box";
        case (int)Buildings::Knight:
            return "Knight";
        case (int)Buildings::Arrow:
            return "Arrow";
        default:
            return "Unknown";
    }
}