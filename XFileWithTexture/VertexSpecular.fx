
//전역변수

float4x4 matWorld;
float4x4 matView;
float4x4 matProjection;

float4 vMaterialDiffuseColor; //출력될 컬러
float3 vLightDir; // 라이트 방향
float3 vEyePos; //눈의 위치 

struct VS_INPUT
{
    float4 position : POSITION;
    float3 normal : NORMAL;
};
//정점 셰이더에서 픽셀 세이더로 넘겨주는 정보 
struct VS_OUTPUT
{
    float4 position : POSITION;
    float4 color : COLOR0;
};
VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.position = 1.0f;

    float4 worldPos = mul(input.position, matWorld);
    output.position = mul(worldPos, matView);
    output.position = mul(output.position, matProjection);

    //로컬 노말
    float3 worldNormal = mul(input.normal, (float3x3)matWorld);
    
    worldNormal = normalize(worldNormal);


    //눈의 방향 얻기
    //worldPos.xyz 스와즐링 문법 
    float3 eyeDir = vEyePos - worldPos.xyz;

    eyeDir = normalize(eyeDir);

    //월드 광원의 방향을 로컬로 변경
    float3 toLightDir = normalize(-vLightDir);

    float3 inLightDir = -toLightDir;

    //빛의 반사 방향을 얻자 

    float3 lightReflect = inLightDir +
    2 * (dot(worldNormal, -inLightDir) * worldNormal);
    //램버트 

    lightReflect = normalize(lightReflect);

    //빛이 반사된 방향과 눈의 방향의 각도차가 적게 나올수록 정반사
    //광이 쌔진다.
    float spec = saturate(dot(eyeDir, lightReflect));

    spec = pow(spec, 30);

    float diffuse = dot(toLightDir, worldNormal);

    //0 ~ 1 사이값 으로 만든다 .
    diffuse = saturate(diffuse);

    output.color = vMaterialDiffuseColor * diffuse + float4(1, 1, 1, 1) * spec;

    return output;
}

float4 ps_main(VS_OUTPUT input) : COLOR0
{
    return input.color;
}

technique BaseColor
{
    pass p0
    {
        VertexShader = compile vs_3_0 vs_main();
        PixelShader = compile ps_3_0 ps_main();
    }
}
