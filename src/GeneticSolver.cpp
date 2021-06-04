#include <cstdlib>
#include <algorithm>
#include <iostream>

#include "GeneticSolver.h"
#include "RandomChoice.h"
#include "Board.h"

GeneticSolver::GeneticSolver(int _popSize, int _nMutateStates, int* _mutationCounts, int* _mutationIters, int seed) {
    popSize = _popSize;
    nMutateStates = _nMutateStates;
    mutationCounts = _mutationCounts;
    mutationIters = _mutationIters;
    srand(seed);
}

Board GeneticSolver::randomBoard(bool* mask, int length, int unitCount) {
    Board board {
        .units = new int[length],
        .rots = new int[length],
    };

    for (int index = 0; index < length; index++) {
        if (mask[index]) {
            board.units[index] = rand() % (unitCount - 1) + 1; // 1 upto unitCount
            board.rots[index] = rand() % 4;
        } else {
            board.units[index] = 0;
        }
    }

    return board;
}

Board GeneticSolver::solve(IEvaluator* eval, bool* mask, int w, int h) {
    Board* boards = new Board[popSize]; 
    int length = w*h;
    int ntake = popSize / 2;

    // create vector storing all mutatable indecies
    std::vector<int> mutatableIndecies;
    for (int i = 0; i < length; i++) {
        if (mask[i]) {
            mutatableIndecies.push_back(i);
        }
    }

    // create initial population
    for (int iter = 0; iter < popSize; iter++) {
        boards[iter] = randomBoard(mask, length, eval->getUnitCount());
    }

    // loop through all mutate states
    for (int mutateState = 0; mutateState < nMutateStates; mutateState++) {
        int mutationCount = mutationCounts[mutateState];
        std::cout << "Entering new state with mutation count " << mutationCount << ", " << mutationIters[mutateState] << " iters" << std::endl;
        for (int iter = 0; iter < mutationIters[mutateState]; iter++) {

            // evaluate population
            double* evals = eval->batchEvaluate(boards, w, h, popSize);

            // normalize to lowest value
            double minVal = evals[0];
            for (int i = 1; i < length; i++) {
                if (evals[i] < minVal) {
                    minVal = evals[i];
                }
            }
            for (int i = 0; i < length; i++) {
                evals[i] -= minVal;
            }

            // choose which boards to keep based on eval
            std::vector<int> choices = randomWeightedChoice(ntake, evals, popSize);
            // sort the choices
            std::sort(choices.begin(), choices.begin()+ntake);

            // delete all boards not chosen
            int ci = 0;
            Board* newBoards = new Board[popSize];
            for (int i = 0; i < popSize; i++) {
                if (i == choices[ci]) {
                    newBoards[ci] = boards[i];
                    ci++;
                } else {
                    delete[] boards[i].units;
                    delete[] boards[i].rots;
                }
            }
            delete[] boards;
            boards = newBoards;

            // fill in the empty spots with mutated copies
            for (int i = ntake; i < popSize; i++) {
                boards[i] = copyBoard(boards[i % ntake], length);

                for (int m = 0; m < mutationCount; m++) {
                    int mutateIndex = mutatableIndecies[rand() % mutatableIndecies.size()];
                    boards[i].units[mutateIndex] = rand() % (eval->getUnitCount() - 1) + 1;
                    boards[i].rots[mutateIndex]  = rand() % 4;
                }
            }

        }
    }

    // find and return the best board
    double* evals = eval->batchEvaluate(boards, w, h, popSize);

    double bestEval = 0;
    Board bestBoard = {
        .units = nullptr,
        .rots = nullptr,
    };
    for (int i = 0; i < popSize; i++) {
        if (evals[i] > bestEval) {
            if (bestBoard.units != nullptr) {
                delete[] bestBoard.units;
                delete[] bestBoard.rots;
            }
            bestEval = evals[i];
            bestBoard = boards[i];
        } else {
            delete[] boards[i].units;
            delete[] boards[i].rots;
        }
    }
    delete[] boards;

    return bestBoard;
}