#version 330 compatibility

uniform float	uTime;				// "Time", from Animate( )
uniform float   uKa, uKd, uKs;		// coefficients of each type of lighting
uniform vec3	uSpecularColor;		// light color
uniform float   uShininess;			// specular exponent

uniform float	uRad;				// Radius of circle pattern
uniform float	uS0;
uniform float	uT0;

in  vec2  vST;			// texture coords
in  vec3  vN;			// normal vector
in  vec3  vL;			// vector from point to light
in  vec3  vE;			// vector from point to eye

void
main( )
{

	vec3 Normal = normalize(vN);
	vec3 Light  = normalize(vL);
	vec3 Eye    = normalize(vE);

	vec3 myColor = vec3( 1., 0., 1. );

	// If current texture coordinate vST is within circle with center at uS0, uT0 with
	// radius of uRad
	// Basically, if we have a circle, each pixel needs to ask if it is within the circle
	// Or asking if the pixel is within area of the circle
	// Equation of circle: (x-center_x)^2 + (y - center_y)^2 < radius^2
	if( pow( vST.s - uS0, 2 ) + pow( vST.t - uT0, 2 ) <= pow( uRad, 2) )
	{
		myColor = vec3( 0., 1., 0. );
	}
	
	vec3 ambient = uKa * myColor;

	float d = max( dot(Normal,Light), 0. );       // only do diffuse if the light can see the point
	vec3 diffuse = uKd * d * myColor;

	float s = 0.;
	if( dot(Normal,Light) > 0. )	          // only do specular if the light can see the point
	{
		vec3 ref = normalize(  reflect( -Light, Normal )  );
		s = pow( max( dot(Eye,ref),0. ), uShininess );
	}
	vec3 specular = uKs * s * uSpecularColor;
	
	gl_FragColor = vec4( ambient + diffuse + specular,  1. );
	
}
