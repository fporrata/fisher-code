varying vec3 normal_eye;

void main(void) {
  float nl = dot(normalize(gl_LightSource[0].position.xyz), normalize(normal_eye));
  gl_FragColor.rgb = ( nl<0.2 ? vec3(0.3, 0.3, 0) 
                              : (nl<0.8 ? vec3(0.7, 0.7, 0.2) 
                                        : vec3(0.8, 0.8, 0.4)));
  //gl_FragColor.rgb = abs(normalize(normal_eye));
  gl_FragColor.a = 1.0;
}