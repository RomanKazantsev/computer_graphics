#pragma once

#include <cmath>

struct Vec {
	double x, y, z;
	Vec(double x_ = 0, double y_ = 0, double z_ = 0) { x = x_; y = y_; z = z_; }
	Vec operator+(const Vec &b) const { return Vec(x + b.x, y + b.y, z + b.z); }
	Vec operator-(const Vec &b) const { return Vec(x - b.x, y - b.y, z - b.z); }
	Vec operator*(double b) const { return Vec(x*b, y*b, z*b); }
	Vec mult(const Vec &b) const { return Vec(x*b.x, y*b.y, z*b.z); }
	Vec& norm() { return *this = *this * (1 / sqrt(x*x + y * y + z * z)); }
	double dot(const Vec &b) const { return x * b.x + y * b.y + z * b.z; }
	Vec operator%(Vec&b) { return Vec(y*b.z - z * b.y, z*b.x - x * b.z, x*b.y - y * b.x); }
};

int WriteImageToBmp(Vec const *image, int width, int height);

class Vector3D {
public:
	// constructor
	Vector3D(float xx = 0.0f, float yy = 0.0f, float zz = 0.0f) : x(xx), y(yy), z(zz) {}
	// copy contructor
	Vector3D(Vector3D const &other) : x(other.x), y(other.y), z(other.z) {}
	// operations
	Vector3D& operator= (Vector3D const & other);
	Vector3D operator+ (Vector3D const &other) const;
	Vector3D operator- (Vector3D const &other) const;
	Vector3D operator* (float scalar) const;
	Vector3D mult(Vector3D const &other) const;
	Vector3D& norm();
	float dot(Vector3D const& other) const;
	Vector3D operator% (Vector3D const& other) const;

private:
	float x, y, z;
};

struct Ray3D {
	Vector3D origin, direction;
	Ray3D(Vector3D origin_, Vector3D direction_) : origin(origin_), direction(direction_) {}
};
