#version 410 core

uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 ambientLight;

uniform sampler2D normTex;

in vec3 normal;
in vec3 worldPosition;
in vec2 texCoords;

out vec4 out_Color;

const float PI = 3.14159265358979f;


// Lambert Diffuse BRDF
float Fd_Lambert(){
    return 1.0 / PI;
}

// Normal Mapping (TBN matrix construction)
mat3 getTBN(vec3 N){
    vec3 Q1 = dFdx(worldPosition), Q2 = dFdy(worldPosition); // position 미분
    vec2 st1 = dFdx(texCoords), st2 = dFdy(texCoords); // tex coord 미분
    float D = st1.s * st2.t - st1.t * st2.s;

    vec3 T = normalize((Q1 * st2.t - Q2 * st1.t) * D);
    vec3 B = normalize((-Q1 * st2.s + Q2 * st1.s) * D);
    return mat3(T, B, N);
}

void main(void)
{
    vec3 n = normalize(normal);                         // normal unit vector
    vec3 l = normalize(lightPosition - worldPosition);  // light unit vector
    vec3 v = normalize(cameraPosition - worldPosition); // view unit vector
    vec3 h = normalize(l+v);                            // half unit vector

	// normal mapping
	mat3 TBN = getTBN(n);
	vec3 normVec = texture(normTex, texCoords).rgb * 2.0 - 1.0; // [0, 1] -> [-1, 1]
	n = normalize(TBN * normVec);

    // dot products
	float NoV = abs(dot(n, v)) + 1e-5;
	float NoL = clamp(dot(n, l), 0.0, 1.0);
	float NoH = clamp(dot(n, h), 0.0, 1.0);
	float LoH = clamp(dot(l, h), 0.0, 1.0);
	
	// diffuse BRDF
	vec3 baseColor = vec3(1); // base color는 diffTex가 아닌 vec4(1)로 사용
	vec3 Fd = baseColor * Fd_Lambert();

	// lighting
	vec3 radiance = (lightColor/dot(l, l)) * NoL; // light source(attenuation) * cosθ
	vec3 color = Fd * radiance;

	out_Color = vec4(color, 1);
}
