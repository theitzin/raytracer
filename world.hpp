#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <cmath>
#include "vec3.hpp"
#include "ray.hpp"


class Light
{
public:
	const Color color;
	const Vec3<float> origin;

	Light(const Color & _color, const Vec3<float> & _origin) 
	: color(_color), origin(_origin) {

	}
};

class WorldObject
{
public:
	Surface * surf;

	WorldObject(Surface * _surf) : surf(_surf) {};
	virtual ~WorldObject() {
		delete surf;
	}
	Surface * surface() { return surf; };
	virtual float distance(const Ray & ray) const = 0;
	virtual Vec3<float> normal(const Ray & ray) const = 0;
	virtual Vec3<float> intersection(const Ray & ray) const {

		float dist = distance(ray);
		return ray.origin + (ray.direction * dist);
	}
};

class WO_Plane : public WorldObject
{
private:
	const Vec3<float> point;
	const Vec3<float> norm;
public:
	WO_Plane(Surface * _surf, const Vec3<float> & _point, const Vec3<float> & _norm) 
	: WorldObject(_surf), point(_point) 
	, norm(_norm.normalise()) {

	}

	virtual float distance(const Ray & ray) const {

		if (ray.direction.dotProduct(norm) == 0.0f)
			return -1.0f;

		return -norm.dotProduct(ray.origin - point) / norm.dotProduct(ray.direction);
	}
	virtual Vec3<float> normal(const Ray & ray) const {
		
		return norm; 
	}
};

class WO_Sphere : public WorldObject
{
private:
	const Vec3<float> origin;
	const float radius;

public:
	WO_Sphere(Surface * _surf, const Vec3<float> & _origin, float _radius)
	: WorldObject(_surf), origin(_origin), radius(_radius) {

	}

	virtual float distance(const Ray & ray) const {

		float dist;
		float a = ray.direction.length2();
		float b = 2 * ray.direction.dotProduct(ray.origin - origin);
		float c = (ray.origin - origin).length2() - radius * radius;

		float tmp = b * b - 4 * a * c;
		if (tmp < 0.0f)
			return -1.0f;

		if (-b + sqrt(tmp) < 0.0f)
			return -1.0f;

		if (-b - sqrt(tmp) > 0.0f)
			dist = (-b - sqrt(tmp)) / a / 2;
		else 
			dist = (-b + sqrt(tmp)) / a / 2;

		return dist;
	}
	virtual Vec3<float> normal(const Ray & ray) const {

		float dist = distance(ray);

		return (ray.origin + (ray.direction * dist) - origin).normalise();
	}
};


class World
{
private:
	float minCastDist;
	float maxLightError;
	Color voidColor;

	Color ambientColor;

public:
	std::vector<WorldObject*> objects;
	std::vector<Light*> lights;

	World() {

		minCastDist = 0.001f;
		maxLightError = 0.001f;
		voidColor = Color(1.0f, 1.0f, 1.0f);
		ambientColor = Color(1.0f, 1.0f, 1.0f);

	}
	~World() {
		for (int i = 0; i < objects.size(); i++)
			delete objects[i];

		for (int i = 0; i < lights.size(); i++)
			delete lights[i];
	}

	void addWorldObject(WorldObject * wo) {
		objects.push_back(wo);
	}

	void addLight(Light * l) {
		lights.push_back(l);
	}

