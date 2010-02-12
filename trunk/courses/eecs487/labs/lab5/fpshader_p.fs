uniform sampler2D tex;

varying vec3 normal;
varying vec3 lightVec;
varying vec3 viewVec;

void main()
{
	gl_FragColor = gl_Color * texture2D(tex, gl_TexCoord[0].xy);
	vec3 N = normal;
	vec3 L = normalize(lightVec);
	vec3 V = normalize(viewVec);
	vec3 H = normalize(L + V);

	float NdotL = dot(N, L); //clamp(dot(normal, lightVec), 0.0, 1.0);
	float NdotH = clamp(dot(N, H), 0.0, 1.0);

	vec4 amb = gl_FrontLightProduct[1].ambient;
	vec4 diffuse = gl_FrontLightProduct[1].diffuse * max(NdotL, 0.0);
	vec4 specular = gl_FrontLightProduct[1].specular * pow(NdotH, 0.5*gl_FrontMaterial.shininess);
	vec4 result = diffuse + specular + amb;
	gl_FragColor = result;
}
