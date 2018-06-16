#include "objects.h"

float SphereObject::Intersect(Ray3D const &ray) const {
	float eps = 1e-4;

	// Need to solve t^2*d*d + 2*t*(o-c)*d + (o-c)*(o-c)-r^2 = 0
	float a = ray.direction.dot(ray.direction);
	Vector3D tmp = ray.origin - center;
	float b = 2.0f * ray.direction.dot(tmp);
	float c = tmp.dot(tmp) - radius * radius;
	float det = b * b - 4.0 * a * c;

	if (det < 0.0f) return 0;
	float t1 = (-b + sqrt(det)) / (2.0 * a);
	float t2 = (-b - sqrt(det)) / (2.0 * a); // t2 <= t1 since a > 0
	if (t2 > eps) return t2;
	if (t1 > eps) return t1;
	return 0;
}