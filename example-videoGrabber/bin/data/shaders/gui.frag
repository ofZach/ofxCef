#version 120

varying vec2 Texcoord;
//varying vec4 outputColor;

uniform sampler2D tex;

void main() {
  
  gl_FragColor = texture2D(tex, Texcoord);
  if (gl_FragColor.a < 0.1)
  {
    discard;
  }
}
