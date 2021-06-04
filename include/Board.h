#ifndef __BOARD_H__
#define __BOARD_H__

struct Board {
    int* units = nullptr;
    int* rots = nullptr;
};

Board copyBoard(Board orig, int length);

#endif //__BOARD_H__