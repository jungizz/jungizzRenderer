#version 410 core

uniform sampler2D colorTex;
uniform sampler2D depthTex;
uniform vec2 size;
uniform float fovy;
uniform float near;
uniform float far;
uniform float kernelParam;

out vec4 out_Color;

// Sum of Gaussian(SoG) fit for skin
const float variances[6] = float[6](0.0064, 0.0484, 0.187, 0.567, 1.99, 7.41); // mm단위
const vec3 colorWeights[6] = vec3[6]( vec3(0.233, 0.455, 0.649), // r, g, b
								    vec3(0.100, 0.336, 0.344), 
							  	    vec3(0.118, 0.198, 0.000),
					  	     		vec3(0.113, 0.007, 0.007), 
							 	 	vec3(0.358, 0.004, 0.000),
						  	  	    vec3(0.078, 0.000, 0.000) );


// convert non-linear depth to linear depth
float LinearizeDepth(float dPri)
{
	float d = dPri * 2 - 1; // [0,1] -> [-1, 1]
	return (2 * near * far) / (near + far + d * (near - far)); // z[n, f]
}

void main(void)
{
	float depth = texture(depthTex, gl_FragCoord.xy / size).r; // d'[0,1] (가까울수록 0에 가까움)
	float worldZ = LinearizeDepth(depth); // linear depth z[n, f] in camera coordinate

	vec2 texelSize = 1.0/size; // 한 픽셀의 텍스처 좌표 크기 [0, 1]

	vec3 wSum = vec3(0.0); // rgb채널 별 누적되는 가중치의 합
	vec3 resColor = vec3(0.0); // 최종 색상 누적 값 (가중치 곱한 샘플 합)
	float screenHeight = 2 * near * (tan(fovy / 2)); // screen height in world coord

	// SoG
	for(int i=0; i<colorWeights.length(); i++)
	{
		float sigma = sqrt(variances[i]) * .001; // kernel size in world coord (mm단위를 m단위로)
		float sigmaPri = abs((near * sigma) / worldZ); // kernel size at screen in world coord
		sigmaPri /= screenHeight; // normalize [0,1]
		sigmaPri *= size.y; // in pixel size
		float variance = sigmaPri * sigmaPri;

		float kernel = sigmaPri;
		kernel *= kernelParam;

		vec3 colorWeight = colorWeights[i];

		for(float dy=-kernel; dy<=kernel; dy+=kernel/10.)
		{
			float yy =  gl_FragCoord.y / size.y + (dy * texelSize.y); // 현재 픽셀의 texture coordinate
			float depth = texture(depthTex, vec2(gl_FragCoord.x / size.x, yy)).r; // d[0,1] (가까울수록 0)
			float sampleZ = LinearizeDepth(depth); // world coord linear depth z[n, f]

			vec3 w = colorWeight * exp(-(dy*dy)/(2.0 * variance))*exp(-pow((sampleZ-worldZ), 2)/(0.01*0.01));
			resColor += w * texture(colorTex, vec2(gl_FragCoord.x/size.x, yy)).rgb;

			wSum += w;
		}
	}
	resColor /= wSum;	 
	out_Color = vec4(resColor, 1.0);
}
