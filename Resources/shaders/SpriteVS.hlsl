struct VertexShaderOutput
{
    float4 position : SV_Position;
};
    
struct VertexShaderInput
{
    float position : POSITION0;
};
    
VertexShaderOutput main(VertexShaderInput input)
    {
        VertexShaderOutput output;
        output.position = input.position;
        return output;
    }