#include <iostream>

#include "VanillaEvaluator.h"

using namespace std;

VanillaEvaluator::VanillaEvaluator(int _threadCount) {
    prodMult = new double[(int)Units::UnitCount];

    prodMult[(int)Units::Empty]  = 0.00;
    prodMult[(int)Units::Prod]   = 0.00;
    prodMult[(int)Units::Box]    = 0.30;
    prodMult[(int)Units::Knight] = 0.35;
    prodMult[(int)Units::Arrow]  = 0.22;
    prodMult[(int)Units::Wall]   = 0.27;

    threads = new pthread_t[_threadCount];
    threadData = new ThreadData[_threadCount];
    threadCount = _threadCount;
}

VanillaEvaluator::~VanillaEvaluator() {
    delete[] prodMult;
    delete[] threads;
    delete[] threadData;
}

// evaluates k boards with size w x h
double* VanillaEvaluator::batchEvaluate(Board* boards, int w, int h, int k) {
    double* results = new double[k];
    int rc;
    
    for (int i = 0; i < threadCount; i++) {
        threadData[i].start = (int)(i*((double)k/threadCount));
        threadData[i].stop = (int)((i+1)*((double)k/threadCount));
        threadData[i].w = w;
        threadData[i].h = h;
        threadData[i].boards = boards;
        threadData[i].results = results;
        threadData[i].eval = this;

        rc = pthread_create(&threads[i], NULL, VanillaEvaluator::pthreadEvaluate, (void*)&threadData[i]);

        if (rc) {
            cout << "Error in creating thread, " << rc << endl;
            exit(-1);
        }
    }

    for (int i = 0; i < threadCount; i++) {
        rc = pthread_join(threads[i], NULL);
        if (rc) {
            cout << "Error in joining thread, " << rc << endl;
            exit(-1);
        }
    }

    return results;
}

// evaluates boards from within a pthread
void* VanillaEvaluator::pthreadEvaluate(void* threadarg) {
    ThreadData data = *((ThreadData*)threadarg);

    for (int i = data.start; i < data.stop; i++) {
        data.results[i] = data.eval->evaluate(data.boards[i], data.w, data.h);
    }

    pthread_exit(NULL);
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
                    applyMult(mults, i, x, y, w, h, (int)Units::Box);
                }
            }
        } else if (board.units[i] == (int)Units::Knight) {
            // Knight
            for (int x = -1; x <= 1; x += 2) {
                for (int y = -2; y <= 2; y += 4) {
                    applyMult(mults, i, x, y, w, h, (int)Units::Knight);
                }
            }
            for (int x = -2; x <= 2; x += 4) {
                for (int y = -1; y <= 1; y += 2) {
                    applyMult(mults, i, x, y, w, h, (int)Units::Knight);
                }
            }
        } else if (board.units[i] == (int)Units::Arrow) {
            // Arrow
            if (board.rots[i] == (int)Rotations::Up) {
                // Up
                for (int y = -5; y <= -3; y++) {
                    for (int x = -(5+y); x <= 5+y; x++) {
                        applyMult(mults, i, x, y, w, h, (int)Units::Arrow);
                    }
                }
                applyMult(mults, i, 0, -2, w, h, (int)Units::Arrow);
                applyMult(mults, i, 0, -1, w, h, (int)Units::Arrow);
            } else if (board.rots[i] == (int)Rotations::Down) {
                // Down
                for (int y = 5; y >= 3; y--) {
                    for (int x = y-5; x <= 5-y; x++) {
                        applyMult(mults, i, x, y, w, h, (int)Units::Arrow);
                    }
                }
                applyMult(mults, i, 0, 2, w, h, (int)Units::Arrow);
                applyMult(mults, i, 0, 1, w, h, (int)Units::Arrow);
            } else if (board.rots[i] == (int)Rotations::Left) {
                // Left
                for (int x = -5; x <= -3; x++) {
                    for (int y = -(5+x); y <= 5+x; y++) {
                        applyMult(mults, i, x, y, w, h, (int)Units::Arrow);
                    }
                }
                applyMult(mults, i, -2, 0, w, h, (int)Units::Arrow);
                applyMult(mults, i, -1, 0, w, h, (int)Units::Arrow);
            } else if (board.rots[i] == (int)Rotations::Right) {
                // Right
                for (int x = 5; x >= 3; x--) {
                    for (int y = x-5; y <= 5-x; y++) {
                        applyMult(mults, i, x, y, w, h, (int)Units::Arrow);
                    }
                }
                applyMult(mults, i, 2, 0, w, h, (int)Units::Arrow);
                applyMult(mults, i, 1, 0, w, h, (int)Units::Arrow);
            }
        } else if (board.units[i] == (int)Units::Wall) {
            // Wall
            if (board.rots[i] == (int)Rotations::Up || board.rots[i] == (int)Rotations::Down) {
                // Vertical
                for (int y = -5; y <= 5; y++) {
                    applyMult(mults, i, 0, y, w ,h, (int)Units::Wall);
                }
            } else {
                // Horizontal
                for (int x = -5; x <= 5; x++) {
                    applyMult(mults, i, x, 0, w, h, (int)Units::Wall);
                }
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
void VanillaEvaluator::applyMult(double* mults, int i, int xOff, int yOff, int w, int h, int building) {
    int x = (i % w) + xOff;
    int y = (int)(i / w) + yOff;
    if (x >= 0 && x < w && y >= 0 && y < h) {
        mults[x + y*w] += prodMult[building];
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
        case (int)Units::Wall:
            switch (rot) {
                case (int)Rotations::Up:
                case (int)Rotations::Down:
                    return "Wall|";
                case (int)Rotations::Left:
                case (int)Rotations::Right:
                    return "Wall-";
            }
        default:
            return "Unknown";
    }
}