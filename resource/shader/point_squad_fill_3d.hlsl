cbuffer global
{
    matrix g_w : packoffset(c0);
    matrix g_v : packoffset(c4);
    matrix g_p : packoffset(c8);

    float2 g_size : packoffset(c12);
};

struct GS_INPUT
{
    float4 pos_ : SV_POSITION;
};

struct PS_INPUT
{
    float4 pos_ : SV_POSITION;
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

    PS_INPUT p = (PS_INPUT) 0;
    p.pos_ = input[0].pos_ + float4(-g_size.x, -g_size.y, 0, 0) * .01f;
    p.pos_ = mul(p.pos_, wvp);
    w = p.pos_.w;
    tri_stream.Append(p);

    p.pos_ = input[0].pos_ + float4(+g_size.x, -g_size.y, 0, 0) * .01f;
    p.pos_ = mul(p.pos_, wvp);
    p.pos_.w = w;
    tri_stream.Append(p);

    p.pos_ = input[0].pos_ + float4(-g_size.x, +g_size.y, 0, 0) * .01f;
    p.pos_ = mul(p.pos_, wvp);
    p.pos_.w = w;
    tri_stream.Append(p);

    p.pos_ = input[0].pos_ + float4(+g_size.x, +g_size.y, 0, 0) * .01f;
    p.pos_ = mul(p.pos_, wvp);
    p.pos_.w = w;
    tri_stream.Append(p);

    tri_stream.RestartStrip();
}

float4 PS(PS_INPUT input) : SV_Target
{
    return float4(1, 1, 1, 1);
}