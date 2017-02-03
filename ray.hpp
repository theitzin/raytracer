#ifndef RAY_HPP
#define RAY_HPP

#include <cmath>
#include "vec3.hpp"
#include "world.hpp"

class Color
{
public:
	float r, g, b;

	Color() {
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
	}
	Color(float _r, float _g, float _b) {
		r = _r;
		g = _g;
		b = _b;
	}

	Color operator+(const Color & color) const {
		return Color(fmin(r + color.r, 1.0f), fmin(g + color.g, 1.0f), fmin(b + color.b, 1.0f));
	}

	void operator+=(const Color & color) {
		r = fmin(r + color.r, 1.0f);
		g = fmin(g + color.g, 1.0f);
		b = fmin(b + color.b, 1.0f);
	}

	Color operator*(float scalar) const {
		return Color(fmin(scalar * r, 1.0f), fmin(scalar * g, 1.0f), fmin(scalar * b, 1.0f));
	}

	void operator*=(float scalar) {
		r = fmin(r * scalar, 1.0f);
		g = fmin(g * scalar, 1.0f);
		b = fmin(b * scalar, 1.0f);
	}

	Color intersect(const Color & color) const {
		return Color(fmin(r, color.r), fmin(g, color.g), fmin(b, color.b));
	}
};

class Surface
{
protected:
	Color color;

	float ambientCoef;
	float diffuseCoef;
	float specularCoef;
	float phongModel;

	float mirrorCoef;

public:
	Surface() {

		color = Color(1.0f, 0.0f, 0.0f);

		ambientCoef = 0.3f;
		diffuseCoef = 0.4f;
		specularCoef = 0.5f;
		phongModel = 20;

		mirrorCoef = 0.0f;
	}
	virtual ~Surface() {};
	virtual void setColor(float r, float g, float b) {
		color = Color(r, g, b);
	}
	virtual void setColor(const Color & _color) {
		color = _color;
	}
	virtual void setShadingModel(float _ambientCoef, float _diffuseCoef, float _specularCoef) {
		ambientCoef = _ambientCoef;
		diffuseCoef = _diffuseCoef;
		specularCoef = _specularCoef;
	}
	virtual void setPhongModel(int _phongModel) {
		phongModel = _phongModel;
	}
	virtual void setMirror(float _mirrorCoef) {
		mirrorCoef = _mirrorCoef;
	}

	virtual Color getColor(const Vec3<float> & point) const {
		return color;
	}
	virtual float getAmbient() const {
		return ambientCoef;
	}
	virtual float getDiffuse() const {
		return diffuseCoef;
	}
	virtual float getSpecular() const {
		return specularCoef;
	}
	virtual int getPhongModel() const {
		return phongModel;
	}
	virtual float getMirror(const Vec3<float> & point) const {
		return mirrorCoef;
	}
};

class S_Pattern : public Surface
{
public:
	S_Pattern() : Surface() {
		
	}

	virtual Color getColor(const Vec3<float> & point) const {

		float modx = fmod(fabs(point.getX()), M_PI);
		float modz = fmod(fabs(point.getZ()), M_PI);

		return Color(
			color.r * sin(modx) * sin(modz), 
			color.g * sin(modx) * sin(modz), 
			color.b * sin(modx) * sin(modz));
	}
};

class Ray
{
public:
	const Vec3<float> origin;
	const Vec3<float> direction;

	Ray(const Vec3<float> & _origin, const Vec3<float> & _direction) 
	: origin(_origin), direction(_direction.normalise()) {

	}
};

#endif