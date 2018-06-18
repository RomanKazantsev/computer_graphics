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
#include <vector>
#include <memory>
#include <utility>
#include <limits>
#include <random>

#include "utils.h"
#include "objects.h"

using namespace std;

class Scene {
	public:
		// empty constructor
		Scene(int max_depth_ = 5) : distribution(0.0, 1.0), max_depth(max_depth_) {}
		// destructor
		virtual ~Scene() {}
		// add a new object to scene
		void AddObject(Object* object_ptr) {
			std::unique_ptr<Object> tmp_ptr(object_ptr);
			object_ptrs.push_back(std::move(tmp_ptr));
		}
		Vector3D ComputeRadiance(Ray3D const &r, int depth);
	private:
		// copy constructor is not allowed
		Scene(Scene const &other) {}
		// intersect a ray with the nearest object of the scene
		Object* IntersectWithNearestObject(Ray3D const &ray, double &t);
		// generate a random unit vector in hemisphere
		Vector3D GenerateRandomUnitVectorInHemisphere(Vector3D const &normal);

	private:
		std::vector<std::unique_ptr<Object>> object_ptrs;
		std::default_random_engine generator;
		std::uniform_real_distribution<double> distribution;
		int max_depth;
};
