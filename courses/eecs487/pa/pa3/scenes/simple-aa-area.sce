sce1.0

# camera
eyepos 0 -2 1.5
eyedir 0 1 -0.4
eyeup 0.0 0.0 1.0
wdist 1.0
fovy_deg 50

nx 600
ny 300

#options

max_recursion 4
aasample 4
		
# scene

background 0 0 0.6

ca 0.1 0.1 0.1

{
	#ground
	cr 0.4 0.5 0.4 
	cp 0.4 0.4 0.4
 
	triangle -3 -2 0  3 -2 0  3 10 0
	triangle -3 -2 0  3 10 0 -3 10 0
}

#spheres

{
	ca 0.2 0 0
	cr 0.5 0.5 0.5
	cp 0.5 0.5 0.5
	shininess 100


	push_matrix
		translate -0.5 2.0 1.0
		rotate 60 1 1 0
		scale 1 1 1.6

		ball 0.3 0 0 0
	pop_matrix

	ca 0.0 0.2 0
	cr 1 0.4 0.4
	cp 0.2 0.2 0.2

	ball 0.3 0.5 2.0 1.0

}

{	
	translate -2 0.6 -0.3
	rotate 25 0 1 0
	scale 0.1 0.1 1.5
	cylinder 
}

{
	translate 1 0 0
	#pointlight 3 0 4   0.8 0.8 0.6
}

{
	translate -1 0 5
	arealight 0.3 0.8 0.8 0.6
}

end