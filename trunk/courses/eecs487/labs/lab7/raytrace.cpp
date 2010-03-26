#include <iostream>
#include <map>
#include <vector>

extern "C" {
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif
}

extern "C" {
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
}

#include "point.h"
#include "sphere.h"
#include "plane.h"
#include "light.h"

using namespace std;

extern int screen_width;
extern int screen_height;

Pt const eye_pos(0,0,900);
Sphere  reflecting_sphere(-100,-80,-100,60);

Color raytrace(Ray const& ray, bool touched_sphere);

void kb(unsigned char key, int x, int y) {

		switch (key) {
				case 'q':
						reflecting_sphere.o.z += 10;
						break;
				case 'e':
						reflecting_sphere.o.z -= 10;
						break;
				case 'w':
						reflecting_sphere.o.y -= 10;
						break;
				case 's':
						reflecting_sphere.o.y += 10;
						break;
				case 'a':
						reflecting_sphere.o.x += 10;
						break;
				case 'd':
						reflecting_sphere.o.x -= 10;
						break;
		}

		glutPostRedisplay();
}


void display(void)
{

  glBegin(GL_POINTS);
  for(int i=0; i<screen_width; ++i) {
    for(int j=0; j<screen_height; ++j) {

      // Create a ray through the screen point
      Pt const screen_pt(double(i-screen_width/2), double(j-screen_height/2), 0);
      Vec const view_vec(screen_pt - eye_pos);
      Ray const view_ray(eye_pos, view_vec);

      // trace the ray and get the color
      Color c = raytrace(view_ray, false);
      
      // set this color
      glColor3f(c.r, c.g, c.b);
      glVertex2f((GLfloat)i, (GLfloat)j);
    }
  }
  glEnd();
  glFlush();
}



Color raytrace(Ray const& ray, bool touched_sphere)
{

  // one diffuse light for the scene
  static Light const light0(Pt(-300,200,900),   // location
			    Color::BLACK,       // ambient
			    .45*Color::WHITE,   // diffuse
			    Color::BLACK,       // specular
			    1, 1.0e-3, 1.0e-4); // attenuation

  // front and back walls: respond to diffuse lighting only, mostly red
  static Material const fb_mat(Color::BLACK, Color(1,.1,.1), Color::BLACK);
  static Plane const back_wall  (-Vec::Z, Pt(0,0,1000), fb_mat);
  static Plane const front_wall ( Vec::Z, Pt(0,0,-3000), fb_mat);

  // left and right walls: respond to diffuse lighting only, mostly green
  static Material const lr_mat(Color::BLACK, Color(.3,1,.3), Color::BLACK);
  static Plane const left_wall  ( Vec::X, Pt(-700,0,0), lr_mat);
  static Plane const right_wall (-Vec::X, Pt(700,0,0),  lr_mat);

  // top and bottom walls: respond to diffuse lighting only, mostly blue
  static Material const tb_mat(Color::BLACK, Color(.3,.3,1), Color::BLACK);
  static Plane const bottom_wall( Vec::Y, Pt(0,-600,0), tb_mat);
  static Plane const top_wall   (-Vec::Y, Pt(0,600,0),  tb_mat);

	static vector<const Plane *> planemap(6); // Map for the planes
	static vector<const Material *> matmap(6); // Map for the materials

	planemap[0] = &back_wall;
	planemap[1] = &front_wall;
	planemap[2] = &left_wall;
	planemap[3] = &right_wall;
	planemap[4] = &bottom_wall;
	planemap[5] = &top_wall;

	matmap[0] = matmap[1] = &fb_mat;
	matmap[2] = matmap[3] = &lr_mat;
	matmap[4] = matmap[5] = &tb_mat;

	Color color;
  

  if(touched_sphere || !reflecting_sphere.is_intersecting(ray)) {

    // ** TODO **

    // If we've already touched the sphere or the ray doesn't intersect
    // the sphere then find out which wall the ray is intersecting and obtain
    // its color. To do this,
    // (1) call Plane::intersect(ray) for each of the walls so that you get the t
    //     parameter for the ray at which the intersection occurs. Negative t tells
    //     you that the plane is behind the origin of the ray (the eye). Also, t
    //     is an indicator of how far one must travel along the ray to reach the
    //     point of intersection.
    // (2) Once you get the 't', computing the actual world point can be done
    //     by calling Ray::pt(t). See point.h
    // (3) Keep track of the most desirable candidate for 't' from among the walls,
    //     and also of the corresponding wall
    // (4) Calculate the color at the intersection point from the best wall candidate
    //     by calling Light::pt(Pt, plane-normal, plane-material). See plane.h
    // (5) [optional] Basic shadow: If the ray from light source to point on the wall intersects
    //     the sphere, then the sphere casts a shadow. Otherwise the wall reflects light
    //     in the usual way. This should be an easy modification.
	
		
		double t[6];
		double t_c = INFINITY; // closest t
		int	idx_c; // closest index

		//Intersection with each plane
		for (int i = 0; i < 6; i++) {
				t[i] = planemap[i]->intersect(ray);
				//if (touched_sphere) cout << "t" << i << ": " << t[i] << endl;
				if (t[i] >= 0 && t[i] < t_c) {
						t_c = t[i];
						idx_c = i;
				}
		}

		Pt intPt = ray.pt(t_c);
		Ray shadow = Ray(intPt, light0.o - intPt);
		if (reflecting_sphere.is_intersecting(shadow))
				color = Color::BLACK;
		else
				color = light0.pt(intPt, planemap[idx_c]->n, *matmap[idx_c]);

		/*if (touched_sphere) {
				cout << "* color: " << color.r << " " << color.g << " " << color.b << endl;
				cout << "* t_c: " << t_c << " idx_c: " << idx_c << endl;
		}*/
  }
  else {

    // ** TODO **

    // ray intersects sphere:
    // (1) Determine the point of intersection. Use Sphere::intersect(ray)
    // (2) Determine reflection vector.
    // (3) Create the reflected ray with this point and direction.
    // (4) recurse this function setting the touched_sphere flag as true.

		Ray nray = ray;
		nray.v = nray.v.normalized();
		Pt intPt = nray.pt(reflecting_sphere.intersect(nray)); // Intersection point
		//cout << "Point: " << intPt.x << " " << intPt.y << " " << intPt.z << endl;
		Vec norm = reflecting_sphere.unit_normal(intPt);
		Vec refl = nray.v - 2 * norm * dot(norm, nray.v);
		refl = refl.normalized();
		Ray ray_reflected(intPt, refl);

		//cout << ray_reflected << endl;

		color = raytrace(ray_reflected, true);
		//cout << "* color: " << color.r << " " << color.g << " " << color.b << endl;

  }
  
  return color;
}

