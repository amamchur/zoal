#ifndef ZOAL_DATA_VECTOR_HPP
#define ZOAL_DATA_VECTOR_HPP

namespace zoal { namespace data {
    template<class T>
    class vector {
    public:
        T x;
        T y;
        T z;

        vector()
            : x(static_cast<T>(0))
            , y(static_cast<T>(0))
            , z(static_cast<T>(0)) {}

        vector(T a, T b, T c)
            : x(a)
            , y(b)
            , z(c) {}

        vector(const vector &v)
            : x(v.x)
            , y(v.y)
            , z(v.z) {}

        vector(volatile const vector &v)
            : x(v.x)
            , y(v.y)
            , z(v.z) {}

        template<class U>
        vector<U> convert() {
            return vector<U>(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z));
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
    };
}}

#endif
