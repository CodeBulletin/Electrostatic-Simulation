#version 130

float k = 8987551792.3;

uniform int Size;
uniform float res;
uniform float dis;
uniform vec2 loc[500];
uniform float charge[500];

in vec4 gl_FragCoord;

out vec4 FragColor;

float map(float value, float min, float max, float minRange, float maxRange) {
	return (minRange + (maxRange - minRange) * ((value - min) / (max - min)));
}

void main(){
	float V = 0;
	for(int i = 0; i < Size; i++){
		V += k*charge[i]/(sqrt(((loc[i].x - gl_FragCoord.x)*(loc[i].x - gl_FragCoord.x))+((res - loc[i].y - gl_FragCoord.y)*(res - loc[i].y - gl_FragCoord.y))) * dis);
	}
	if(V <= 0){
		FragColor = vec4(0.0, 0.0, map(V, -10000.0, 0.0, 1.0, 0.0), 0.5);
	} else {
		FragColor = vec4(map(V, 0.0, 10000.0, 0.0, 1.0), 0.0, 0.0, 0.5);
	}
}