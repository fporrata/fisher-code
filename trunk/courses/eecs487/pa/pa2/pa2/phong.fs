varying vec4 pos_eye;

void main(void) {
  gl_FragColor.rgb = pos_eye.xyz;
  gl_FragColor.a = 1.0;
}
