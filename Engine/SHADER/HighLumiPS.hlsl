#include "HighLumiHeader.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

PSOutput main(VSOutput input)// : SV_TARGET
{
    PSOutput output;
    float4 texColor = tex.Sample(smp, input.uv);
    
    float grayScale = texColor.r * 0.299f + texColor.g * 0.587f + texColor.b * 0.114f;
    float extract = smoothstep(0.1f, 0.6f, grayScale);
    float power = 10.0f;
    
    float4 result = float4(texColor.rgb * extract * power, 1);
    
    output.target0 = result;
    
    return output;
}