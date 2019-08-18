#version 100

precision mediump float;

#define TWO_PI 6.28318530718

// Get the required uniforms
uniform vec2 u_resolution;
uniform float u_time;

// this is a function from the book of shaders
vec3 hsb2rgb( in vec3 c ) {

  vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0, 4.0, 2.0),
			   6.0) - 3.0) - 1.0,
		   0.0,
		   1.0);

  rgb = rgb * rgb * (3.0 - 2.0 * rgb);

  return c.z * mix( vec3(1.0), rgb, c.y);

}


void main() {

  vec2 st = gl_FragCoord.xy / u_resolution;
  vec3 color = vec3(0.0);

  // Use some polar coordinates
  vec2 toCenter = vec2(0.5) - st;
  float angle = atan(toCenter.y, toCenter.x);

  // Make the angle vary with time.
  angle = angle + u_time;
  
  float radius = length(toCenter) * 2.0;

  // Map the angle to the hue and the saturation to the radius
  color = hsb2rgb(vec3((angle/TWO_PI) + 0.5, radius, 1.0));

  float alpha = 1.0;
  if (radius > 1.0) {
    alpha = 0.0;
  }

  gl_FragColor = vec4(color, alpha);
  
}
