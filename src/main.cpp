#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"

using namespace std;

string filename = "hello.ppm";

vec3 color(const ray &r, hitable *world) {
    hit_record rec;
    if (world->hit(r, 0.0f, MAXFLOAT, rec)) {
        return 0.5f * (rec.normal + vec3(1, 1, 1));
    } else {
        // Background color: Lerp from blue to white
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
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
    
    // Sphere list
    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5f);
    list[1] = new sphere(vec3(0,-100.5f,-1), 100);
    hitable *world = new hitable_list(list, 2);

    // Loop through pixels
    for (int j = ny - 1; j > 0; --j) {
        for (int i = 0; i < nx; ++i) {
            // Horizontal and vertical offset vectors
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            // Ray through each of the pixels
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            vec3 p = r.point_at_parameter(2.0f);
            vec3 col = color(r, world);
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
