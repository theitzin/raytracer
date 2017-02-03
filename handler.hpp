#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <GL/glut.h>
#include <cmath>
#include "world.hpp"

using namespace std;

class DrawMode
{
protected: 
	const Camera * camera;
	const World * world;

	int win_height;
	int win_width;
	int win_pow2;

	float * texture;
	GLuint textureID;

	bool done;

public:
	DrawMode(Camera * _camera, World * _world) : camera(_camera), world(_world) {};
	virtual ~DrawMode() {

		delete[] texture;
	};

	bool finished() const { return done; }
	void draw() const { 

		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glTexCoord2i(0, 0);
		glVertex2i(0, 0);
		glTexCoord2i(1, 0);
		glVertex2i(win_width, 0);
		glTexCoord2i(1, 1);
		glVertex2i(win_width, win_height);
		glTexCoord2i(0, 1);
		glVertex2i(0, win_height);
		glEnd();
		glFlush();
		glDisable(GL_TEXTURE_2D);

	}
	void setFinishedState(bool state) { done = state; }
	void updateWindowSize(int width, int height) {
		
		delete[] texture;

		win_width = width;
		win_height = height;
		win_pow2 = 1;
		while (win_pow2 < win_width || win_pow2 < win_height)
			win_pow2 *= 2;

		glMatrixMode(GL_PROJECTION); 
    	glLoadIdentity();
    	gluOrtho2D(0.0, win_width, 0.0, win_height);

		texture = new float[win_pow2 * win_pow2 * 3];

		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexSubImage2D(
			GL_TEXTURE_2D, 
			0, 
			GL_RGB32F, 
			win_pow2, 
			win_pow2, 
			0, 
			GL_RGB, 
			GL_FLOAT, 
			texture);

	}
	virtual void updateWindowContent() = 0;
	virtual void drawNext() = 0;
};

class DM_Iterative : public DrawMode
{
private:
	int win_size_pad;
	int tile_size;

	int tile_bottom;
	int tile_left;

public: 
	DM_Iterative(Camera * _camera, World * _world) : DrawMode(_camera, _world) {}

	virtual void updateWindowContent() {

		int tmp = 1;
		while (tmp < win_height || tmp < win_width)
			tmp <<= 1;
		win_size_pad = tmp;
		tile_size = win_size_pad;

		tile_bottom = 0;
		tile_left = 0;

		done = false;
	}
	virtual void drawNext() {

		Ray ray = camera->getRay(tile_left, tile_bottom);
		Color color = world->getColor(ray);

		drawRect(	min(win_width - 1, tile_left + tile_size), 
					tile_bottom, 
					min(win_width - 1, tile_left + tile_size + tile_size), 
					min(win_height - 1, tile_bottom + tile_size),
					color);

		drawRect(	tile_left, 
					min(win_height - 1, tile_bottom + tile_size), 
					min(win_width - 1, tile_left + tile_size), 
					min(win_height - 1, tile_bottom + tile_size + tile_size),
					color);

		drawRect(	min(win_width - 1, tile_left + tile_size), 
					min(win_height - 1, tile_bottom + tile_size),
					min(win_width - 1, tile_left + tile_size + tile_size), 
					min(win_height - 1, tile_bottom + tile_size + tile_size),
					color);

		tile_bottom += tile_size + tile_size;

		if (tile_bottom >= win_height) {
			tile_bottom = 0;
			tile_left += tile_size + tile_size;
		}
		if (tile_left >= win_width) {
			tile_bottom = 0;
			tile_left = 0;

			if (tile_size <= 1)
				done = true;
			else
				tile_size >>= 1;
		}
	}
	void drawRect(int left, int bottom, int right, int top, const Color & color) {

		for (int i = bottom; i < top; i++) {
			for (int j = left; j < right; j++) {

				int index = (i * win_pow2 + j) * 3;
				texture[index] = color.r;
				texture[index + 1] = color.g;
				texture[index + 2] = color.b;
			}
		}
	}
};

