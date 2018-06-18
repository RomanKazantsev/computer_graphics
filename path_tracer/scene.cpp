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

#define _USE_MATH_DEFINES
#include <cmath> 

#include "scene.h"

Object* Scene::IntersectWithNearestObject(Ray3D const &ray, double &t) {
	Object *res_ptr = nullptr;
	double res_t = std::numeric_limits<double>::max();
	for (int i = 0; i < object_ptrs.size(); i++) {
		Object *object_ptr = object_ptrs[i].get();
		double tmp_t = object_ptr->Intersect(ray);
		if (tmp_t && tmp_t < res_t) {
			res_ptr = object_ptr;
			res_t = tmp_t;
		}
	}
	t = res_t;
	return res_ptr;
}

Vector3D Scene::GenerateRandomUnitVectorInHemisphere(Vector3D const &normal) {
	Vector3D u, v; // form orthogonal system of vectors with normal
	Vector3D tmp;
	if (fabsf(normal.x) > 0.1) tmp = Vector3D(0.0, 1.0, 0.0);
	else tmp = Vector3D(1.0, 0.0, 0.0);
	u = (tmp % normal).norm();
	v = normal % u;

	// generate two random numbers
	double r1 = 2.0f * M_PI * distribution(generator);
	double r2 = distribution(generator);
	Vector3D res = (u * cosf(r1) * sqrtf(r2) + v * sinf(r1) * sqrtf(r2) + normal * sqrtf(1 - r2)).norm();
	return res;
}

Vector3D Scene::ComputeRadiance(Ray3D const &r, int depth) {
	Ray3D current_ray = r;
	double tmp_t;
	Object *current_object_ptr = IntersectWithNearestObject(current_ray, tmp_t);

	// if the ray does not hit any object on the scene
	if (current_object_ptr == nullptr) return Vector3D(0.0, 0.0, 0.0);

	// if the ray intersects light object on the scene
	if (current_object_ptr->IsLight()) return current_object_ptr->GetEmission();

	Vector3D intersect_point = current_ray.origin + current_ray.direction * tmp_t;
	Vector3D normal = current_object_ptr->GetNormal(intersect_point);
	Vector3D normal2 = normal.dot(current_ray.direction) < 0.0 ? normal : normal * (-1.0);
	Object::Material object_material = current_object_ptr->GetMaterial();
	Vector3D object_color = current_object_ptr->GetColor();
	Vector3D object_emission = current_object_ptr->GetEmission();

	double p = std::max<double>(object_color.x, std::max<double>(object_color.y, object_color.z));
	if (++depth > max_depth) {
		if (distribution(generator) < p) object_color = object_color * (1 / p);
		else return object_emission;
	}

	// a case of diffuse reflection - diffuse material
	if (object_material == Object::Material::diffuse) {
		Vector3D new_ray_direction = GenerateRandomUnitVectorInHemisphere(normal2);
		Ray3D new_ray(intersect_point, new_ray_direction);
		return object_emission + object_color.mult(ComputeRadiance(new_ray, depth));
	}

	// a case of specular reflection - mirror
	if (object_material == Object::Material::specular) {
		Vector3D new_ray_direction = current_ray.direction - normal * 2.0 * normal.dot(current_ray.direction);
		Ray3D new_ray(intersect_point, new_ray_direction);
		return object_emission + ComputeRadiance(new_ray, depth);
	}

	// a case of glass (dielectric) material
	if (object_material == Object::Material::refracture) {
		Vector3D reflection_ray_direction = current_ray.direction - normal * 2.0 * normal.dot(current_ray.direction);
		Ray3D reflection_ray(intersect_point, reflection_ray_direction);
		bool into = normal.dot(normal2) > 0; // where is the current ray going: into glass or outside?
		double n_outside = 1.0; // index of refraction for air
		double n_inside = 1.5; // index of refraction for glass
		double nnt = into ? n_outside / n_inside : n_inside / n_outside; // Snell's law
		double cos_2_theta1 = normal2.dot(current_ray.direction) * normal2.dot(current_ray.direction);
		double cos_theta1 = sqrtf(cos_2_theta1);
		double cos_2_theta2 = 1 - nnt * nnt * (1 - cos_2_theta1);

		if (cos_2_theta2 < 0.0) { // if angle is too shalow, total internal reflection occurs
			return object_emission + object_color.mult(ComputeRadiance(reflection_ray, depth));
		}

		// compute refracted ray		
		double cos_theta2 = sqrtf(cos_2_theta2);
		double sin_theta2 = sqrtf(1 - cos_2_theta2);
		double ddn = current_ray.direction.dot(normal2);
		Vector3D refraction_ray_direction = (current_ray.direction * nnt - normal * ((into ? 1 : -1)*(ddn * nnt + sqrt(cos_2_theta2)))).norm();
		Ray3D refraction_ray(intersect_point, refraction_ray_direction);

		// compute reflectance and refranction percentages
		double F0 = (nnt - 1) * (nnt - 1) / ((nnt + 1) * (nnt + 1));
		double c = 1 - (into ? cos_theta1 : cos_theta2);
		double Re = F0 + (1 - F0)*c*c*c*c*c; // reflrection percentage
		double Tr = 1.0f - Re; // refraction percentage
		double P = 0.25f + 0.5f * Re;
		double RP = Re / P;
		double TP = Tr / (1.0f - P);
		if (depth > 2) {
			if (distribution(generator) < P) return object_emission + object_color.mult(ComputeRadiance(reflection_ray, depth)) * RP;
			else return object_emission + object_color.mult(ComputeRadiance(refraction_ray, depth)) * TP;
		}
		else {
			return object_emission + object_color.mult(ComputeRadiance(reflection_ray, depth) * Re + ComputeRadiance(refraction_ray, depth) * Tr);
		}
	}

	return Vector3D(0.0, 0.0, 0.0); // unknown material
}
