#pragma once

namespace toolbox {

namespace geometry {

/**
 * @brief A 2D point (equivalently, a vector from the origin) with coordinates of type T.
 * @tparam T The coordinate type. Use an integer type (e.g. long long) for exact predicates
 * (cross/dot are computed without division), or a floating type when approximate coordinates
 * are acceptable.
 * @note This is a plain value type: x and y are public, deliberately deviating from the
 * usual private-member convention, because a point carries no invariant and public
 * coordinates read naturally (like std::complex's real/imag).
 * @note [Overflow]: cross() and dot() multiply two coordinates. With T = long long and
 * coordinates up to ~1e9, each product is < ~1e18 and fits; callers with larger coordinates
 * must use a wider type to avoid overflow.
 */
template <typename T>
struct Point {
    T x;
    T y;

    Point() : x(T()), y(T()) {}
    Point(T x_arg, T y_arg) : x(x_arg), y(y_arg) {}

    Point operator+(const Point &other) const { return Point(x + other.x, y + other.y); }
    Point operator-(const Point &other) const { return Point(x - other.x, y - other.y); }
    Point operator*(const T &scalar) const { return Point(x * scalar, y * scalar); }
    bool operator==(const Point &other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point &other) const { return !(*this == other); }
    // Lexicographic order by (x, y); used to sort points for the convex hull algorithms.
    bool operator<(const Point &other) const { return x != other.x ? x < other.x : y < other.y; }
};

/**
 * @brief Cross product of vectors a and b: a.x * b.y - a.y * b.x.
 * @note Sign gives the orientation of b relative to a: > 0 if b is counterclockwise from a.
 */
template <typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return a.x * b.y - a.y * b.x;
}

/**
 * @brief Cross product of the vectors (a - o) and (b - o).
 * @return > 0 if o->a->b turns counterclockwise (left turn), < 0 if clockwise (right turn),
 * 0 if o, a, b are collinear.
 */
template <typename T>
T cross(const Point<T> &o, const Point<T> &a, const Point<T> &b) {
    return cross(a - o, b - o);
}

/**
 * @brief Dot product of vectors a and b: a.x * b.x + a.y * b.y.
 */
template <typename T>
T dot(const Point<T> &a, const Point<T> &b) {
    return a.x * b.x + a.y * b.y;
}

/**
 * @brief Dot product of the vectors (a - o) and (b - o).
 */
template <typename T>
T dot(const Point<T> &o, const Point<T> &a, const Point<T> &b) {
    return dot(a - o, b - o);
}

/**
 * @brief Sign of a value: -1 if negative, 0 if zero, +1 if positive.
 */
template <typename T>
int sgn(const T &value) {
    return (T() < value) - (value < T());
}

}  // namespace geometry

}  // namespace toolbox
