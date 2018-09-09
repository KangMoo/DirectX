
//전역변수

float4x4 matWorld;
float4x4 matView;
float4x4 matProjection;
float4 vColor; //출력될 컬러
float3 vLightDir;// 라이트 방향
float4x4 matInvWorld; //월드의 역행렬 

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
    output.position = mul(input.position, matWorld);
    output.position = mul(output.position, matView);
    output.position = mul(output.position, matProjection);

    //로컬 노말
    float3 localNormal = input.normal;

    //월드 광원의 방향을 로컬로 변경
    float3 localLightDir = mul(vLightDir, (float3x3) matInvWorld);

    localLightDir = normalize(localLightDir);

    localLightDir = -localLightDir;

    //램버트 
    float diffuse = dot(localLightDir, localNormal);

    //0 ~ 1 사이값 으로 만든다 .
    diffuse = saturate(diffuse);

    output.color = vColor * diffuse;

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
