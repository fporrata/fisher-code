#ifndef __MATVEC_H__
#define __MATVEC_H__

#ifdef __APPLE__
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

#ifdef WIN32
	#include <windows.h>
#endif

#include <stdio.h>


/* Matrix-vector utility functions */

void copy_vec(GLfloat a[4], GLfloat b[4]); /* Copy a to b */

void swap_vec(GLfloat a[4], GLfloat b[4]); /* swap a and b */

void zero_mat(GLfloat mat[4][4]); /* Fill matrix with zeros */

void identity_mat(GLfloat mat[4][4]); /* Make the matrix the 4x4 identity */

void mat_vec_mult(GLfloat A[4][4], GLfloat x[4], GLfloat y[4]); /* y = Ax + b */

void normalize_vec(GLfloat x[4], GLfloat y[4]); /* y = unit vector in direction of x */

GLfloat dot(GLfloat a[4], GLfloat b[4]); /* dot product */

void cross(GLfloat a[4], GLfloat b[4], GLfloat c[4]); /* c = a x b */

void transpose(GLfloat A[4][4], GLfloat A_T[4][4]);

/* result = a*vec_a + b*vec_b */
void lin_comb2(GLfloat a, GLfloat vec_a[4], GLfloat b, GLfloat vec_b[4], GLfloat result[4]);


/* result = a*vec_a + b*vec_b + c*vec_c */
void lin_comb3(GLfloat a, GLfloat vec_a[4], GLfloat b, GLfloat vec_b[4], GLfloat c, GLfloat vec_c[4], GLfloat result[4]);

/* result = a*vec_a + b*vec_b + c*vec_c +d*vec_d */
void lin_comb4(GLfloat a, GLfloat vec_a[4],
	       GLfloat b, GLfloat vec_b[4],
	       GLfloat c, GLfloat vec_c[4],
	       GLfloat d, GLfloat vec_d[4],
	       GLfloat result[4]);


void copy_mat(GLfloat A[4][4], GLfloat A_copy[4][4]);

void invert_mat(GLfloat A[4][4], GLfloat A_inv[4][4]); /* Invert A and put into A_inv */

void mat_mat_mult(GLfloat A[4][4], GLfloat B[4][4], GLfloat result[4][4]);

void fprintf_mat(FILE *stream, GLfloat A[4][4]); /* Print A to stream */

void printf_mat(GLfloat A[4][4]); /* Print to stdout (screen) */

#endif
