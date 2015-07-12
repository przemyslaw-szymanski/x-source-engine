
struct VS_IN {
	float3 f3Position : POSITION;
	float3 f3Normal : NORMAL;
	float2 f2TexCoord : TEXCOORD0;
	//float4 f4Color : COLOR;
	//float3 f3Tangent : TANGENT;
};


struct VS_OUT {
	float4 f4VertexPos : SV_POSITION;
	float3 f3Pos : TEXCOORD1;
	float3 f3Normal : NORMAL;
	float2 f2TexCoord : TEXCOORD0;
	float4 f4Color : COLOR;
};

VS_OUT vs(VS_IN IN)
{
	VS_OUT OUT = (VS_OUT)0;
	OUT.f4VertexPos = mul( float4(IN.f3Position, 1), mtxObjWVP );
	OUT.f3Normal =  IN.f3Normal; //normalize( mul( IN.f3Normal, mtxObjWorld ) );
	OUT.f3Pos = mul( IN.f3Position.xyz, mtxObjWorld );
	OUT.f2TexCoord = IN.f2TexCoord;
	//OUT.f4Color = IN.f4Color;
	return OUT;
}


sampler DiffuseSampler : register(s0);
Texture2D DiffuseTex;

float4 ps(VS_OUT IN) : SV_TARGET
{
	//const float3 f3LightPos = float3( 0, 100, 0 );
	const float3 f3VertexPos = IN.f3Pos;
	float3 f3N = normalize( IN.f3Normal );
	float4 tc = DiffuseTex.Sample( DiffuseSampler, IN.f2TexCoord * 100 );
	//f3N = normalize( tc.rgb );
	float3 f3L = normalize( f3LightPos - f3VertexPos );
	float fDL = max( dot( f3N, f3L ), 0.0 );
	float4 c = tc;
	c.rgba = fDL * c;
	return c;
}