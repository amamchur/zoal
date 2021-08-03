#ifndef ZOAL_DATA_VECTOR_HPP
#define ZOAL_DATA_VECTOR_HPP

#include <math.h>

namespace zoal { namespace data {
    template<class T>
    class vector {
    public:
        T x;
        T y;
        T z;

        vector() noexcept
            : x(static_cast<T>(0))
            , y(static_cast<T>(0))
            , z(static_cast<T>(1)) {}

        vector(T a, T b, T c) noexcept
            : x(a)
            , y(b)
            , z(c) {}

        vector(const vector &v) noexcept
            : x(v.x)
            , y(v.y)
            , z(v.z) {}

        vector(volatile const vector &v) noexcept
            : x(v.x)
            , y(v.y)
            , z(v.z) {}

        template<class U>
        vector<U> convert() {
            return vector<U>(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z));
        }

        double length() const {
            return sqrt((double)x * x + (double)y * y + (double)z * z);
        }

        vector<double> normalize() const {
            auto l = length();
            return vector<double>(x / l, y / l, z / l);
        }

        vector &operator=(const vector &v) {
            x = v.x;
            y = v.y;
            z = v.z;
            return *this;
        }

        vector &operator+=(T m) {
            x += m;
            y += m;
            z += m;
            return *this;
        }

        vector &operator-=(T m) {
            x -= m;
            y -= m;
            z -= m;
            return *this;
        }

        vector &operator*=(T m) {
            x *= m;
            y *= m;
            z *= m;
            return *this;
        }

        vector &operator/=(T m) {
            x /= m;
            y /= m;
            z /= m;
            return *this;
        }

        inline T operator[](int i) const {
            return *(&x + i);
        }

        inline T& operator[](int i) {
            return *(&x + i);
        }
    };
}}

#endif
