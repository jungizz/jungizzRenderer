#version 410 core

uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
//uniform vec3 ambientLight;

uniform sampler2D diffTex;
uniform sampler2D normTex;
uniform sampler2D roughTex;
uniform sampler2D metalTex;
uniform sampler2D aoTex;

in vec3 normal;
in vec3 worldPosition;
in vec2 texCoords;

out vec4 out_Color;

const float PI = 3.14159265358979f;
const float SKIN_SPEC_REFELCTANCE = 0.028f; // 'skin' specular reflectance at normal incidence angle

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

// Microfacet (Cook-Torrance GGX)
float V_SmithGGXCorrelated(float NoV, float NoL, float roughness) {
    float a2 = roughness * roughness;
    float GGXL = NoV * sqrt((-NoL * a2 + NoL) * NoL + a2);
    float GGXV = NoL * sqrt((-NoV * a2 + NoV) * NoV + a2);
    return 0.5 / (GGXV + GGXL);
}

float D_GGX(float NoH, float roughness) {
    float a2 = roughness * roughness;
    float f = (NoH * a2 - NoH) * NoH + 1.0;
    return a2 / (PI * f * f);
}

vec3 F_Schlick(float u, vec3 f0) {
    return f0 + (vec3(1.0) - f0) * pow(1.0 - u, 5.0);
}


void main()
{
    vec3 n = normalize(normal);                         // normal unit vector
    vec3 l = normalize(lightPosition - worldPosition);  // light unit vector
    vec3 v = normalize(cameraPosition - worldPosition); // view unit vector
    vec3 h = normalize(l+v);                            // half unit vector
    
    // normal mapping
    mat3 TBN = getTBN(n);
    vec3 normVec = texture(normTex, texCoords).rgb * 2.0 - 1.0; // [0,1] -> [-1, 1]
    n = normalize(TBN * normVec);

    // dot products
    float NoV = abs(dot(n, v)) + 1e-5;
	float NoL = clamp(dot(n, l), 0.0, 1.0);
	float NoH = clamp(dot(n, h), 0.0, 1.0);
	float LoH = clamp(dot(l, h), 0.0, 1.0);

    // textures
    vec3 albedo = texture(diffTex, texCoords).rgb;
    float roughness = texture(roughTex, texCoords).r;
    float metallic = texture(metalTex, texCoords).r;
    float ao = texture(aoTex, texCoords).r;

    // specular BRDF
    vec3 f0 = vec3(SKIN_SPEC_REFELCTANCE); // base reflectivity

    float D = D_GGX(NoH, NoH * roughness);
    float V = V_SmithGGXCorrelated(NoV, NoL, roughness);
    vec3 F = F_Schlick(LoH, f0);

    vec3 specular = (D * V) * F;

    // diffuse BRDF
    vec3 kd = (1.0 - F) * (1.0 - metallic); // diffuse contribution
    vec3 diffuse = kd * albedo * Fd_Lambert();

    // lighting
    vec3 radiance = (lightColor/dot(l, l)) * NoL; // light source(attenuation) * cosθ
    vec3 color = (diffuse + specular) * radiance;

    // ambient occlusion
    color *= ao;

    out_Color = vec4(color, 1.0);
}
