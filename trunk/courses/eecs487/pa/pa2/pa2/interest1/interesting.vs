const float Eta =  0.4;
const float FresnelPower = 4.0;

varying vec3 normal;
varying vec3 lightVec;
varying vec3 viewVec;

const float F = ((1.0 - Eta) * (1.0 - Eta)) / ((1.0+Eta) * (1.0 + Eta));

varying vec3 reflect;
varying vec3 refract;
varying float ratio;

void main(void) {
  	vec4 pos = gl_ModelViewMatrix * gl_Vertex;
	vec3 pos3 = pos.xyz / pos.w;
	
	vec3 i = normalize(pos3);
	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	
	ratio = F + (1.0 - F) * pow((1.0 - dot(-i, n)), FresnelPower);

	refract = refract(i, n, Eta);
	
	reflect = reflect(i, n);

	gl_Position = ftransform();

	  vec4 vert = gl_ModelViewMatrix * gl_Vertex;

  	normal = normalize(gl_NormalMatrix * gl_Normal);

  		vec3 lightPos = vec3(gl_LightSource[0].position);
  		lightVec = vec3(lightPos - vert.xyz);
  viewVec = -vec3(vert);
}