attribute vec4 a_vertices;
attribute vec2 a_textureCoordinates;
attribute vec4 a_textureColor;

varying mediump vec2 v_textureCoordinates;
varying lowp vec4 v_textureColor;

uniform mat4 u_modelViewProjection;

void main()
{
  v_textureCoordinates = a_textureCoordinates;
  v_textureColor = a_textureColor;
  gl_Position = u_modelViewProjection * a_vertices;
}