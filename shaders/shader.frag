#version 100

precision mediump float;

// Canvas size (width, height)
uniform vec2 u_resolution;
// Time (in seconds) since load.
uniform float u_time;

void main() {

  // float colval = abs(sin(u_time));
  // vec2 st = gl_FragCoord.xy / u_resolution;

  // gl_FragColor = vec4(abs(sin((20.0 * st.x) + u_time)),
  // 		      abs(cos(6.5 * st.y)),
  // 		      colval, 1.0);

  // Scale and centre.
  vec2 st = 2.0 * ((gl_FragCoord.xy / u_resolution) - 0.5);

  float r = sqrt(pow(st.x, 2.0) + pow(st.y, 2.0));

  float valthing = abs(sin(4.0 * (r + 0.25 * u_time)));
  
  gl_FragColor = vec4(abs(sin(u_time)), valthing, 0.0, 1.0);

  
}
