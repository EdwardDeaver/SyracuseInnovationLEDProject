#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main( void ) {

	vec2 position = gl_FragCoord.xy;

	gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);

	float x = floor(position.x / 5.);
	if (mod(position.x, 5.) < 2.) {
		return;
	}

	float y = floor(position.y / 5. + sin(-time*2. + x*5.)*20.);

	float dist = (floor(resolution.y/10.) - y);



	float c = 1./(1.+dist);
	c /= (1.+pow(sin(-time*2. + x*-1.5), 2.));
	gl_FragColor = vec4(c-0.07, c, c, 0.5);
}
