varying vec3 normal;
varying vec3 lightVec;
varying vec3 viewVec;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
		vec4 vert = gl_ModelViewMatrix * gl_Vertex;

	normal = normalize(gl_NormalMatrix * gl_Normal);
	vec3 lightPos = vec3(gl_LightSource[1].position);
	lightVec = vec3(lightPos - vert.xyz);
	viewVec = -vec3(vert);
}
