varying vec3 Refract;

varying vec3 normal;
varying vec3 lightVec;
varying vec3 viewVec;
uniform sampler2D tex;
uniform sampler2D sqmap;

void main(void) {
	vec3 refractColor = Refract;
	refractColor.x = refractColor.x - 0.5;
	refractColor.y = refractColor.y + 0.5;
	refractColor = vec3(texture2D(sqmap, refractColor.xy));
	
    //gl_FragColor = gl_Color * texture2D(tex, gl_TexCoord[0].xy);
    vec3 N = normal;
    vec3 V = normalize(viewVec);

    vec3 L = normalize(lightVec);
    vec3 H = normalize(L + V);


    float NdotL = dot(N, L); //clamp(dot(normal, lightVec), 0.0, 1.0);
    float NdotH = clamp(dot(N, H), 0.0, 1.0);

    vec4 amb = gl_FrontLightProduct[0].ambient * gl_FrontMaterial.diffuse;
    vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(NdotL, 0.0);
    vec4 specular = gl_FrontLightProduct[0].specular * pow(NdotH, gl_FrontMaterial.shininess);
    vec4 result = diffuse + specular + amb;
	gl_FragColor = vec4(refractColor, 1.0) + specular;
}
