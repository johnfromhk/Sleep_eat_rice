#include "fast_vec_sum.h"
#include"constdef.h"

const vec_t2 prefix_t::empty_prefix_sum(ROWS + 1, std::vector<int>(COLS + 1, 0));
prefix_t::prefix_t(const vec_t2&vec) {
    for (size_t i = 1; i <= ROWS; ++i) {
        for (size_t j = 1; j <= COLS; ++j) {
            prefix_sum[i][j] = vec[i - 1][j - 1]
                + prefix_sum[i - 1][j]
                + prefix_sum[i][j - 1]
                - prefix_sum[i - 1][j - 1];
        }
    }
}

int prefix_t::getRectangleSum(const size_t& x1, const size_t& y1, const size_t& x2, const size_t& y2) const{
    return prefix_sum[x2 + 1][y2 + 1] - prefix_sum[x1][y2 + 1] - prefix_sum[x2 + 1][y1] + prefix_sum[x1][y1];
}
