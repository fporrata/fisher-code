varying vec4 pos_eye;

void main(void) {
  pos_eye = gl_ModelViewMatrix * gl_Vertex;
  gl_Position = ftransform();
}
