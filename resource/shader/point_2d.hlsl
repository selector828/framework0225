cbuffer global
{
    matrix g_w : packoffset(c0);
    float2 g_viewport : packoffset(c4);
};

struct PS_INPUT
{
    float4 pos_ : SV_POSITION;
};

PS_INPUT VS(float4 pos : POSITION)
{
    PS_INPUT output;
   
    output.pos_ = mul(pos, transpose(g_w));

    output.pos_.x = (output.pos_.x / g_viewport.x) * 2 - 1;
    output.pos_.y = 1 - (output.pos_.y / g_viewport.y) * 2;

    return output;
}

float4 PS(PS_INPUT Input) : SV_Target
{
    return float4(1, 1, 1, 1);
}