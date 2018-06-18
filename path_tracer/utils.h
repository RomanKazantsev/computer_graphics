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

#pragma once

#include <cmath>

class Vector3D {
public:
	// constructor
	Vector3D(double xx = 0.0, double yy = 0.0, double zz = 0.0) : x(xx), y(yy), z(zz) {}
	// copy contructor
	Vector3D(Vector3D const &other) : x(other.x), y(other.y), z(other.z) {}
	// destructor
	virtual ~Vector3D() {}
	// operations
	Vector3D& operator= (Vector3D const & other);
	Vector3D operator+ (Vector3D const &other) const;
	Vector3D operator- (Vector3D const &other) const;
	Vector3D operator* (double scalar) const;
	Vector3D mult(Vector3D const &other) const;
	Vector3D& norm();
	double dot(Vector3D const& other) const;
	Vector3D operator% (Vector3D const& other) const;
	double x, y, z;
};

struct Ray3D {
	Vector3D origin, direction;
	Ray3D(Vector3D origin_, Vector3D direction_) : origin(origin_), direction(direction_) {}
};

int WriteImageToBmp(Vector3D const *image, int width, int height);
