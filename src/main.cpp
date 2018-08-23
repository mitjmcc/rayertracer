#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

using namespace std;

string filename = "hello.ppm";

int main() {
    int nx = 200;
    int ny = 200;

    ofstream imgRender(filename);

    int stb_n = 3;
    unsigned char img[nx * ny * stb_n];
    // stbi_load("hello.png" , &nx, &ny, &stb_n, 3)
    if (!img) {
        // fprintf( stderr, stbi_failure_reason() );
        cout << "\n";
    }

    cout << img[0] << "\n";
    // imgRender << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j > 0; --j) {
        for (int i = 0; i < nx; ++i) {
            float r = float(i) / float(nx);
            float g = float(j) / float(ny);
            float b = 0.2;
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);

            // img[3] = ir; 
            img << ir << " " << ig << " " << ib << "\n";
        }
    }


    imgRender.close();
    // stbi_image_free(img);
}