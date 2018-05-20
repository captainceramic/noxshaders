#version 100

precision mediump float;

// Canvas size (width, height)
uniform vec2 u_resolution;
// Time (in seconds) since load.
uniform float u_time;

void main() {

  // Scale and centre.
  vec2 st = 2.0 * ((gl_FragCoord.xy / u_resolution) - 0.5);

  float r = st.x * sqrt(pow(st.x, 2.0) + pow(st.y, 2.0)) + st.y;

  float valthing = abs(sin(4.0 * (r + 0.25 * u_time)));
  
  gl_FragColor = vec4(abs(sin(u_time)),
		      abs(sin(2.0 * valthing)),
		      abs(cos(u_time)),
		      1.0);

  
}
