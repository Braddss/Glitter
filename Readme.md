Resources:

	The following repository was used as a project template:
	https://github.com/Polytonic/Glitter 

	The marching cubes algorithms was implemented with the help of the following resources:

	http://paulbourke.net/geometry/polygonise/
	klacansky.com

	Datasets in the folder Glitter/DataSets are from:
	http://klacansky.com/open-scivis-datasets/

	Perlin Noise resource:
	https://github.com/sol-prog/Perlin_Noise

	Some of the OpenGl-Code:
	https://learnopengl.com/

_____________________________________________________

Controls:

Movement:

	Mouse: Look around
	W: Forward
	S: Backward
	A: Left
	D: Right
	E: Up
	Q: Down


Switch between models:

	1-5
	1: Model created with different overlayed perlin noises
	
	Following models are created with datasets from:
	http://klacansky.com/open-scivis-datasets/
	
	2: Silicium (98x34x34)
	3: Lobster (301x324x56)
	4: Boston Teapot (256x256x178)
	5: Bonsai (256x256x256)

Model operations:

	X: Raise surface level value
	C: Lower surface level value
	(surface values printed to console from min 0.0 to max 1.0)


	(The next six keys only work for model 1)

	V: Raise perlin-noise frequency
	B: Lower perlin-noise frequency
	N: Raise perlin-noise amplitude
	M: Lower perlin-noise amplitude
	,: Raise offset value
	.: Lower offset value

	9: Show/ Hide Pointcloud of current object (pointCloud-points are rendered in a grayscale from black to white corresponding to their given values)
	0: Show/ Hide Marching-Cubes-Mesh of current object


Model transformations:

	I: Translate current object along the x-Axis in the positive direction
	K: ~ x-Axis ~ negative direction
	J: ~ z-Axis ~ negative direction 
	L: ~ z-Axis ~ positive direction
	U: ~ y-Axis ~ negative direction
	O: ~ y-Axis ~ positive direction

	R: Scale up current object
	T: Scale down current object

	F: Rotate current object around the x-Axis
	G: ~ y-Axis
	H: ~ z-Axis
	(Rotation-values will be printed to the console | beware of deadlock)

