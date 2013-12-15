
namespace XSE
{
	namespace D3D11
	{
		#define XSE_DS_DEFAULT_COLOR	"1.0f, 0.1f, 0.6f, 1.0f"
		#define XSE_DS_POSITION			"float4 Pos			: POSITION;"
		#define XSE_DS_NORMAL			"float3 Normal		: NORMAL;"
		#define XSE_DS_BINORMAL			"float3 Binormal	: BINORMAL;"
		#define XSE_DS_TANGENT			"float3 Tangnet		: TANGENT;"
		#define XSE_DS_TEXCOORD0		"float2 TexCoord	: TEXCOORD0;"
		#define XSE_DS_COLOR			"float4 Color		: COLOR;"
		#define XSE_DS_CONSTANT_BUFFER	\
			"cbuffer ConstantBuffer : register( b0 )" \
			"{" \
				"matrix mtxWorld;" \
				"matrix mtxView;" \
				"matrix mtxProjection;" \
			"};" 

		#define XSE_DS_VS_OUTPUT \
			"struct VS_OUTPUT" \
			"{" \
				"float4 Pos : SV_POSITION;" \
			"};"

		#define XSE_DS_VS_INPUT( _params ) \
			"struct VS_INPUT" \
			"{" \
				_params \
			"};"

		#define XSE_DS_VS \
			"VS_OUTPUT VS( VS_INPUT In )" \
			"{" \
				"VS_OUTPUT output = (VS_OUTPUT)0;" \
				"output.Pos = mul( In.Pos, mtxWorld );" \
				"output.Pos = mul( output.Pos, mtxView );" \
				"output.Pos = mul( output.Pos, mtxProjection );" \
				"return output;" \
			"}"

		#define XSE_DS_PS \
			"float4 PS( VS_OUTPUT input ) : SV_Target" \
			"{" \
				"return float4( " XSE_DS_DEFAULT_COLOR " );" \
			"}"

		const char* g_lpszDefault_MVP =	
			XSE_DS_CONSTANT_BUFFER
			XSE_DS_VS_OUTPUT
			XSE_DS_VS_INPUT( XSE_DS_POSITION )
			XSE_DS_VS
			XSE_DS_PS;

		const char* g_lpszDefault_MVP_COLOR =	
			XSE_DS_CONSTANT_BUFFER
			XSE_DS_VS_OUTPUT
			XSE_DS_VS_INPUT( XSE_DS_POSITION XSE_DS_COLOR )
			XSE_DS_VS
			XSE_DS_PS;

		struct DefaultShaders
		{
			enum
			{
				MVP,
				MVP_COLOR,
				MVP_NORMAL,
				MVP_TEXTURE,
				MVP_BINORMAL,
				MVP_TANGENT,
				MVP_NORMAL_COLOR,
				MVP_TEXTURE_NORMAL,
				MVP_TEXTURE_COLOR,
				MVP_TEXTURE_NORMAL_COLOR,
				MVP_BINORMAL_NORMAL,
				MVP_BINORMAL_NORMAL_COLOR,
				MVP_BINORMAL_TEXTURE_NORMAL,
				MVP_BINORMAL_TEXTURE_NORMAL_COLOR,
				
			};
		};

		const char* g_alpszDefaultShaders[] = 
		{
			g_lpszDefault_MVP,
			g_lpszDefault_MVP_COLOR
		};

	}//d3d11
}//xse