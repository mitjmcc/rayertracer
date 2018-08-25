#include <iostream>
#include <fstream>
#include <ray.h>

using namespace std;

string filename = "hello.ppm";

bool hit_sphere(const vec3 &center, float radius, const ray &r) {
    vec3 OC = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0f * dot(OC, r.direction());
    float c = dot(OC, OC) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    return discriminant > 0;
}

vec3 color(const ray &r) {
    if (hit_sphere(vec3(0, 0, -1), 0.5f, r)) {
        return vec3(1, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5f * (unit_direction.y() + 1.0f);
    // Lerp from blue to white
    return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
    // Dimensions
    int nx = 200;
    int ny = 100;

    // Output file
    ofstream imgRender(filename);

    // PPM Header
    imgRender << "P3\n" << nx << " " << ny << "\n255\n";
    
    // Near plane definitions, +y up, +x right, -z into screen
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    
    // Loop through pixels
    for (int j = ny - 1; j > 0; --j) {
        for (int i = 0; i < nx; ++i) {
            // Horizontal and vertical offset vectors
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            // Ray through each of the pixels
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            vec3 col = color(r);
            // Color scaled to 0 to 255
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            // Write color to image
            imgRender << ir << " " << ig << " " << ib << "\n";
        }
    }

    imgRender.close();
}
