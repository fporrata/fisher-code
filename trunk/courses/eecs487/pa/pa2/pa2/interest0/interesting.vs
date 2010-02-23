varying vec3 Refract;

varying vec3 normal;
varying vec3 lightVec;
varying vec3 viewVec;

void main(void) {
  	vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;
	vec3 ecPosition3 = ecPosition.xyz / ecPosition.w;
	
	gl_Position = ftransform();
	Refract = gl_Position.xyz;
  vec4 vert = gl_ModelViewMatrix * gl_Vertex;

  normal = normalize(gl_NormalMatrix * gl_Normal);

  		vec3 lightPos = vec3(gl_LightSource[0].position);
  		lightVec = vec3(lightPos - vert.xyz);
  viewVec = -vec3(vert);
}
