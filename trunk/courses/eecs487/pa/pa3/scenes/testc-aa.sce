sce1.0

# camera
eyepos 1 -3 1.5
eyedir -0.2 1 -0.3
eyeup 0.0 0.0 1.0
wdist 1.0
fovy_deg 45

nx 400
ny 300

#options

max_recursion 4
aasample 4
		
# scene

background 0 0 0.6

ca 0.1 0.1 0.1

rotate 36 0 0 1

{
	#ground
	cr 0.4 0.5 0.4 
	cp 0.4 0.4 0.4
 
	triangle -3 -10 0  3 -10 0  3 10 0
	triangle -3 -10 0  3 10 0 -3 10 0
}



{	
	translate -1 0.6 -0.3
	rotate 25 0 1 0
	scale 0.1 0.1 1.5
	cylinder 
}

{
	#translate 1 1 0
	#rotate -45 0 0 1
	scale 0.5 0.5 0.5
	cr 0.8 0.4 0.4 
	cp 0.0 0.0 0.0
	object_flat cube.ob
}

{
	translate 0.3 -0.3 0.4
	rotate 25 0 1 0.3
	scale 0.5 0.5 0.5
	cr 0.5 0.4 0.9 
	cp 0.0 0.0 0.0
	object_flat cube.ob
}

{
	translate -1 -1 5
	pointlight 3 0 4   0.8 0.8 0.6
}

end