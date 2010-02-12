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

	vec3 N = normal;
	vec3 L = normalize(lightVec);
	vec3 V = normalize(viewVec);
	vec3 H = normalize(L + V);

	float NdotL = dot(N, L); //clamp(dot(normal, lightVec), 0.0, 1.0);
	float NdotH = clamp(dot(N, H), 0.0, 1.0);

	vec4 amb = gl_FrontLightProduct[1].ambient;
	vec4 diffuse = gl_FrontLightProduct[1].diffuse * max(NdotL, 0.0);
	vec4 specular = gl_FrontLightProduct[1].specular * pow(NdotH, 64.0);
	vec4 result = diffuse + specular + amb;
	gl_FrontColor = vec4(result);
}