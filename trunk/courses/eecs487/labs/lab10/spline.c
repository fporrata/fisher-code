

#include "main.h"
#include "matvec.h"




void calculate_basis_from_constraint(GLfloat C[4][4], GLfloat B[4][4])
{
  invert_mat(C,B);
}


/* Evaluate the spline at the given value of u, given the coefficients */

void evaluate_spline(double u,        /* 0<= parameter <= 1 */
		     GLfloat a[4][4], /* coefficients       */
		     GLfloat f[4]     /* result function    */
		     )
{
  const double u2 = u*u;
  const double u3 = u2*u;
  // const double u4 = u2*u2;
  int i;

  for(i=0; i<4; ++i)
    f[i] = a[0][i] + a[1][i]*u + a[2][i]*u2 + a[3][i]*u3;
}

void calculate_quad_spline(GLfloat p[4][4], GLfloat a[3][4])
{
	GLfloat C[4][4] = 
	{{1, 0, 0, 0},
	 {1, 0.5, 0.25, 0},
	 {1, 1, 1, 0},
	 {0, 0, 0, 1}};

	GLfloat B[4][4];

	calculate_basis_from_constraint(C, B);

	int i;
	for (i = 0; i < 4; ++i)
		lin_comb3(B[i][0], p[0], B[i][1], p[1], B[i][2], p[2], a[i]);
}



/* Given 4 points p[0]...p[3], calculate the coefficients a[0]...a[3] */

void calculate_catmull_rom_spline(GLfloat p0[4], GLfloat p1[4], GLfloat p2[4], GLfloat p3[4], GLfloat a[4][4])
{
  int i;

  GLfloat C[4][4] = /* constraint matrix */
    { { 1, -1,  1,  1 },
      { 1,  0,  0,  0 },
      { 1,  1,  1,  1 },
      { 1,  2,  4,  6 }
    };

  GLfloat B[4][4]; /* basis matrix */
  
  calculate_basis_from_constraint(C, B);

  for(i=0; i<4; ++i)
    lin_comb4(B[i][0], p0, B[i][1], p1, B[i][2], p2, B[i][3], p3, a[i]);
}





void calculate_bezier_curve(GLfloat p0[4], GLfloat p1[4], GLfloat p2[4], GLfloat p3[4], GLfloat a[4][4])
{
  int i;
  GLfloat C[4][4] = /* constraint matrix */
    { {1, 0, 0, 0},
      {1, 1.0/3, 0, 0},
      {1, 2.0/3, 1.0/3, 0},
      {1, 1, 1, 1}
    };
  GLfloat B[4][4]; /* basis matrix */

  calculate_basis_from_constraint(C,B);

  for(i=0; i<4; ++i)
    lin_comb4(B[i][0], p0, B[i][1], p1, B[i][2], p2, B[i][3], p3, a[i]);
  
}
