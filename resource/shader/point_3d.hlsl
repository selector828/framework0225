cbuffer global
{
	matrix g_w;
	matrix g_v;
	matrix g_p;
};

struct PS_INPUT
{
	float4 pos_ : SV_POSITION;
};

PS_INPUT VS( float4 pos : POSITION ) 
{
    matrix wvp = transpose(mul(g_p, mul(g_v, g_w)));

    PS_INPUT output;
    output.pos_ = mul(pos, wvp);
    return output;
}

float4 PS( PS_INPUT input ) : SV_Target
{
	return float4(1,1,1,1);
}