class Handler
{
private:
	int batch_size;
	int window_width;
	int window_height;

public:
	World * world;
	Camera * camera;
	DrawMode * drawmode;

	Handler() {

		batch_size = 1000;
		window_width = 1500;
		window_height = 1000;

		Vec3<float> origin(-14.0f, 40.0f, -40.0f);
		float rotationHorizontal = 0.68f;
		float rotationVertical = 0.25f;
		float viewPort = 1.5f;

		camera = new Cam_Std(	
			window_width, 
			window_height, 
			origin, 
			rotationHorizontal, 
			rotationVertical, 
			viewPort);

		world = new World();

		// lights

		Color l1Color(1.0f, 1.0f, 1.0f);
		Vec3<float> l1Origin(0.0f, 100.0f, 0.0f);
		world->addLight(new Light(l1Color, l1Origin));

		Color l2Color(1.0f, 1.0f, 1.0f);
		Vec3<float> l2Origin(-30.0f, 50.0f, 0.15f);
		world->addLight(new Light(l2Color, l2Origin));

		// objects

		Surface * p1Surface = new Surface();
		p1Surface->setColor(0.8f, 0.8f, 0.8f);
		p1Surface->setMirror(0.1f);

		Vec3<float> p1Origin(0.0f, 0.0f, 0.0f);
		Vec3<float> p1Normal(0.0f, 1.0f, 0.0f);
		world->addWorldObject(new WO_Plane(p1Surface, p1Origin, p1Normal));


		Surface * p2Surface = new Surface();
		p2Surface->setColor(0.0f, 1.0f, 0.0f);

		Vec3<float> p2Origin(5.0f, 5.0f, 0.0f);
		Vec3<float> p2Normal(-1.0f, 0.5f, -1.0f);
		world->addWorldObject(new WO_Plane(p2Surface, p2Origin, p2Normal));


		Surface * s1Surface = new Surface();
		s1Surface->setColor(0.3f, 0.3f, 1.0f);
		//s1Surface->setShadingModel(0.0f, 0.3f, 0.7f);
		s1Surface->setMirror(0.3f);
		
		Vec3<float> s1Origin(-5.0f, 5.0f, 0.0f);
		float s1Radius = 10.0f;
		world->addWorldObject(new WO_Sphere(s1Surface, s1Origin, s1Radius));


		Surface * s2Surface = new Surface();
		s2Surface->setColor(0.0f, 1.0f, 1.0f);

		Vec3<float> s2Origin(-5.0f, 40.0f, -20.0f);
		float s2Radius = 3.0f;
		world->addWorldObject(new WO_Sphere(s2Surface, s2Origin, s2Radius));


		Surface * s3Surface = new Surface();
		s3Surface->setColor(1.0f, 0.0f, 1.0f);

		Vec3<float> s3Origin(-25.0f, 15.0f, -30.0f);
		float s3Radius = 10.0f;
		world->addWorldObject(new WO_Sphere(s3Surface, s3Origin, s3Radius));


		Surface * s4Surface = new Surface();
		s4Surface->setColor(1.0f, 1.0f, 0.0f);

		Vec3<float> s4Origin(-40.0f, 10.0f, 0.0f);
		float s4Radius = 10.0f;
		world->addWorldObject(new WO_Sphere(s4Surface, s4Origin, s4Radius));

		drawmode = new DM_Iterative(camera, world);

	}

	~Handler() {
		delete world;
		delete camera;
		delete drawmode;
	}

	int getBatchSize() { return batch_size; }
	int getWindowWidth() { return window_width; }
	int getWindowHeight() { return window_height; }
	void glInit() { 
		drawmode->updateWindowSize(window_width, window_height); 
		drawmode->updateWindowContent();
		glScalef(1.f, -1.f, 1.f);
	}
	void resize(int width, int height) {
		window_width = width;
		window_height = height;
		camera->resize(width, height);
		drawmode->updateWindowSize(width, height);
		drawmode->updateWindowContent();
	}
};

#endif