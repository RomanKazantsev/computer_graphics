/*
Copyright (c) 2018 Roman Kazantsev
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <vector>
#include <memory>
#include <utility>
#include <iostream>

#include "objects.h"
#include "scene.h"

using namespace std;

Scene scene(5);

std::default_random_engine generator;
std::uniform_real_distribution<double> distr(0.0, 1.0);
double urandom() {
	return distr(generator);
}

void CreateScene() {
	scene.AddObject(new SphereObject(1e5, Vector3D(1e5 + 1, 40.8, 81.6), Object::Material::diffuse, Vector3D(.75, .25, .25), Vector3D())); // left
	scene.AddObject(new SphereObject(1e5, Vector3D(-1e5 + 99, 40.8, 81.6), Object::Material::diffuse, Vector3D(.25, .25, .75), Vector3D())); // right
	scene.AddObject(new SphereObject(1e5, Vector3D(50, 40.8, 1e5), Object::Material::diffuse, Vector3D(.75, .75, .75), Vector3D())); // back
	scene.AddObject(new SphereObject(1e5, Vector3D(50, 40.8, -1e5 + 170), Object::Material::diffuse, Vector3D(), Vector3D())); // front
	scene.AddObject(new SphereObject(1e5, Vector3D(50, 1e5, 81.6), Object::Material::diffuse, Vector3D(.75, .75, .75), Vector3D())); // bottom
	scene.AddObject(new SphereObject(1e5, Vector3D(50, -1e5 + 81.6, 81.6), Object::Material::diffuse, Vector3D(.75, .75, .75), Vector3D())); // top
	scene.AddObject(new SphereObject(16.5, Vector3D(27, 16.5, 47), Object::Material::specular, Vector3D(1, 1, 1)*.999, Vector3D())); // mirror
	scene.AddObject(new SphereObject(16.5, Vector3D(73, 16.5, 78), Object::Material::refracture, Vector3D(1, 1, 1)*.999, Vector3D())); // glass
	scene.AddObject(new SphereObject(600, Vector3D(50, 681.6 - .27, 81.6), Object::Material::diffuse, Vector3D(), Vector3D(12, 12, 12))); // light
}

inline double clamp(double x) { return x < 0.0 ? 0.0 : x > 1.0 ? 1.0 : x; }

int main(int argc, char *argv[]) {
	// handle command line
	int samples_per_pixel = 1;
	std::cout << "Usage: " << argv[0] << " [samples_per_pixel(default value is 1)]" << std::endl;
	if (argc > 1) samples_per_pixel = atoi(argv[1]) / 4; // since every pixel is split into 4 subpixels

	// create a scene to model global illumination
	CreateScene();

	// create array to store image
	int width = 512;
	int height = 512;
	std::unique_ptr<Vector3D> image_ptr(new Vector3D[width * height]);
	Vector3D *c = image_ptr.get();
	Vector3D r;

	// setup camera
	Ray3D cam(Vector3D(50, 52, 295.6), Vector3D(0, -0.042612, -1).norm());
	Vector3D cx = Vector3D(width * 0.5135 / height);
	Vector3D cy = (cx % cam.direction).norm()* 0.5135;

    #pragma omp parallel for schedule(dynamic, 1) private(r)
	for (int y = 0; y < height; y++) {
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samples_per_pixel * 4, 100.*y / (height - 1));
		for (int x = 0; x < width; x++)
			for (int sy = 0, i = (height - y - 1) * width + x; sy < 2; sy++)
				for (int sx = 0; sx < 2; sx++, r = Vector3D()) {
					for (int s = 0; s < samples_per_pixel; s++) {
						double r1 = 2 * urandom(), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
						double r2 = 2 * urandom(), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
						Vector3D d = cx * (((sx + 0.5 + dx) / 2 + x) / width - 0.5) + cy * (((sy + 0.5 + dy) / 2 + y) / height - 0.5) + cam.direction;
						r = r + scene.ComputeRadiance(Ray3D(cam.origin + d * 140, d.norm()), 0)*(1. / samples_per_pixel);
					}
					c[i] = c[i] + Vector3D(clamp(r.x), clamp(r.y), clamp(r.z))* 0.25;
				}
	}
	WriteImageToBmp(c, width, height);
}
