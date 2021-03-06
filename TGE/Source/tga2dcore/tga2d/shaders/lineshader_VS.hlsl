#include "../render/common.hlsli"

struct VertexInputType
{
    float4 position : POSITION;
	float4 color : TEXCOORD0;
};

struct PixelInputTypeLine
{
    float4 position : SV_POSITION;
	float4 color : TEXCOORD0;
};

PixelInputTypeLine main(VertexInputType input)
{
    PixelInputTypeLine output;
	
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
	
	output.position = input.position;
	output.color = input.color;
    
    return output;
}