# NguSolver

Ngu Solver is a program to investigate the optimal layout for maps within the game NGU Industries. The program is comprised to 2 main components: the solver and evaluator. These components are abstracted with the ISolver and IEvaluator classes.

A board is defined by a width and height, denoted w and h in the code, and is stored flattened as a 1D int array of size w*h. Each element in the array represents the ID of a building, defined by the Evaluator, 0 always being empty.

A mask is a 1D boolean array the same size as the board (w*h) where each element specifies whether a building can be placed on that tile or not.

The Evaluator is expected to implement:
 - double* batchEvaluate(int** board, int w, int h, int k): process k boards of size w*h, returning k evaluations.
 - double evaluate(int* board, int w, int h): process a single board of size w*h, returning a single evaluation of the board.
 - int getBuildingCount(): return the amount of unique placable buildings (including empty).
 - string buildingToString(int building): return the user-friendly string description of a building.

The Solver is expected to implement:
 - int* solve(IEvaluator* eval, bool* mask, int w, int h): return a single board of size w*h with a given mask and evaluator.

Note:
 - It is the Solver's responsability to abide by the rules of the mask. All indexes where the mask is false should be 0 (empty) on the board being passed to the evaluator and returned.
