//グローバル
cbuffer global
{
	matrix g_W; //ワールドから射影までの変換行列
	matrix g_V; //ワールドから射影までの変換行列
	matrix g_P; //ワールドから射影までの変換行列
};
//構造体
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
};

//
//
//バーテックスシェーダー
PS_INPUT VS( float4 Pos : POSITION ) 
{
    matrix WVP = transpose(mul(g_P, mul(g_V, g_W)));

    PS_INPUT Out;
    Out.Pos = mul(Pos, WVP);
    return Out;
}
//
//
//ピクセルシェーダー
float4 PS( PS_INPUT Input ) : SV_Target
{
	return float4(1,1,1,1);
}