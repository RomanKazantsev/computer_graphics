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

#include "utils.h"

class Object {
	public:
		enum class Material { diffuse, specular, refracture };
		Object(Material material_, Vector3D color_, Vector3D emission_) : material(material_), color(color_), emission(emission_) {}
		Object(Object const &other) : material(other.material), color(other.color), emission(other.emission) {}
		// intersect some ray with the object
		virtual double Intersect(Ray3D const &ray) const = 0;
		// get a normal at some point of the object
		virtual Vector3D GetNormal(Vector3D const &point) const = 0;
		// get object material
		virtual Material GetMaterial() const { return material; }
		virtual bool IsLight() const;
		virtual Vector3D GetEmission() const { return emission; }
		virtual Vector3D GetColor() const { return color; }
		virtual ~Object() {}
	protected:
		Material material;
		Vector3D color;
		Vector3D emission;
};

class SphereObject : public Object {
	public:
		SphereObject(double radius_, Vector3D center_, Material material_, Vector3D color_, Vector3D emission_) :
			Object(material_, color_, emission_), radius(radius_), center(center_){}
		SphereObject(SphereObject const &other) :
			Object(other), radius(other.radius), center(other.center) {}
		virtual double Intersect(Ray3D const &ray) const;
		// get a normal at some point of the object
		virtual Vector3D GetNormal(Vector3D const &point) const;
		virtual ~SphereObject() {}
	protected:
		double radius;
		Vector3D center;
};
