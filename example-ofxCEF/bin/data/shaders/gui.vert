#version 120

uniform vec2 position;
varying vec2 Texcoord;

void main() {
  Texcoord = (vec2(position.x + 1.0, position.y - 1.0) * 0.5);
  Texcoord.y *= -1.0;
  gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}
