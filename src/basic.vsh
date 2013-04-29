struct VertexIn
{
	float3 pos			: POSITION;
	float2 tex			: TEXCOORD;
};

struct VertexOut
{
	float4 pos			: SV_POSITION;
	float2 tex			: TEXCOORD2;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	vout.pos.xyz = vin.pos;
    vout.pos.w   = 1.f;
	vout.tex	 = vin.tex;
	return vout;
}