#include "Board.h"

Board copyBoard(Board orig, int length) {
    Board newBoard = {
        .units = new int[length],
        .rots = new int[length],
    };
    for (int i = 0; i < length; i++) {
        newBoard.units[i] = orig.units[i];
        newBoard.rots[i]  = orig.rots[i];
    }

    return newBoard;
};