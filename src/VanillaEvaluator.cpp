#include "VanillaEvaluator.h"

#include <iostream>
using namespace std;

VanillaEvaluator::VanillaEvaluator() {
    prodMult = new double[(int)Units::UnitCount];

    prodMult[(int)Units::Empty]  = 0.0;
    prodMult[(int)Units::Prod]   = 0.0;
    prodMult[(int)Units::Box]    = 0.3;
    prodMult[(int)Units::Knight] = 0.35;
    prodMult[(int)Units::Arrow]  = 0.22;
}

VanillaEvaluator::~VanillaEvaluator() {
    delete[] prodMult;
}

// evaluates k boards with size w x h
double* VanillaEvaluator::batchEvaluate(Board* board, int w, int h, int k) {
    double* results = new double[k];
    for (int i = 0; i < k; i++) {
        results[i] = evaluate(board[i], w, h);
    }
    return results;
}

// evaluates board of size w x h
double VanillaEvaluator::evaluate(Board board, int w, int h) {
    int length = w * h;

    // initialize multipliers
    double* mults = new double[length];
    for (int i = 0; i < length; i++) {
        mults[i] = 1.0;
    }

    // calculate multipliers
    for (int i = 0; i < length; i++) {
        if (board.units[i] == (int)Units::Box) {
            // Box
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    applyMult(mults, i, x, y*w, length, (int)Units::Box);
                }
            }
        } else if (board.units[i] == (int)Units::Knight) {
            // Knight
            for (int x = -1; x <= 1; x += 2) {
                for (int y = -2; y <= 2; y += 4) {
                    applyMult(mults, i, x, y*w, length, (int)Units::Knight);
                }
            }
            for (int x = -2; x <= 2; x += 4) {
                for (int y = -1; y <= 1; y += 2) {
                    applyMult(mults, i, x, y*w, length, (int)Units::Knight);
                }
            }
        } else if (board.units[i] == (int)Units::Arrow) {
            // Arrow
            if (board.rots[i] == (int)Rotations::Up) {
                // Up
                for (int y = -5; y <= -3; y++) {
                    for (int x = -(5+y); x <= 5+y; x++) {
                        applyMult(mults, i, x, y*w, length, (int)Units::Arrow);
                    }
                }
                applyMult(mults, i, 0, -2*w, length, (int)Units::Arrow);
                applyMult(mults, i, 0, -w, length, (int)Units::Arrow);
            }
            else if (board.rots[i] == (int)Rotations::Down) {
                // Down
                for (int y = 5; y >= 3; y--) {
                    for (int x = -(5+y); x <= 5+y; x++) {
                        applyMult(mults, i, x, y*w, length, (int)Units::Arrow);
                    }
                }
                applyMult(mults, i, 0, 2*w, length, (int)Units::Arrow);
                applyMult(mults, i, 0, w, length, (int)Units::Arrow);
            }
            else if (board.rots[i] == (int)Rotations::Left) {
                // Left
                for (int x = -5; x <= -3; x++) {
                    for (int y = -(5+x); y <= 5+x; y++) {
                        applyMult(mults, i, x, y*w, length, (int)Units::Arrow);
                    }
                }
                applyMult(mults, i, -2*w, 0, length, (int)Units::Arrow);
                applyMult(mults, i, -w, 0, length, (int)Units::Arrow);
            }
            else if (board.rots[i] == (int)Rotations::Right) {
                // Right
                for (int x = 5; x >= 3; x--) {
                    for (int y = -(5+x); y <= 5+x; y++) {
                        applyMult(mults, i, x, y*w, length, (int)Units::Arrow);
                    }
                }
                applyMult(mults, i, 2*w, 0, length, (int)Units::Arrow);
                applyMult(mults, i, w, 0, length, (int)Units::Arrow);
            }
        }
    }

    // apply multipliers to production
    double production = 0;
    for (int i = 0; i < length; i++) {
        production += (board.units[i] == (int)Units::Prod) * mults[i];
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

std::string VanillaEvaluator::unitToString(int unit, int rot) {
    switch (unit) {
        case (int)Units::Empty:
            return "Empty";
        case (int)Units::Prod:
            return "Prod";
        case (int)Units::Box:
            return "Box";
        case (int)Units::Knight:
            return "Knight";
        case (int)Units::Arrow:
            switch (rot) {
                case (int)Rotations::Up:
                    return "Arrow^";
                case (int)Rotations::Down:
                    return "Arrow∨";
                case (int)Rotations::Left:
                    return "Arrow<";
                case (int)Rotations::Right:
                    return "Arrow>";
            }
        default:
            return "Unknown";
    }
}