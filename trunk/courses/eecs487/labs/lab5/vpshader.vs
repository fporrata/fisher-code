const float PI = 3.14159265358979323846;
uniform float time;

void
main(void)
{
	float period = 3.0;
	float omega = 2.0 * PI / period;
	vec4 vertices = vec4(gl_Vertex.x * (1.0 + 0.5 * sin(omega * time)),
						gl_Vertex.y * (1.0 + 0.5 * cos(omega * time)),
						gl_Vertex.z, gl_Vertex.w);
//	gl_Vertex.x = gl_Vertex.x * (1.0 + 0.5 * sin(omega * time));
//	gl_Vertex.y = gl_Vertex.y * (1.0 + 0.5 * cos(omega * time)); 

	gl_Position = gl_ModelViewProjectionMatrix *
	vertices;

}