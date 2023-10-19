cbuffer cbuff0 : register(b0)
{
	matrix mat; //3D変換行列
    matrix viewproj;
    matrix world;
    float3 cameraPos;
    float shininess;//光沢度
    float alpha;
    float3 color;
};

cbuffer cbuff1 : register(b1) {
	float3 m_ambient : packoffset(c0); //アンビエント係数
	float3 m_diffuse : packoffset(c1); //ディフューズ係数
	float3 m_specular : packoffset(c2); //スペキュラー係数
	float m_alpha : packoffset(c2.w); //アルファ

}


cbuffer cbuff3 : register(b3) {
    float _Destruction : packoffset(c0);
    float _ScaleFactor : packoffset(c0.y);
    float _RotationFactor : packoffset(c0.z);
    float _PositionFactor : packoffset(c0.w);
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float3 normal:NORMAL;//法線ベクトル
    float4 worldpos:POSITION;
	float2 uv  :TEXCOORD; // uv値
};

struct GSOutput
{
    //システム用頂点座標
    float4 svpos : SV_POSITION;
    float3 normal:NORMAL;//法線ベクトル
    float4 worldpos:POSITION;
    //uv値
    float2 uv  :TEXCOORD;
};