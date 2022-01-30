#version 120

uniform sampler2D samp;

void main(void) {
    //gl_FragColor = texture2D(samp, gl_TexCoord[0].st);
    vec4 color = texture2D(samp, gl_TexCoord[0].st);
    gl_FragColor = vec4(0.1, 0.0, 0.0, 1.0) + color;
}