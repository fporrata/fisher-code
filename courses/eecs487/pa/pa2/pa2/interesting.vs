varying vec3 normal_eye;

void main(void) {
  normal_eye = normalize(gl_NormalMatrix*gl_Normal);
  gl_Position = ftransform();
}