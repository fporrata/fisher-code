sce1.0

# camera
eyepos 1 -1.5 0.5
eyedir 0 1 -0.10
eyeup 0.0 0.0 1.0
wdist 1.0
fovy_deg 50

nx 400
ny 300

#options

max_recursion 4
aasample 0
# scene

background 0 0 0.6

ca 0.1 0.1 0.1

rotate -15 0 0 1

{
	#ground
	cr 0.4 0.5 0.4 
	cp 0.4 0.4 0.4
 
	triangle -3 -10 0  3 -10 0  3 10 0
	triangle -3 -10 0  3 10 0 -3 10 0
}

{	
	cr 0.6 0.6 0.6 
	cp 0.4 0.4 0.4

	translate 1 2.9 -0.3
	#rotate 25 0 1 0
	scale 0.5 0.5 2
	ball 1 0 0 0 
}

{
	translate 0.4 0.3 -0.56
	rotate 90 1 0 0
	scale 0.7 0.7 0.7
	cr 0.5 0.4 0.9 
	cp 0.0 0.0 0.0
	object_phong feline100.ob
}

{
	translate -1 -1 5
	pointlight 3 0 4   0.8 0.8 0.6
}

end