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

#include "objects.h"

bool Object::IsLight() const {
	if (emission.x != 0.0 || emission.y != 0.0 || emission.z != 0.0) return true;
	return false;
}

double SphereObject::Intersect(Ray3D const &ray) const {
	double eps = 1e-4;

	// Need to solve t^2*d*d + 2*t*(o-c)*d + (o-c)*(o-c)-r^2 = 0
	double a = ray.direction.dot(ray.direction);
	Vector3D tmp = ray.origin - center;
	double b = 2.0f * ray.direction.dot(tmp);
	double c = tmp.dot(tmp) - radius * radius;
	double det = b * b - 4.0 * a * c;

	if (det < 0.0f) return 0;
	double t1 = (-b + sqrt(det)) / (2.0 * a);
	double t2 = (-b - sqrt(det)) / (2.0 * a); // t2 <= t1 since a > 0
	if (t2 > eps) return t2;
	if (t1 > eps) return t1;
	return 0;
}

Vector3D SphereObject::GetNormal(Vector3D const &point) const {
	return (point - center).norm();
}
