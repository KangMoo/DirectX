//shdaer -> 

//색상 레스터 라이즈 알아서 색상 값을 사용
//HLSL -> 

//Shader-> 랜더링 파이프 라인 (고정파이프 라인) -> 

//전역 
float4x4 matWorld;
float4x4 matView;
float4x4 matProjection;

float4 vColor;

struct VS_OUTPUT
{
    float4 position : POSITION;
    float4 color : COLOR0;
};

VS_OUTPUT vs_main(float3 position : POSITION)
{

    VS_OUTPUT output;
    output.position.w = 1.0f;
    output.position = mul(float4(position, 1.0f), matWorld);
    output.position = mul(output.position, matView );
    output.position = mul(output.position, matProjection);
    output.color = vColor;

    return output;
}

struct PS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR0;
};

float4 ps_main(PS_INPUT input) : COLOR0
{
    return input.color;
}
//dx9 -> 쉐이더를 사용 고정 파이프 라인 삭제 
//dx11 -> 고정 파이프 라인x 
technique BaseColor
{
    pass p0
    {
        VertexShader = compile vs_3_0 vs_main();
        PixelShader = compile ps_3_0 ps_main();
    }
}
