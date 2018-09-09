//TextureMapping.fx

float4x4 matWorld;
float4x4 matView;
float4x4 matProjection;

float4 vMaterialDiffuseColor; //도형 diffuse 색상
float4 vMaterialSpecularColor;

float3 vLightDir;	//광원의 방향(정규화)
float3 vEyePos;		//눈의 위치
float  fPower;		//정반사 날카롭기

texture diffuseTex;
sampler2D diffuseSampler = sampler_state
{
	Texture = diffuseTex;
};


struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 position : POSITION; //무조건 필요 
	float2 texCoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 eyeDir : TEXCOORD2;
};

VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 worldPos = mul(float4(input.position, 1.0), matWorld);
	output.position = mul(worldPos, matView);
	output.position = mul(output.position, matProjection);

	float3 worldNormal = mul(input.normal, (float3x3)matWorld);

	output.normal = normalize(worldNormal);
	float3 eyeDir = vEyePos - worldPos.xyz;

	output.eyeDir = normalize(eyeDir);

	output.texCoord = input.texCoord;
	return output;
}

float4 ps_main(VS_OUTPUT input) : COLOR0
{
	//텍스쳐 색상값 가지고 호출?
	float4 texColor = tex2D(diffuseSampler, input.texCoord);


	//정확히 계산하기 위해서 다시 정규화
	float3 normal = normalize(input.normal);
	float3 eyeDir = normalize(input.eyeDir);

	//월드 광원의 방향 
	float3 toLight = -vLightDir;
	//Diffuse

	//half lamert = -1.0f ~ 1.0f -> 0.0 ~ 1.0f 
	float diffuse = dot(toLight, normal);
	diffuse = (diffuse + 1.0f) * 0.5f;

	//lightReflection 계산 
	//float3 reflection = reflect(vLightDir, normal);
	//reflection = normalize(reflection);
	//float spec = saturate(dot(reflection, eyeDir));

	//jim blinn 개발한 정반사 계산법 
	float3 halfVector = toLight + eyeDir;
	halfVector = normalize(halfVector);

	float spec = saturate(dot(halfVector, normal));

	spec = pow(spec, fPower);

	//최종 컬러 계산 (0 ~ 1)
	float4 color = texColor
		* (vMaterialDiffuseColor * diffuse)
		+(vMaterialSpecularColor * spec);

	return color;
}

technique BaseColor
{
	pass p0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}