varying vec3 reflect;
varying vec3 refract;
varying float ratio;

varying vec3 normal;
varying vec3 lightVec;
varying vec3 viewVec;

uniform samplerCube Cubemap;

void main(void) {
  	vec3 refractColor = vec3(textureCube(Cubemap, refract));
	vec3 reflectColor = vec3(textureCube(Cubemap, reflect));

	vec3 color = mix(refractColor, reflectColor, ratio);

    vec3 N = normal;
    vec3 V = normalize(viewVec);
    vec3 L = normalize(lightVec);
    vec3 H = normalize(L + V);

    float NdotH = clamp(dot(N, H), 0.0, 1.0);

    vec4 specular = gl_FrontLightProduct[0].specular * pow(NdotH, gl_FrontMaterial.shininess);

	gl_FragColor = vec4(color, 0.1) + specular;
}