	Color getColor(const Ray & ray, int depth = 0) const {

		if (depth > 10) { 
			return voidColor;
		}

		int obj = castRay(ray);
		if (obj == -1)
			return voidColor;

		Vec3<float> inter = objects[obj]->intersection(ray);
		Vec3<float> norm = objects[obj]->normal(ray);
		Surface * surf = objects[obj]->surface();

		Color lightColor = ambientColor * surf->getAmbient();

		for (int i = 0; i < lights.size(); i++) {

			Vec3<float> lightDir = (inter - lights[i]->origin).normalise();
			Ray lightRay(lights[i]->origin, lightDir);

			int lightObj = castRay(lightRay);
			if (lightObj == -1 || lightObj != obj)
				continue;

			// diffuse
			float diffusion = fmax(0.0f, -norm.dotProduct(lightDir));
			lightColor += lights[i]->color * (surf->getDiffuse() * diffusion);

			// specular
			Vec3<float> bisector = (ray.direction + lightDir).normalise();
			float specular = pow(fmax(0.0f, -norm.dotProduct(bisector)), surf->getPhongModel());
	
			lightColor += lights[i]->color * (surf->getSpecular() * specular);
			
		}

		float mir = surf->getMirror(inter);
		Color surfaceColor;

		if (mir != 0.0f) {
			Vec3<float> reflectionDir = norm * (-2 * norm.dotProduct(ray.direction)) + ray.direction;
			Color mirrorColor = getColor(Ray(inter, reflectionDir), ++depth);

			surfaceColor = surf->getColor(inter) * (1 - mir) + mirrorColor * mir;
		}
		else {
			surfaceColor = surf->getColor(inter);
		}

		return surfaceColor.intersect(lightColor);
	}

	int castRay(const Ray & ray) const {

		int index = -1;
		float smallestDist;
		float tmpDist;

		for (int i = 0; i < objects.size(); i++) {
			
			tmpDist = objects[i]->distance(ray);
			if (tmpDist > 0.0f) {
				if (tmpDist > minCastDist && (index == -1 || tmpDist < smallestDist)) {
					smallestDist = tmpDist;
					index = i;
				}
			}
		}

		return index;
	}
};


class Camera
{
protected:
	int w;
	int h;

	Vec3<float> origin;

	float rotHor;
	float rotVer;
	Mat3<float> rotation;
public:
	Camera(int _w, int _h, const Vec3<float> & _origin, float _rotHor, float _rotVer) 
	: w(_w), h(_h) {

		origin = _origin;
		setRotation(_rotHor, _rotVer);
	}
	virtual ~Camera() {};

	Vec3<float> getOrigin() const { return origin; }
	void setOrigin(Vec3<float> _origin) { origin = _origin; }
	void updateOrigin(Vec3<float> diffOrigin) { origin += (rotation * diffOrigin); }
	float getRotationHorizontal() const { return rotHor; }
	float getRotationVertical() const { return rotVer; }
	void setRotation(const float _rotHor, const float _rotVer) {
		rotHor = fmin(M_PI / 2, fmax(-M_PI / 2, _rotHor));
		rotVer = _rotVer;
		rotation = Mat3Identity<float>().rotateVer(rotVer).rotateHor(rotHor);
	}
	void updateRotation(const float diffHor, const float diffVer) {
		setRotation(rotHor + diffHor, rotVer + diffVer);
	}
	void setRotHorizontal(const float _rotHor) { rotHor = _rotHor; }
	void setRotVertival(const float _rotVer) { rotVer = _rotVer; }

	virtual void resize(int _w, int _h) = 0;
	virtual Ray getRay(int _w, int _h) const = 0;
};

class Cam_Std : public Camera
{
private:
	float viewPort;
	float pixelSize;

public:
	Cam_Std(int _w, int _h, const Vec3<float> & _origin, float _rotHor, float _rotVer, float _viewPort)
	: Camera(_w, _h, _origin, _rotHor, _rotVer) {

		viewPort = _viewPort;
		resize(w, h);
	}

	virtual void resize(int _w, int _h) {

		w = _w;
		h = _h;

		pixelSize = tan(viewPort / 2.0) / w;
	}
	
	virtual Ray getRay(int x, int y) const {

		int relX = x - w / 2;
		int relY = y - h / 2;
		Vec3<float> direction(relX * pixelSize, relY * pixelSize, 1);

		return Ray(origin, rotation * direction);
	}
};

#endif