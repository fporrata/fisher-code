
#ifndef __SPLINE_H__
#define __SPLINE_H__

void calculate_basis_from_constraint(GLfloat[4][4], GLfloat[4][4]);
void calculate_catmull_rom_spline(GLfloat[4], GLfloat[4], GLfloat[4], GLfloat[4], GLfloat[4][4]);
void calculate_bezier_curve(GLfloat[4], GLfloat[4], GLfloat[4], GLfloat[4], GLfloat[4][4]);
void evaluate_spline(double, GLfloat[4][4], GLfloat[4]);

#endif

