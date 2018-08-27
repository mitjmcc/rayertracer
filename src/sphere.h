#ifndef SPHEREH
#define SPHEREH
#include "hitable.h"

class sphere: public hitable {
    public:
        sphere() {}
        sphere(vec3 cen, float r) : center(cen), radius(r) {};
        virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
        vec3 center;
        float radius; 
};

// t*t*dot( B, B ) + 2*t*dot( B, A -​ C ) + dot( A - C, A - C ) - R*R = 0
// Return true when the equation has one or two roots for t, so ray is in sphere
bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    vec3 OC = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(OC, r.direction());
    float c = dot(OC, OC) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}

#endif