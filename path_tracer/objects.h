#pragma once

#include "utils.h"

class Object {
	public:
		enum class Material { diffuse, specular, refracture };
		Object(Material material_, Vector3D color_) : material(material_), color(color_) {}
		Object(Object const &other) : material(other.material), color(other.color) {}
		virtual float Intersect(Ray3D const &ray) const = 0;

	protected:
		Material material;
		Vector3D color;
};

class SphereObject : public Object {
	SphereObject(float radius_, Vector3D center_, Material material_, Vector3D color_) :
		Object(material_, color_), radius(radius_), center(center_){}
	SphereObject(SphereObject const &other) :
		Object(other), radius(other.radius), center(other.center) {}
	virtual float Intersect(Ray3D const &ray) const;

protected:
	float radius;
	Vector3D center;
};