attribute vec4 a_vertices;
attribute vec4 a_sourceColor;
attribute float a_pointSize;

varying lowp vec4 v_destinationColor;

uniform mat4 u_modelViewProjection;

void main()
{
  v_destinationColor = a_sourceColor;
  gl_Position = u_modelViewProjection * a_vertices;
  gl_PointSize = a_pointSize;
}