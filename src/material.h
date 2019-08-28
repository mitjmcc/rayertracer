#ifndef MATERIALH
#define MATERIALH

struct hit_record;

#include "ray.h"
#include "hitable.h"

vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - 2 * dot(v, n) * n;
}

// Using Snell's Law and IOR
bool refract(const vec3 &v, const vec3 &n, float ior, vec3 &refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ior * ior * (1 - dt * dt);
    if (discriminant > 0) {
        refracted = ior * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    return false;
}

// schlick fresnel approximation
float schlick(float cosine, float ref_index) {
    float r0 = (1 - ref_index) / (1 + ref_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0f * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0f);
    return p;
}

class material {
    public:
        virtual bool scatter(const ray &r_in,
                             const hit_record &rec,
                             vec3 &attenuation,
                             ray &scattered) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const vec3 &a) : albedo(a) {}
        virtual bool scatter(const ray &r_in, 
                                      const hit_record &rec,
                                      vec3 &attenuation,
                                      ray &scattered) const {
            vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            scattered = ray(rec.p, target - rec.p, r_in.time());
            attenuation = albedo;
            return true;
        }

        vec3 albedo;
};

class metal : public material {
    public:
        metal(const vec3 &a, float m) : albedo(a) { if (m < 1) metallic = m; else metallic = 1; }
        virtual bool scatter(const ray &r_in, 
                                      const hit_record &rec,
                                      vec3 &attenuation,
                                      ray &scattered) const {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + metallic * random_in_unit_sphere());
            attenuation = albedo;
            return dot(scattered.direction(), rec.normal) > 0;
        }

        vec3 albedo;
        float metallic;
};

class dielectric : public material {
    public:
        dielectric(float rf) : ref_index(rf) {}
        virtual bool scatter(const ray &r_in, 
                             const hit_record &rec,
                             vec3 &attenuation,
                             ray &scattered) const {
            vec3 outward_normal;
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            float ior_ratio;
            float cosine;
            float reflect_prob;
            attenuation = vec3(1, 1, 1);
            vec3 refracted;
            if (dot(r_in.direction(), rec.normal) > 0) {
                outward_normal = -rec.normal;
                ior_ratio = ref_index;
                cosine = ref_index * dot(r_in.direction(), rec.normal) / r_in.direction().length();
            } else {
                outward_normal = rec.normal;
                ior_ratio = 1.0 / ref_index;
                cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
            }
            if (refract(r_in.direction(), outward_normal, ior_ratio, refracted)) {
                reflect_prob = schlick(cosine, ref_index);
            } else {
                reflect_prob = 1.0f;
            }
            if (drand48() < reflect_prob) {
                scattered = ray(rec.p, reflected);
            } else {
                scattered = ray(rec.p, refracted);
            }
            return true;
        }

        float ref_index;
};

#endif
