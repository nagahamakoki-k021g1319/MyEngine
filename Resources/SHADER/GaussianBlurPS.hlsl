#include "GaussianBlurHeader.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

// �K�E�X�֐�
float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

// �K�E�V�A���u���[
float4 GaussianBlur(Texture2D<float4> tex, SamplerState smp, float2 uv, float sigma, float loopNum)
{
    float4 result = float4(0, 0, 0, 0);
    float totalWeight = 0;
    float stepWidth = sigma / loopNum;

    for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
    {
        for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
        {
		    // �F�擾����UV���W
            float2 pickUV = uv + float2(px, py);
            
            // ��ʊO�̐F���擾���Ȃ��悤��
            pickUV = clamp(pickUV, 0.001, 0.999);

			// �E�F�C�g
            float weight = Gaussian(uv, pickUV, sigma);
            totalWeight += weight;

            result += tex.Sample(smp, pickUV) * weight;
        }
    }
    
    result.rgb = result.rgb / totalWeight;
    result.a = 1;
    return result;
}

PSOutput main(VSOutput input)// : SV_TARGET
{
    PSOutput output;
    float4 texcolor = GaussianBlur(tex, smp, input.uv, 0.0025f, 5);
    float4 result = float4(texcolor.rgb, 1);
    output.target0 = result;
    
    return output;
}