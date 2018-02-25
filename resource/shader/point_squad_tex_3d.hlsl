Texture2D g_tex : register(t0);

SamplerState g_sampler : register(s0);

cbuffer global
{
    matrix g_w : packoffset(c0);
    matrix g_v : packoffset(c4);
    matrix g_p : packoffset(c8);

    float2 g_size : packoffset(c12);
    float2 g_split : packoffset(c13);
    float g_pattern : packoffset(c14);
};

struct GS_INPUT
{
    float4 pos_ : SV_POSITION;
};

struct PS_INPUT
{
    float4 pos_ : SV_POSITION;
    float2 uv_ : TEXCOORD;
};

GS_INPUT VS(float4 pos : POSITION)
{
    GS_INPUT output = (GS_INPUT) 0;
    output.pos_ = pos;
    return output;
}

[maxvertexcount(4)]
void GS(point GS_INPUT input[1], inout TriangleStream<PS_INPUT> tri_stream)
{
    float w = 0; //ÉpÅ[ÉXñhé~
    matrix wvp = transpose(mul(g_p, mul(g_v, g_w)));

    int2 start = int2((int) g_pattern % (int) g_split.x, (int) g_pattern / (int) g_split.x);
    float2 size = float2(1.f / g_split.x, 1.f / g_split.y);

    PS_INPUT p = (PS_INPUT) 0;
    p.pos_ = input[0].pos_ + float4(-g_size.x, +g_size.y, 0, 0) * .001f;
    p.pos_ = mul(p.pos_, wvp);
    w = p.pos_.w;
    p.uv_ = (start + int2(0, 0)) * size;
    tri_stream.Append(p);

    p.pos_ = input[0].pos_ + float4(+g_size.x, +g_size.y, 0, 0) * .001f;
    p.pos_ = mul(p.pos_, wvp);
    p.pos_.w = w;
    p.uv_ = (start + int2(1, 0)) * size;
    tri_stream.Append(p);

    p.pos_ = input[0].pos_ + float4(-g_size.x, -g_size.y, 0, 0) * .001f;
    p.pos_ = mul(p.pos_, wvp);
    p.pos_.w = w;
    p.uv_ = (start + int2(0, 1)) * size;
    tri_stream.Append(p);

    p.pos_ = input[0].pos_ + float4(+g_size.x, -g_size.y, 0, 0) * .001f;
    p.pos_ = mul(p.pos_, wvp);
    p.pos_.w = w;
    p.uv_ = (start + int2(1, 1)) * size;
    tri_stream.Append(p);

    tri_stream.RestartStrip();
}

float4 PS(PS_INPUT input) : SV_Target
{
    return g_tex.Sample(g_sampler, input.uv_);
}