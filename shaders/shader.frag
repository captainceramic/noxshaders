#version 100

precision mediump float;

uniform float u_time;

float colval;

void main() {

  colval = 0.5 + (0.5 * sin(u_time));

  gl_FragColor = vec4(0.0, 0.5, colval, 1.0);
}
