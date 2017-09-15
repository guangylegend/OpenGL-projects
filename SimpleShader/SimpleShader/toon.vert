attribute vec3 vColor;

varying vec3 color;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    color = vColor;
}