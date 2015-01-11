
struct VS_IN
{
	float4 f4Position : POSITION;
	float3 f3Normal : NORMAL;
};

struct VS_OUT
{
	float4 f4VertexPos : SV_POSITION;
	float3 f3Normal : NORMAL;
};

VS_OUT vs(VS_IN IN)
{
	VS_OUT OUT;
	OUT.f4VertexPos = mul( IN.f4Position, mtxObjWVP );
	OUT.f3Normal = mul( IN.f3Normal, (float3x3)mtxObjWorld );
	return OUT;
}