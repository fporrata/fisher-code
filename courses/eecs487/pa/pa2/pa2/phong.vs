varying vec3 normal;
varying vec3 lightVec[gl_MaxLights];
varying vec3 viewVec;


void main(void) {
  gl_Position = ftransform();

  vec4 vert = gl_ModelViewMatrix * gl_Vertex;

  normal = normalize(gl_NormalMatrix * gl_Normal);

  for (int i = 0; i < gl_MaxLights; i++) {
	  vec3 lightPos = vec3(gl_LightSource[i].position);
	  lightVec[i] = vec3(lightPos - vert.xyz);
  }
  viewVec = -vec3(vert);
  
}
