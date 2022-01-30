#version 120

uniform sampler2D samp;

void main(void) {
    gl_FragColor = texture2D(samp, gl_TexCoord[0].st);    
}