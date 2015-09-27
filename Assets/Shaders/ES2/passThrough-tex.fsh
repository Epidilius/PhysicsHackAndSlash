varying mediump vec2 v_textureCoordinates;
varying lowp vec4 v_textureColor;

uniform sampler2D u_texture;

void main()
{
    gl_FragColor = texture2D(u_texture, v_textureCoordinates) * v_textureColor;
}