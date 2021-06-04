#include <vector>
#include <random>
#include <cmath>
#define pow2(n) ( 1 << (n) )

// code adapted from: https://stackoverflow.com/questions/57599509/c-random-non-repeated-integers-with-weights

std::vector<int> randomWeightedChoice(int ntake, double* weights, int size) {
    /* initialize random sampler */
    unsigned int seed = 12345;
    std::mt19937 rng(seed);

    /* determine smallest power of two that is larger than N */
    int tree_levels = ceil(log2((double) size));

    /* initialize vector with place-holders for perfectly-balanced tree */
    std::vector<double> tree_weights(pow2(tree_levels + 1));

    /* compute sums for the tree leaves at each node */
    int offset = pow2(tree_levels) - 1;
    for (int ix = 0; ix < size; ix++) {
        tree_weights[ix + offset] = weights[ix];
    }
    for (int ix = pow2(tree_levels+1) - 1; ix > 0; ix--) {
        tree_weights[(ix - 1) / 2] += tree_weights[ix];
    }

    /* sample according to uniform distribution */
    double rnd_subrange, w_left;
    double curr_subrange;
    int curr_ix;
    std::vector<int> sampled(ntake);
    for (int el = 0; el < ntake; el++) {

        /* go down the tree by drawing a random number and
           checking if it falls in the left or right sub-ranges */
        curr_ix = 0;
        curr_subrange = tree_weights[0];
        for (int lev = 0; lev < tree_levels; lev++) {
            rnd_subrange = std::uniform_real_distribution<double>(0, curr_subrange)(rng);
            w_left = tree_weights[2 * curr_ix + 1];
            curr_ix = 2 * curr_ix + 1 + (rnd_subrange >= w_left);
            curr_subrange = tree_weights[curr_ix];
        }

        /* finally, add element from this iteration */
        sampled[el] = curr_ix - offset;

        /* now remove the weight of the chosen element */
        tree_weights[curr_ix] = 0;
        for (int lev = 0; lev < tree_levels; lev++) {
            curr_ix = (curr_ix - 1) / 2;
            tree_weights[curr_ix] = tree_weights[2 * curr_ix + 1] + tree_weights[2 * curr_ix + 2];
        }
    }

    return sampled;
}