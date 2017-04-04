//
// Created by Ficos on 25/03/2017.
//

#ifndef RAYTRACING_UTILITIES_H
#define RAYTRACING_UTILITIES_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include "common.h"

template <class T>
class Vector {
public:
    T x, y, z;
    Vector(): x(T(0)), y(T(0)), z(T(0)) {}
    Vector(T x) : x(x), y(x), z(x) {}
    Vector(T x, T y, T z): x(x), y(y), z(z) {}
    Vector(const Vector<T> &v): x(v.x), y(v.y), z(v.z) {}
    void normalize(){
        T norm = length();
        if (norm > 0) {
            *this /= norm;
        }
    }
    T length2() const { return x * x + y * y + z * z; }
    T length() const { return sqrt(length2()); }

    // operators
    Vector<T> operator * (const T &c) const { return Vector<T>(c * x, c * y, c * z); }
    Vector<T> operator / (const T &c) const { return Vector<T>(x / c, y / c, z / c); }
    Vector<T> operator * (const Vector<T> &v) const { return Vector<T>(x * v.x, y * v.y, z * v.z); }
    Vector<T> operator - (const Vector<T> &v) const { return Vector<T>(x - v.x, y - v.y, z - v.z); }
    Vector<T> operator + (const Vector<T> &v) const { return Vector<T>(x + v.x, y + v.y, z + v.z); }
    Vector<T>& operator += (const Vector<T> &v) { x += v.x, y += v.y, z += v.z; return *this; }
    Vector<T>& operator -= (const Vector<T> &v) { x -= v.x, y -= v.y, z -= v.z; return *this; }
    Vector<T>& operator *= (const Vector<T> &v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
    Vector<T>& operator *= (const T &c) { x *= c, y *= c, z *= c; return *this; }
    Vector<T>& operator /= (const T &c) { x /= c, y /= c, z /= c; return *this; }
    bool isEqual (const Vector<T> &c) const {
        if (fabsf(x - c.x) < EPSILON && fabsf(y - c.y) < EPSILON && fabsf(z - c.z) < EPSILON)
            return true;
        return false;
    }
    Vector<T>& exp() {
        x = expf(x);
        y = expf(y);
        z = expf(z);
        return *this;
    }
    Vector<T> operator - () const { return Vector<T>(-x, -y, -z); }
    friend Vector<T> operator * ( T f, const Vector<T>& v ) { return Vector<T>( v.x * f, v.y * f, v.z * f ); }

    T getMax() const {
        return std::max(x, std::max(y, z));
    }

    T dot(const Vector<T> &v) const { return x * v.x + y * v.y + z * v.z; }
    Vector<T> cross(const Vector<T> &v) const {
        return Vector<T>( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
    }

    friend std::ostream & operator << (std::ostream &os, const Vector<T> &v)
    {
        os << "(" << v.x << " " << v.y << " " << v.z << ")";
        return os;
    }
};

typedef Vector<float> VecF;
// RGB emission
typedef Vector<float> Color;

#define WHITE   Color(1.0f, 1.0f, 1.0f)
#define BLACK   Color(0.0f, 0.0f, 0.0f)
#define GRAY    Color(0.6f, 0.6f, 0.6f)
#define RED     Color(0.8f, 0.0f, 0.0f)
#define BLUE    Color(0.0f, 0.2f, 1.0f)
#define GREEN   Color(0.0f, 1.0f, 0.0f)

#endif //RAYTRACING_UTILITIES_H