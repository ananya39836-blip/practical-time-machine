#ifndef PTM_MATH_UTILS_HPP
#define PTM_MATH_UTILS_HPP

#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <random>
#include <stdexcept>
#include <iostream>

namespace ptm {

// Simple, robust Matrix class for small/medium dimensions (e.g. 3x3, NxM for regression)
class Matrix {
public:
    size_t rows = 0;
    size_t cols = 0;
    std::vector<double> data;

    Matrix() : rows(0), cols(0) {}
    Matrix(size_t r, size_t c, double init_val = 0.0) : rows(r), cols(c), data(r * c, init_val) {}

    double& operator()(size_t r, size_t c) {
        return data[r * cols + c];
    }

    const double& operator()(size_t r, size_t c) const {
        return data[r * cols + c];
    }

    static Matrix identity(size_t n) {
        Matrix I(n, n, 0.0);
        for (size_t i = 0; i < n; ++i) {
            I(i, i) = 1.0;
        }
        return I;
    }

    Matrix transpose() const {
        Matrix T(cols, rows);
        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                T(c, r) = (*this)(r, c);
            }
        }
        return T;
    }

    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::runtime_error("Matrix dimensions mismatch in addition");
        }
        Matrix res(rows, cols);
        for (size_t i = 0; i < data.size(); ++i) {
            res.data[i] = data[i] + other.data[i];
        }
        return res;
    }

    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::runtime_error("Matrix dimensions mismatch in subtraction");
        }
        Matrix res(rows, cols);
        for (size_t i = 0; i < data.size(); ++i) {
            res.data[i] = data[i] - other.data[i];
        }
        return res;
    }

    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            throw std::runtime_error("Matrix dimensions mismatch in multiplication");
        }
        Matrix res(rows, other.cols, 0.0);
        for (size_t r = 0; r < rows; ++r) {
            for (size_t k = 0; k < cols; ++k) {
                double a = (*this)(r, k);
                for (size_t c = 0; c < other.cols; ++c) {
                    res(r, c) += a * other(k, c);
                }
            }
        }
        return res;
    }

    // Invert square matrix using Gauss-Jordan elimination with partial pivoting
    Matrix inverse() const {
        if (rows != cols) {
            throw std::runtime_error("Cannot invert non-square matrix");
        }
        size_t n = rows;
        Matrix aug(n, 2 * n, 0.0);
        for (size_t r = 0; r < n; ++r) {
            for (size_t c = 0; c < n; ++c) {
                aug(r, c) = (*this)(r, c);
            }
            aug(r, n + r) = 1.0;
        }

        for (size_t col = 0; col < n; ++col) {
            // Find pivot
            size_t max_row = col;
            double max_val = std::abs(aug(col, col));
            for (size_t r = col + 1; r < n; ++r) {
                if (std::abs(aug(r, col)) > max_val) {
                    max_val = std::abs(aug(r, col));
                    max_row = r;
                }
            }

            if (max_val < 1e-30) {
                throw std::runtime_error("Matrix is singular or near-singular");
            }

            // Swap rows
            if (max_row != col) {
                for (size_t c = 0; c < 2 * n; ++c) {
                    std::swap(aug(col, c), aug(max_row, c));
                }
            }

            // Scale pivot row
            double pivot = aug(col, col);
            for (size_t c = 0; c < 2 * n; ++c) {
                aug(col, c) /= pivot;
            }

            // Eliminate column in other rows
            for (size_t r = 0; r < n; ++r) {
                if (r != col) {
                    double factor = aug(r, col);
                    for (size_t c = 0; c < 2 * n; ++c) {
                        aug(r, c) -= factor * aug(col, c);
                    }
                }
            }
        }

        Matrix inv(n, n);
        for (size_t r = 0; r < n; ++r) {
            for (size_t c = 0; c < n; ++c) {
                inv(r, c) = aug(r, n + c);
            }
        }
        return inv;
    }
};

// Statistical utilities
inline double calculate_mean(const std::vector<double>& v) {
    if (v.empty()) return 0.0;
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    return sum / static_cast<double>(v.size());
}

inline double calculate_variance(const std::vector<double>& v, double mean) {
    if (v.size() <= 1) return 0.0;
    double sum_sq = 0.0;
    for (double val : v) {
        double d = val - mean;
        sum_sq += d * d;
    }
    return sum_sq / static_cast<double>(v.size() - 1);
}

inline double calculate_std(const std::vector<double>& v) {
    return std::sqrt(calculate_variance(v, calculate_mean(v)));
}

inline double calculate_median(std::vector<double> v) {
    if (v.empty()) return 0.0;
    size_t n = v.size();
    std::nth_element(v.begin(), v.begin() + n / 2, v.end());
    if (n % 2 != 0) {
        return v[n / 2];
    }
    double v2 = v[n / 2];
    std::nth_element(v.begin(), v.begin() + n / 2 - 1, v.end());
    double v1 = v[n / 2 - 1];
    return 0.5 * (v1 + v2);
}

inline double calculate_mad(const std::vector<double>& v) {
    if (v.empty()) return 0.0;
    double med = calculate_median(v);
    std::vector<double> dev(v.size());
    for (size_t i = 0; i < v.size(); ++i) {
        dev[i] = std::abs(v[i] - med);
    }
    return calculate_median(dev);
}

inline double calculate_durbin_watson(const std::vector<double>& residuals) {
    if (residuals.size() <= 1) return 2.0;
    double num = 0.0;
    double den = 0.0;
    for (size_t i = 1; i < residuals.size(); ++i) {
        double diff = residuals[i] - residuals[i - 1];
        num += diff * diff;
    }
    for (double r : residuals) {
        den += r * r;
    }
    return den > 0.0 ? (num / den) : 2.0;
}

// Simple Ordinary Least Squares linear regression: y = beta_0 + beta_1 * x
inline void solve_linear_regression(const std::vector<double>& x, const std::vector<double>& y, double& beta_0, double& beta_1) {
    size_t n = x.size();
    if (n != y.size() || n < 2) {
        throw std::runtime_error("Invalid sample sizes in linear regression");
    }
    double mean_x = calculate_mean(x);
    double mean_y = calculate_mean(y);
    double s_xx = 0.0;
    double s_xy = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double dx = x[i] - mean_x;
        s_xx += dx * dx;
        s_xy += dx * (y[i] - mean_y);
    }
    if (s_xx <= 0.0) {
        beta_1 = 0.0;
        beta_0 = mean_y;
    } else {
        beta_1 = s_xy / s_xx;
        beta_0 = mean_y - beta_1 * mean_x;
    }
}

} // namespace ptm

#endif // PTM_MATH_UTILS_HPP
