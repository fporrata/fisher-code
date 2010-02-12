uniform sampler2D tex;

void main()
{
	float xmod = mod(gl_FragCoord.x, 20.0);
	float ymod = mod(gl_FragCoord.y, 10.0);
	if (xmod >= 5.0 && xmod <= 15.0 && ymod >= 2.0 && ymod <= 8.0)
		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	else
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
