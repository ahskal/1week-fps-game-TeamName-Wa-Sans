
cbuffer VS_W : register(b0)
{
    matrix World;
}

cbuffer VS_VP : register(b1)
{
    matrix ViewProj;
}

cbuffer VS_COLOR : register(b2)
{
    //4 개의 4바이트
    float4 color;
}

Texture2D TextureD : register(t1);
SamplerState SamplerD : register(s1);