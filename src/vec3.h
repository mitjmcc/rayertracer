#ifndef VEC3H
#define VEC3H
#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 {
    public:
        // Constructors 
        vec3() {}
        vec3(float x, float y, float z) {
            vec[0] = x;
            vec[1] = y;
            vec[2] = z;
        }
        // Getters
        inline float x() const { return vec[0]; }
        inline float y() const { return vec[1]; }
        inline float z() const { return vec[2]; }
        inline float r() const { return vec[0]; }
        inline float g() const { return vec[1]; }
        inline float b() const { return vec[2]; }
        // Arithmetic Operator Overloads
        inline const vec3& operator+() const { return *this; }
        inline vec3 operator-() const { return vec3(-vec[0], -vec[1], -vec[2]); }
        inline float operator[](int i) const { return vec[i]; }
        inline float& operator[](int i) { return vec[i]; }
        // Arithmetic Assignment Operator Overloads
        inline vec3& operator+=(const vec3 &v2);
        inline vec3& operator-=(const vec3 &v2);
        inline vec3& operator*=(const vec3 &v2);
        inline vec3& operator/=(const vec3 &v2);
        inline vec3& operator*=(const float t);
        inline vec3& operator/=(const float t);
        // Vector operation methods
        inline float length() const { return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]); }
        inline float squared_length() const { return vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]; }
        inline void make_unit_vector();

        float vec[3];
};

// IO stream operator overload
inline std::istream& operator>>(std::istream &is, vec3 &t) {
    is >> t.vec[0] >> t.vec[1] >> t.vec[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const vec3 &t) {
    os << t.x() << " " << t.y() << " " << t.z();
    return os;
}

// Arithmetic operator overload
inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z());
}

inline vec3 operator*(float t, const vec3 &v) {
    return vec3(t * v.x(), t * v.y(), t * v.z());
}

inline vec3 operator/(vec3 v, float t) {
    return vec3(v.x() / t, v.y() / t, v.z() / t);
}

inline vec3 operator*(const vec3 &v, float t) {
    return vec3(t * v.x(), t * v.y(), t * v.z());
}

// Arithmetic assignment operator overload
inline vec3& vec3::operator+=(const vec3 &v) {
    vec[0] += v.x();
    vec[1] += v.y();
    vec[2] += v.z(); 
    return *this;
}

inline vec3& vec3::operator-=(const vec3 &v) {
    vec[0] -= v.x();
    vec[1] -= v.y();
    vec[2] -= v.z(); 
    return *this;
} 

inline vec3& vec3::operator*=(const vec3 &v) {
    vec[0] *= v.x();
    vec[1] *= v.y();
    vec[2] *= v.z(); 
    return *this;
} 

inline vec3& vec3::operator/=(const vec3 &v) {
    vec[0] /= v.x();
    vec[1] /= v.y();
    vec[2] /= v.z(); 
    return *this;
}

inline vec3& vec3::operator*=(const float t) {
    vec[0] *= t;
    vec[1] *= t;
    vec[2] *= t; 
    return *this;
} 

inline vec3& vec3::operator/=(const float t) {
    vec[0] /= t;
    vec[1] /= t;
    vec[2] /= t; 
    return *this;
} 

// Vector operation methods
inline void vec3::make_unit_vector() {
    float k = 1.0f / squared_length();
    vec[0] = vec[0] / k; vec[1] = vec[1] / k; vec[2] = vec[2] / k;
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

inline float dot(const vec3 &v1, const vec3 &v2) {
    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec3( (v1.y() * v2.z() - v1.z() * v2.y()),
                -(v1.x() * v2.z() - v1.z() * v2.x()),
                 (v1.x() * v2.y() - v1.y() * v2.x()));
}

#endif
