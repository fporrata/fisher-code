varying vec3 normal;
varying vec3 lightVec[gl_MaxLights];
varying vec3 viewVec;
uniform sampler2D tex;


void main(void) {
    gl_FragColor = gl_Color * texture2D(tex, gl_TexCoord[0].xy);
    vec3 N = normalize(normal);
    vec3 V = normalize(viewVec);

	for (int i = 0; i < gl_MaxLights; i++) {
		vec3 L = normalize(lightVec[i]);
		vec3 H = normalize(L + V);


		float NdotL = clamp(dot(N, L), 0.0, 1.0);
		float NdotH = clamp(dot(N, H), 0.0, 1.0);

		vec4 amb = gl_FrontLightProduct[i].ambient * gl_FrontMaterial.diffuse;
		vec4 diffuse = gl_FrontLightProduct[i].diffuse * max(NdotL, 0.0);
		vec4 specular = gl_FrontLightProduct[i].specular * pow(NdotH, gl_FrontMaterial.shininess);
		vec4 result = diffuse + specular + amb;
		gl_FragColor += result;
	}
  
}
