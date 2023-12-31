#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.worldpos = mul(world, pos);
	output.svpos = pos;
	output.normal = normal;
	output.uv = uv;
	return output;

}