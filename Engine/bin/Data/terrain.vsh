
struct VS_IN
{
	float4 f4Position : POSITION;
	float3 f3Normal : NORMAL;
};

struct VS_OUT
{
	float4 f4VertexPos : SV_POSITION;
	float3 f3Normal : NORMAL;
	float3 f3Pos : TEXCOORD0;
};

VS_OUT vs(VS_IN IN)
{
	VS_OUT OUT = (VS_OUT)0;
	OUT.f4VertexPos = mul( IN.f4Position, mtxObjWVP );
	OUT.f3Normal =  normalize( mul( IN.f3Normal, mtxObjWorld ) );
	OUT.f3Pos = mul( IN.f4Position.xyz, mtxObjWorld );
	return OUT;
}