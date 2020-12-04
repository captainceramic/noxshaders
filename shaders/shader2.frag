#version 100

/* This is the random 2D noise function from the book of shaders */
/* See: https://thebooksofshaders.com/10 */
precision mediump float;

uniform vec2 u_resolution;
uniform float u_time;

const vec2 seed_vector = vec2( 15.262, 482.1 );

/* function to make a pseudorandom uniform (0, 1) from a 2D position vector
   and a given floating point time value */
float random (vec2 st, float time_point) {
  
  float seed_dot = dot( st.xy, seed_vector );

  // sin of the dot product (radians)
  float sin_val = sin( seed_dot );
   
  return fract( 10000.0 * time_point * sin( seed_dot ) );
}


void main() {

  /* 2D vector in range 0-1 */
  vec2 st = gl_FragCoord.xy / u_resolution.xy ;

  /* 'random' number */
  float rnd = random( st, u_time );
  
  gl_FragColor = vec4(vec3( rnd ), 1.0);
  
}
