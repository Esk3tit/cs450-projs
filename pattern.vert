#version 330 compatibility

uniform float		uTime;	// "Time", from Animate( )
uniform float		uX;		// Modify x coord to distort
uniform float		uY;		// Modify y coord to distort
uniform float		uZ;		// Modify z coord to distort

out		vec2  		vST;	// texture coords
out		vec3		vN;		// normal vector
out		vec3		vL;		// vector from point to light
out		vec3		vE;		// vector from point to eye

vec3 LightPosition = vec3(  0., 5., 5. );

const float PI = 	3.14159265;
const float AMP = 	0.2;		// amplitude
const float W = 	2.;		// frequency

void
main( )
{ 
	vST = gl_MultiTexCoord0.st;
	vec3 vert = gl_Vertex.xyz;

	// Animate vertex stuff here based on uniform vars
	vert.x = -tan( uX + sin( gl_Vertex.x ) );
	vert.y = gl_Vertex.y * cos( uY );
	vert.z = gl_Vertex.z;

	vec4 ECposition = gl_ModelViewMatrix * vec4( vert, 1. );
	vN = normalize( gl_NormalMatrix * gl_Normal );				// normal vector
	vL = LightPosition - ECposition.xyz;						// vector from the point
																// to the light position
	vE = vec3( 0., 0., 0. ) - ECposition.xyz;					// vector from the point
																// to the eye position 
	gl_Position = gl_ModelViewProjectionMatrix * vec4( vert, 1. );
}
