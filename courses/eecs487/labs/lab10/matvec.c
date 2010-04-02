#ifdef __APPLE__
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

#include <math.h>
#include <stdio.h>
#include "matvec.h"


/* Copy a to b */
void copy_vec(GLfloat a[4], GLfloat b[4])
{
  b[0] = a[0];
  b[1] = a[1];
  b[2] = a[2];
  b[3] = a[3];
}


/* swap a and b */
void swap_vec(GLfloat a[4], GLfloat b[4])
{
  GLfloat temp;
  temp = a[0];
  a[0] = b[0];
  b[0] = temp;
  temp = a[1];
  a[1] = b[1];
  b[1] = temp;
  temp = a[2];
  a[2] = b[2];
  b[2] = temp;
  temp = a[3];
  a[3] = b[3];
  b[3] = temp;
}


/* Make the matrix the zero matrix */
void zero_mat(GLfloat mat[4][4])
{
  int i;
  for(i=0; i<4; ++i)
    mat[i][0] = mat[i][1] = mat[i][2] = mat[i][3] = 0.0;
}


/* Make the matrix the identity matrix */
void identity_mat(GLfloat mat[4][4])
{
  int i;
  zero_mat(mat);
  for(i=0; i<4; ++i) mat[i][i] = 1.0;
}



/* Computes y = Ax where x,y are vectors and A is matrix */

void mat_vec_mult(GLfloat A[4][4], /* in  */
		  GLfloat x[4],    /* in  */
		  GLfloat y[4])    /* out */
{
  /* store in temp array first because in and out may overlap */
  GLfloat temp[4];
  temp[0] = A[0][0]*x[0] + A[0][1]*x[1] + A[0][2]*x[2] + A[0][3]*x[3];
  temp[1] = A[1][0]*x[0] + A[1][1]*x[1] + A[1][2]*x[2] + A[1][3]*x[3];
  temp[2] = A[2][0]*x[0] + A[2][1]*x[1] + A[2][2]*x[2] + A[2][3]*x[3];
  temp[3] = A[3][0]*x[0] + A[3][1]*x[1] + A[3][2]*x[2] + A[3][3]*x[3];

  copy_vec(temp, y);
}



/* Normalize x and store in y. x can be the same as y */
void normalize_vec(GLfloat x[4], /* in  */
		   GLfloat y[4]) /* out */
{
  GLfloat length = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
  y[0] = x[0]/length;
  y[1] = x[1]/length;
  y[2] = x[2]/length;
}


/* a dot b */
GLfloat dot(GLfloat a[4],  /* in */
	    GLfloat b[4])  /* in */
{
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}


/* c = a x b */
void cross(GLfloat a[4],  /* in  */
	   GLfloat b[4],  /* in  */
	   GLfloat c[4])  /* out */
{
  c[0] = a[1]*b[2] - a[2]*b[1];
  c[1] = a[2]*b[0] - a[0]*b[2];
  c[2] = a[0]*b[1] - a[1]*b[0];
  c[3] = 0.0;
}


/* A_T = transpose(A) */
void transpose(GLfloat A[4][4], GLfloat A_T[4][4])
{
  int i;
  for(i=0; i<4; ++i) {
    A_T[i][0] = A[0][i];
    A_T[i][1] = A[1][i];
    A_T[i][2] = A[2][i];
    A_T[i][3] = A[3][i];
  }
}


/* result = a*vec_a + b*vec_b */
void lin_comb2(GLfloat a, GLfloat vec_a[4], GLfloat b, GLfloat vec_b[4], GLfloat result[4])
{
  lin_comb4(a, vec_a, b, vec_b, 0.0, vec_b, 0.0, vec_b, result);
}

/* result = a*vec_a + b*vec_b + c*vec_c */
void lin_comb3(GLfloat a, GLfloat vec_a[4], GLfloat b, GLfloat vec_b[4], GLfloat c, GLfloat vec_c[4], GLfloat result[4])
{
  lin_comb4(a, vec_a, b, vec_b, c, vec_c, 0.0, vec_c, result);
}



/* result = a*vec_a + b*vec_b + c*vec_c + d*vec_d */
void lin_comb4(GLfloat a, GLfloat vec_a[4], GLfloat b, GLfloat vec_b[4],
	       GLfloat c, GLfloat vec_c[4], GLfloat d, GLfloat vec_d[4],
	       GLfloat result[4])
{
  result[0] = a*vec_a[0] + b*vec_b[0] + c*vec_c[0] + d*vec_d[0];
  result[1] = a*vec_a[1] + b*vec_b[1] + c*vec_c[1] + d*vec_d[1];
  result[2] = a*vec_a[2] + b*vec_b[2] + c*vec_c[2] + d*vec_d[2];
  result[3] = a*vec_a[3] + b*vec_b[3] + c*vec_c[3] + d*vec_d[3];
}


void copy_mat(GLfloat A[4][4], GLfloat A_copy[4][4])
{
  int i,j;
  for(i=0; i<4; ++i)
    for(j=0; j<4; ++j)
      A_copy[i][j] = A[i][j];
}



void mat_mat_mult(GLfloat A[4][4], GLfloat B[4][4], GLfloat result[4][4])
{
  int i,j,k;
  GLfloat sum;

  for(i=0; i<4; ++i) {
    for(j=0; j<4; ++j) {
      sum = 0.0;
      for(k=0; k<4; ++k)
	sum += A[i][k] * B[k][j];
      result[i][j] = sum;
    }
  }
}


/* A_inv = inverse of A. No check performed to see if inverse exists */
void invert_mat(GLfloat A[4][4], GLfloat A_inv[4][4])
{
  GLfloat div;
  int i, i1,j;
  GLfloat A_copy[4][4];

  copy_mat(A, A_copy); /* don't destroy the input */

  /* initialize the inverse to the identity matrix */
  identity_mat(A_inv);

  /* systematically perform elementary row operations on A and A_inv so that
   * A becomes the identity matrix and A_inv becomes the inverse */

  for(i=0; i<4; ++i) {
    /* Divide through by A[i][i] to get 1 on the diagonal */
    div = A_copy[i][i];
    for(j=0; j<4; ++j) {
      A_copy[i][j] /= div;
      A_inv[i][j]  /= div;
    }

    /* Subtract a factor times this row from other rows to get zeros above and below the 1 just created by division */
    for(i1=0; i1<4; ++i1) {
      if(i1 == i) continue; /* skip the present row */
      div = A_copy[i1][i];
      for(j=0; j<4; ++j) {
	A_copy[i1][j] -= div*A_copy[i][j];
	A_inv[i1][j]  -= div*A_inv[i][j];
      }
    }
  }
}


void fprintf_mat(FILE *stream, GLfloat A[4][4])
{
  int i,j;

  for(i=0; i<4; ++i) {
    for(j=0; j<4; ++j) {
      fprintf(stream, "%10.3f", A[i][j]);
    }
    fprintf(stream, "\n");
  }
}


void printf_mat(GLfloat A[4][4])
{
  fprintf_mat(stdout, A);
}
