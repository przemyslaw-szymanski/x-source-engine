/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for license. */

//-----------------------------------NOTICE----------------------------------//
// Some of this file is automatically filled in by a Python script.  Only    //
// add custom code in the designated areas or it will be overwritten during  //
// the next update.                                                          //
//-----------------------------------NOTICE----------------------------------//

//--BEGIN FILE HEAD CUSTOM CODE--//
//--END CUSTOM CODE--//

#include "../../include/FixLink.h"
#include "../../include/ObjectRegistry.h"
#include "../../include/NIF_IO.h"
#include "../../include/obj/NiTexturingProperty.h"
#include "../../include/gen/TexDesc.h"
#include "../../include/obj/NiSourceTexture.h"
#include "../../include/gen/TexDesc.h"
#include "../../include/obj/NiSourceTexture.h"
#include "../../include/gen/TexDesc.h"
#include "../../include/obj/NiSourceTexture.h"
#include "../../include/gen/TexDesc.h"
#include "../../include/obj/NiSourceTexture.h"
#include "../../include/gen/TexDesc.h"
#include "../../include/obj/NiSourceTexture.h"
#include "../../include/gen/TexDesc.h"
#include "../../include/obj/NiSourceTexture.h"
#include "../../include/gen/TexDesc.h"
#include "../../include/obj/NiSourceTexture.h"
#include "../../include/gen/TexDesc.h"
#include "../../include/obj/NiSourceTexture.h"
#include "../../include/gen/TexDesc.h"
#include "../../include/obj/NiSourceTexture.h"
#include "../../include/gen/TexDesc.h"
#include "../../include/obj/NiSourceTexture.h"
#include "../../include/gen/ShaderTexDesc.h"
#include "../../include/gen/TexDesc.h"
#include "../../include/obj/NiSourceTexture.h"
using namespace Niflib;

//Definition of TYPE constant
const Type NiTexturingProperty::TYPE("NiTexturingProperty", &NiProperty::TYPE );

NiTexturingProperty::NiTexturingProperty() : flags((unsigned short)0), applyMode((ApplyMode)2), textureCount((unsigned int)7), hasBaseTexture(false), hasDarkTexture(false), hasDetailTexture(false), hasGlossTexture(false), hasGlowTexture(false), hasBumpMapTexture(false), bumpMapLumaScale(0.0f), bumpMapLumaOffset(0.0f), hasDecal0Texture(false), hasDecal1Texture(false), hasDecal2Texture(false), hasDecal3Texture(false), numShaderTextures((unsigned int)0) {
	//--BEGIN CONSTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

NiTexturingProperty::~NiTexturingProperty() {
	//--BEGIN DESTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

const Type & NiTexturingProperty::GetType() const {
	return TYPE;
}

NiObject * NiTexturingProperty::Create() {
	return new NiTexturingProperty;
}

void NiTexturingProperty::Read( istream& in, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//

	unsigned int block_num;
	NiProperty::Read( in, link_stack, info );
	if ( info.version <= 0x0A000102 ) {
		NifStream( flags, in, info );
	};
	NifStream( applyMode, in, info );
	NifStream( textureCount, in, info );
	NifStream( hasBaseTexture, in, info );
	if ( (hasBaseTexture != 0) ) {
		NifStream( block_num, in, info );
		link_stack.push_back( block_num );
		NifStream( baseTexture.clampMode, in, info );
		NifStream( baseTexture.filterMode, in, info );
		NifStream( baseTexture.uvSet, in, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( baseTexture.ps2L, in, info );
			NifStream( baseTexture.ps2K, in, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( baseTexture.unknown1, in, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( baseTexture.hasTextureTransform, in, info );
			if ( (baseTexture.hasTextureTransform != 0) ) {
				NifStream( baseTexture.translation, in, info );
				NifStream( baseTexture.tiling, in, info );
				NifStream( baseTexture.wRotation, in, info );
				NifStream( baseTexture.transformType_, in, info );
				NifStream( baseTexture.centerOffset, in, info );
			};
		};
	};
	NifStream( hasDarkTexture, in, info );
	if ( (hasDarkTexture != 0) ) {
		NifStream( block_num, in, info );
		link_stack.push_back( block_num );
		NifStream( darkTexture.clampMode, in, info );
		NifStream( darkTexture.filterMode, in, info );
		NifStream( darkTexture.uvSet, in, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( darkTexture.ps2L, in, info );
			NifStream( darkTexture.ps2K, in, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( darkTexture.unknown1, in, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( darkTexture.hasTextureTransform, in, info );
			if ( (darkTexture.hasTextureTransform != 0) ) {
				NifStream( darkTexture.translation, in, info );
				NifStream( darkTexture.tiling, in, info );
				NifStream( darkTexture.wRotation, in, info );
				NifStream( darkTexture.transformType_, in, info );
				NifStream( darkTexture.centerOffset, in, info );
			};
		};
	};
	NifStream( hasDetailTexture, in, info );
	if ( (hasDetailTexture != 0) ) {
		NifStream( block_num, in, info );
		link_stack.push_back( block_num );
		NifStream( detailTexture.clampMode, in, info );
		NifStream( detailTexture.filterMode, in, info );
		NifStream( detailTexture.uvSet, in, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( detailTexture.ps2L, in, info );
			NifStream( detailTexture.ps2K, in, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( detailTexture.unknown1, in, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( detailTexture.hasTextureTransform, in, info );
			if ( (detailTexture.hasTextureTransform != 0) ) {
				NifStream( detailTexture.translation, in, info );
				NifStream( detailTexture.tiling, in, info );
				NifStream( detailTexture.wRotation, in, info );
				NifStream( detailTexture.transformType_, in, info );
				NifStream( detailTexture.centerOffset, in, info );
			};
		};
	};
	NifStream( hasGlossTexture, in, info );
	if ( (hasGlossTexture != 0) ) {
		NifStream( block_num, in, info );
		link_stack.push_back( block_num );
		NifStream( glossTexture.clampMode, in, info );
		NifStream( glossTexture.filterMode, in, info );
		NifStream( glossTexture.uvSet, in, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( glossTexture.ps2L, in, info );
			NifStream( glossTexture.ps2K, in, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( glossTexture.unknown1, in, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( glossTexture.hasTextureTransform, in, info );
			if ( (glossTexture.hasTextureTransform != 0) ) {
				NifStream( glossTexture.translation, in, info );
				NifStream( glossTexture.tiling, in, info );
				NifStream( glossTexture.wRotation, in, info );
				NifStream( glossTexture.transformType_, in, info );
				NifStream( glossTexture.centerOffset, in, info );
			};
		};
	};
	NifStream( hasGlowTexture, in, info );
	if ( (hasGlowTexture != 0) ) {
		NifStream( block_num, in, info );
		link_stack.push_back( block_num );
		NifStream( glowTexture.clampMode, in, info );
		NifStream( glowTexture.filterMode, in, info );
		NifStream( glowTexture.uvSet, in, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( glowTexture.ps2L, in, info );
			NifStream( glowTexture.ps2K, in, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( glowTexture.unknown1, in, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( glowTexture.hasTextureTransform, in, info );
			if ( (glowTexture.hasTextureTransform != 0) ) {
				NifStream( glowTexture.translation, in, info );
				NifStream( glowTexture.tiling, in, info );
				NifStream( glowTexture.wRotation, in, info );
				NifStream( glowTexture.transformType_, in, info );
				NifStream( glowTexture.centerOffset, in, info );
			};
		};
	};
	NifStream( hasBumpMapTexture, in, info );
	if ( (hasBumpMapTexture != 0) ) {
		NifStream( block_num, in, info );
		link_stack.push_back( block_num );
		NifStream( bumpMapTexture.clampMode, in, info );
		NifStream( bumpMapTexture.filterMode, in, info );
		NifStream( bumpMapTexture.uvSet, in, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( bumpMapTexture.ps2L, in, info );
			NifStream( bumpMapTexture.ps2K, in, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( bumpMapTexture.unknown1, in, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( bumpMapTexture.hasTextureTransform, in, info );
			if ( (bumpMapTexture.hasTextureTransform != 0) ) {
				NifStream( bumpMapTexture.translation, in, info );
				NifStream( bumpMapTexture.tiling, in, info );
				NifStream( bumpMapTexture.wRotation, in, info );
				NifStream( bumpMapTexture.transformType_, in, info );
				NifStream( bumpMapTexture.centerOffset, in, info );
			};
		};
		NifStream( bumpMapLumaScale, in, info );
		NifStream( bumpMapLumaOffset, in, info );
		NifStream( bumpMapMatrix, in, info );
	};
	NifStream( hasDecal0Texture, in, info );
	if ( (hasDecal0Texture != 0) ) {
		NifStream( block_num, in, info );
		link_stack.push_back( block_num );
		NifStream( decal0Texture.clampMode, in, info );
		NifStream( decal0Texture.filterMode, in, info );
		NifStream( decal0Texture.uvSet, in, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( decal0Texture.ps2L, in, info );
			NifStream( decal0Texture.ps2K, in, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( decal0Texture.unknown1, in, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( decal0Texture.hasTextureTransform, in, info );
			if ( (decal0Texture.hasTextureTransform != 0) ) {
				NifStream( decal0Texture.translation, in, info );
				NifStream( decal0Texture.tiling, in, info );
				NifStream( decal0Texture.wRotation, in, info );
				NifStream( decal0Texture.transformType_, in, info );
				NifStream( decal0Texture.centerOffset, in, info );
			};
		};
	};
	if ( (textureCount >= 8) ) {
		NifStream( hasDecal1Texture, in, info );
	};
	if ( (((textureCount >= 8)) && ((hasDecal1Texture != 0))) ) {
		NifStream( block_num, in, info );
		link_stack.push_back( block_num );
		NifStream( decal1Texture.clampMode, in, info );
		NifStream( decal1Texture.filterMode, in, info );
		NifStream( decal1Texture.uvSet, in, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( decal1Texture.ps2L, in, info );
			NifStream( decal1Texture.ps2K, in, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( decal1Texture.unknown1, in, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( decal1Texture.hasTextureTransform, in, info );
			if ( (decal1Texture.hasTextureTransform != 0) ) {
				NifStream( decal1Texture.translation, in, info );
				NifStream( decal1Texture.tiling, in, info );
				NifStream( decal1Texture.wRotation, in, info );
				NifStream( decal1Texture.transformType_, in, info );
				NifStream( decal1Texture.centerOffset, in, info );
			};
		};
	};
	if ( (textureCount >= 9) ) {
		NifStream( hasDecal2Texture, in, info );
	};
	if ( (((textureCount >= 9)) && ((hasDecal2Texture != 0))) ) {
		NifStream( block_num, in, info );
		link_stack.push_back( block_num );
		NifStream( decal2Texture.clampMode, in, info );
		NifStream( decal2Texture.filterMode, in, info );
		NifStream( decal2Texture.uvSet, in, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( decal2Texture.ps2L, in, info );
			NifStream( decal2Texture.ps2K, in, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( decal2Texture.unknown1, in, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( decal2Texture.hasTextureTransform, in, info );
			if ( (decal2Texture.hasTextureTransform != 0) ) {
				NifStream( decal2Texture.translation, in, info );
				NifStream( decal2Texture.tiling, in, info );
				NifStream( decal2Texture.wRotation, in, info );
				NifStream( decal2Texture.transformType_, in, info );
				NifStream( decal2Texture.centerOffset, in, info );
			};
		};
	};
	if ( (textureCount >= 10) ) {
		NifStream( hasDecal3Texture, in, info );
	};
	if ( (((textureCount >= 10)) && ((hasDecal3Texture != 0))) ) {
		NifStream( block_num, in, info );
		link_stack.push_back( block_num );
		NifStream( decal3Texture.clampMode, in, info );
		NifStream( decal3Texture.filterMode, in, info );
		NifStream( decal3Texture.uvSet, in, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( decal3Texture.ps2L, in, info );
			NifStream( decal3Texture.ps2K, in, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( decal3Texture.unknown1, in, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( decal3Texture.hasTextureTransform, in, info );
			if ( (decal3Texture.hasTextureTransform != 0) ) {
				NifStream( decal3Texture.translation, in, info );
				NifStream( decal3Texture.tiling, in, info );
				NifStream( decal3Texture.wRotation, in, info );
				NifStream( decal3Texture.transformType_, in, info );
				NifStream( decal3Texture.centerOffset, in, info );
			};
		};
	};
	if ( info.version >= 0x0A000100 ) {
		NifStream( numShaderTextures, in, info );
		shaderTextures.resize(numShaderTextures);
		for (unsigned int i2 = 0; i2 < shaderTextures.size(); i2++) {
			NifStream( shaderTextures[i2].isUsed, in, info );
			if ( (shaderTextures[i2].isUsed != 0) ) {
				NifStream( block_num, in, info );
				link_stack.push_back( block_num );
				NifStream( shaderTextures[i2].textureData.clampMode, in, info );
				NifStream( shaderTextures[i2].textureData.filterMode, in, info );
				NifStream( shaderTextures[i2].textureData.uvSet, in, info );
				if ( info.version <= 0x0A020000 ) {
					NifStream( shaderTextures[i2].textureData.ps2L, in, info );
					NifStream( shaderTextures[i2].textureData.ps2K, in, info );
				};
				if ( info.version <= 0x0401000C ) {
					NifStream( shaderTextures[i2].textureData.unknown1, in, info );
				};
				if ( info.version >= 0x0A010000 ) {
					NifStream( shaderTextures[i2].textureData.hasTextureTransform, in, info );
					if ( (shaderTextures[i2].textureData.hasTextureTransform != 0) ) {
						NifStream( shaderTextures[i2].textureData.translation, in, info );
						NifStream( shaderTextures[i2].textureData.tiling, in, info );
						NifStream( shaderTextures[i2].textureData.wRotation, in, info );
						NifStream( shaderTextures[i2].textureData.transformType_, in, info );
						NifStream( shaderTextures[i2].textureData.centerOffset, in, info );
					};
				};
				NifStream( shaderTextures[i2].unknownInt, in, info );
			};
		};
	};

	//--BEGIN POST-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiTexturingProperty::Write( ostream& out, const map<NiObjectRef,unsigned int> & link_map, const NifInfo & info ) const {
	//--BEGIN PRE-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiProperty::Write( out, link_map, info );
	numShaderTextures = (unsigned int)(shaderTextures.size());
	if ( info.version <= 0x0A000102 ) {
		NifStream( flags, out, info );
	};
	NifStream( applyMode, out, info );
	NifStream( textureCount, out, info );
	NifStream( hasBaseTexture, out, info );
	if ( (hasBaseTexture != 0) ) {
		if ( info.version < VER_3_3_0_13 ) {
			NifStream( (unsigned int)&(*baseTexture.source), out, info );
		} else {
			if ( baseTexture.source != NULL ) {
				NifStream( link_map.find( StaticCast<NiObject>(baseTexture.source) )->second, out, info );
			} else {
				NifStream( 0xFFFFFFFF, out, info );
			}
		}
		NifStream( baseTexture.clampMode, out, info );
		NifStream( baseTexture.filterMode, out, info );
		NifStream( baseTexture.uvSet, out, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( baseTexture.ps2L, out, info );
			NifStream( baseTexture.ps2K, out, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( baseTexture.unknown1, out, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( baseTexture.hasTextureTransform, out, info );
			if ( (baseTexture.hasTextureTransform != 0) ) {
				NifStream( baseTexture.translation, out, info );
				NifStream( baseTexture.tiling, out, info );
				NifStream( baseTexture.wRotation, out, info );
				NifStream( baseTexture.transformType_, out, info );
				NifStream( baseTexture.centerOffset, out, info );
			};
		};
	};
	NifStream( hasDarkTexture, out, info );
	if ( (hasDarkTexture != 0) ) {
		if ( info.version < VER_3_3_0_13 ) {
			NifStream( (unsigned int)&(*darkTexture.source), out, info );
		} else {
			if ( darkTexture.source != NULL ) {
				NifStream( link_map.find( StaticCast<NiObject>(darkTexture.source) )->second, out, info );
			} else {
				NifStream( 0xFFFFFFFF, out, info );
			}
		}
		NifStream( darkTexture.clampMode, out, info );
		NifStream( darkTexture.filterMode, out, info );
		NifStream( darkTexture.uvSet, out, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( darkTexture.ps2L, out, info );
			NifStream( darkTexture.ps2K, out, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( darkTexture.unknown1, out, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( darkTexture.hasTextureTransform, out, info );
			if ( (darkTexture.hasTextureTransform != 0) ) {
				NifStream( darkTexture.translation, out, info );
				NifStream( darkTexture.tiling, out, info );
				NifStream( darkTexture.wRotation, out, info );
				NifStream( darkTexture.transformType_, out, info );
				NifStream( darkTexture.centerOffset, out, info );
			};
		};
	};
	NifStream( hasDetailTexture, out, info );
	if ( (hasDetailTexture != 0) ) {
		if ( info.version < VER_3_3_0_13 ) {
			NifStream( (unsigned int)&(*detailTexture.source), out, info );
		} else {
			if ( detailTexture.source != NULL ) {
				NifStream( link_map.find( StaticCast<NiObject>(detailTexture.source) )->second, out, info );
			} else {
				NifStream( 0xFFFFFFFF, out, info );
			}
		}
		NifStream( detailTexture.clampMode, out, info );
		NifStream( detailTexture.filterMode, out, info );
		NifStream( detailTexture.uvSet, out, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( detailTexture.ps2L, out, info );
			NifStream( detailTexture.ps2K, out, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( detailTexture.unknown1, out, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( detailTexture.hasTextureTransform, out, info );
			if ( (detailTexture.hasTextureTransform != 0) ) {
				NifStream( detailTexture.translation, out, info );
				NifStream( detailTexture.tiling, out, info );
				NifStream( detailTexture.wRotation, out, info );
				NifStream( detailTexture.transformType_, out, info );
				NifStream( detailTexture.centerOffset, out, info );
			};
		};
	};
	NifStream( hasGlossTexture, out, info );
	if ( (hasGlossTexture != 0) ) {
		if ( info.version < VER_3_3_0_13 ) {
			NifStream( (unsigned int)&(*glossTexture.source), out, info );
		} else {
			if ( glossTexture.source != NULL ) {
				NifStream( link_map.find( StaticCast<NiObject>(glossTexture.source) )->second, out, info );
			} else {
				NifStream( 0xFFFFFFFF, out, info );
			}
		}
		NifStream( glossTexture.clampMode, out, info );
		NifStream( glossTexture.filterMode, out, info );
		NifStream( glossTexture.uvSet, out, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( glossTexture.ps2L, out, info );
			NifStream( glossTexture.ps2K, out, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( glossTexture.unknown1, out, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( glossTexture.hasTextureTransform, out, info );
			if ( (glossTexture.hasTextureTransform != 0) ) {
				NifStream( glossTexture.translation, out, info );
				NifStream( glossTexture.tiling, out, info );
				NifStream( glossTexture.wRotation, out, info );
				NifStream( glossTexture.transformType_, out, info );
				NifStream( glossTexture.centerOffset, out, info );
			};
		};
	};
	NifStream( hasGlowTexture, out, info );
	if ( (hasGlowTexture != 0) ) {
		if ( info.version < VER_3_3_0_13 ) {
			NifStream( (unsigned int)&(*glowTexture.source), out, info );
		} else {
			if ( glowTexture.source != NULL ) {
				NifStream( link_map.find( StaticCast<NiObject>(glowTexture.source) )->second, out, info );
			} else {
				NifStream( 0xFFFFFFFF, out, info );
			}
		}
		NifStream( glowTexture.clampMode, out, info );
		NifStream( glowTexture.filterMode, out, info );
		NifStream( glowTexture.uvSet, out, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( glowTexture.ps2L, out, info );
			NifStream( glowTexture.ps2K, out, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( glowTexture.unknown1, out, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( glowTexture.hasTextureTransform, out, info );
			if ( (glowTexture.hasTextureTransform != 0) ) {
				NifStream( glowTexture.translation, out, info );
				NifStream( glowTexture.tiling, out, info );
				NifStream( glowTexture.wRotation, out, info );
				NifStream( glowTexture.transformType_, out, info );
				NifStream( glowTexture.centerOffset, out, info );
			};
		};
	};
	NifStream( hasBumpMapTexture, out, info );
	if ( (hasBumpMapTexture != 0) ) {
		if ( info.version < VER_3_3_0_13 ) {
			NifStream( (unsigned int)&(*bumpMapTexture.source), out, info );
		} else {
			if ( bumpMapTexture.source != NULL ) {
				NifStream( link_map.find( StaticCast<NiObject>(bumpMapTexture.source) )->second, out, info );
			} else {
				NifStream( 0xFFFFFFFF, out, info );
			}
		}
		NifStream( bumpMapTexture.clampMode, out, info );
		NifStream( bumpMapTexture.filterMode, out, info );
		NifStream( bumpMapTexture.uvSet, out, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( bumpMapTexture.ps2L, out, info );
			NifStream( bumpMapTexture.ps2K, out, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( bumpMapTexture.unknown1, out, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( bumpMapTexture.hasTextureTransform, out, info );
			if ( (bumpMapTexture.hasTextureTransform != 0) ) {
				NifStream( bumpMapTexture.translation, out, info );
				NifStream( bumpMapTexture.tiling, out, info );
				NifStream( bumpMapTexture.wRotation, out, info );
				NifStream( bumpMapTexture.transformType_, out, info );
				NifStream( bumpMapTexture.centerOffset, out, info );
			};
		};
		NifStream( bumpMapLumaScale, out, info );
		NifStream( bumpMapLumaOffset, out, info );
		NifStream( bumpMapMatrix, out, info );
	};
	NifStream( hasDecal0Texture, out, info );
	if ( (hasDecal0Texture != 0) ) {
		if ( info.version < VER_3_3_0_13 ) {
			NifStream( (unsigned int)&(*decal0Texture.source), out, info );
		} else {
			if ( decal0Texture.source != NULL ) {
				NifStream( link_map.find( StaticCast<NiObject>(decal0Texture.source) )->second, out, info );
			} else {
				NifStream( 0xFFFFFFFF, out, info );
			}
		}
		NifStream( decal0Texture.clampMode, out, info );
		NifStream( decal0Texture.filterMode, out, info );
		NifStream( decal0Texture.uvSet, out, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( decal0Texture.ps2L, out, info );
			NifStream( decal0Texture.ps2K, out, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( decal0Texture.unknown1, out, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( decal0Texture.hasTextureTransform, out, info );
			if ( (decal0Texture.hasTextureTransform != 0) ) {
				NifStream( decal0Texture.translation, out, info );
				NifStream( decal0Texture.tiling, out, info );
				NifStream( decal0Texture.wRotation, out, info );
				NifStream( decal0Texture.transformType_, out, info );
				NifStream( decal0Texture.centerOffset, out, info );
			};
		};
	};
	if ( (textureCount >= 8) ) {
		NifStream( hasDecal1Texture, out, info );
	};
	if ( (((textureCount >= 8)) && ((hasDecal1Texture != 0))) ) {
		if ( info.version < VER_3_3_0_13 ) {
			NifStream( (unsigned int)&(*decal1Texture.source), out, info );
		} else {
			if ( decal1Texture.source != NULL ) {
				NifStream( link_map.find( StaticCast<NiObject>(decal1Texture.source) )->second, out, info );
			} else {
				NifStream( 0xFFFFFFFF, out, info );
			}
		}
		NifStream( decal1Texture.clampMode, out, info );
		NifStream( decal1Texture.filterMode, out, info );
		NifStream( decal1Texture.uvSet, out, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( decal1Texture.ps2L, out, info );
			NifStream( decal1Texture.ps2K, out, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( decal1Texture.unknown1, out, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( decal1Texture.hasTextureTransform, out, info );
			if ( (decal1Texture.hasTextureTransform != 0) ) {
				NifStream( decal1Texture.translation, out, info );
				NifStream( decal1Texture.tiling, out, info );
				NifStream( decal1Texture.wRotation, out, info );
				NifStream( decal1Texture.transformType_, out, info );
				NifStream( decal1Texture.centerOffset, out, info );
			};
		};
	};
	if ( (textureCount >= 9) ) {
		NifStream( hasDecal2Texture, out, info );
	};
	if ( (((textureCount >= 9)) && ((hasDecal2Texture != 0))) ) {
		if ( info.version < VER_3_3_0_13 ) {
			NifStream( (unsigned int)&(*decal2Texture.source), out, info );
		} else {
			if ( decal2Texture.source != NULL ) {
				NifStream( link_map.find( StaticCast<NiObject>(decal2Texture.source) )->second, out, info );
			} else {
				NifStream( 0xFFFFFFFF, out, info );
			}
		}
		NifStream( decal2Texture.clampMode, out, info );
		NifStream( decal2Texture.filterMode, out, info );
		NifStream( decal2Texture.uvSet, out, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( decal2Texture.ps2L, out, info );
			NifStream( decal2Texture.ps2K, out, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( decal2Texture.unknown1, out, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( decal2Texture.hasTextureTransform, out, info );
			if ( (decal2Texture.hasTextureTransform != 0) ) {
				NifStream( decal2Texture.translation, out, info );
				NifStream( decal2Texture.tiling, out, info );
				NifStream( decal2Texture.wRotation, out, info );
				NifStream( decal2Texture.transformType_, out, info );
				NifStream( decal2Texture.centerOffset, out, info );
			};
		};
	};
	if ( (textureCount >= 10) ) {
		NifStream( hasDecal3Texture, out, info );
	};
	if ( (((textureCount >= 10)) && ((hasDecal3Texture != 0))) ) {
		if ( info.version < VER_3_3_0_13 ) {
			NifStream( (unsigned int)&(*decal3Texture.source), out, info );
		} else {
			if ( decal3Texture.source != NULL ) {
				NifStream( link_map.find( StaticCast<NiObject>(decal3Texture.source) )->second, out, info );
			} else {
				NifStream( 0xFFFFFFFF, out, info );
			}
		}
		NifStream( decal3Texture.clampMode, out, info );
		NifStream( decal3Texture.filterMode, out, info );
		NifStream( decal3Texture.uvSet, out, info );
		if ( info.version <= 0x0A020000 ) {
			NifStream( decal3Texture.ps2L, out, info );
			NifStream( decal3Texture.ps2K, out, info );
		};
		if ( info.version <= 0x0401000C ) {
			NifStream( decal3Texture.unknown1, out, info );
		};
		if ( info.version >= 0x0A010000 ) {
			NifStream( decal3Texture.hasTextureTransform, out, info );
			if ( (decal3Texture.hasTextureTransform != 0) ) {
				NifStream( decal3Texture.translation, out, info );
				NifStream( decal3Texture.tiling, out, info );
				NifStream( decal3Texture.wRotation, out, info );
				NifStream( decal3Texture.transformType_, out, info );
				NifStream( decal3Texture.centerOffset, out, info );
			};
		};
	};
	if ( info.version >= 0x0A000100 ) {
		NifStream( numShaderTextures, out, info );
		for (unsigned int i2 = 0; i2 < shaderTextures.size(); i2++) {
			NifStream( shaderTextures[i2].isUsed, out, info );
			if ( (shaderTextures[i2].isUsed != 0) ) {
				if ( info.version < VER_3_3_0_13 ) {
					NifStream( (unsigned int)&(*shaderTextures[i2].textureData.source), out, info );
				} else {
					if ( shaderTextures[i2].textureData.source != NULL ) {
						NifStream( link_map.find( StaticCast<NiObject>(shaderTextures[i2].textureData.source) )->second, out, info );
					} else {
						NifStream( 0xFFFFFFFF, out, info );
					}
				}
				NifStream( shaderTextures[i2].textureData.clampMode, out, info );
				NifStream( shaderTextures[i2].textureData.filterMode, out, info );
				NifStream( shaderTextures[i2].textureData.uvSet, out, info );
				if ( info.version <= 0x0A020000 ) {
					NifStream( shaderTextures[i2].textureData.ps2L, out, info );
					NifStream( shaderTextures[i2].textureData.ps2K, out, info );
				};
				if ( info.version <= 0x0401000C ) {
					NifStream( shaderTextures[i2].textureData.unknown1, out, info );
				};
				if ( info.version >= 0x0A010000 ) {
					NifStream( shaderTextures[i2].textureData.hasTextureTransform, out, info );
					if ( (shaderTextures[i2].textureData.hasTextureTransform != 0) ) {
						NifStream( shaderTextures[i2].textureData.translation, out, info );
						NifStream( shaderTextures[i2].textureData.tiling, out, info );
						NifStream( shaderTextures[i2].textureData.wRotation, out, info );
						NifStream( shaderTextures[i2].textureData.transformType_, out, info );
						NifStream( shaderTextures[i2].textureData.centerOffset, out, info );
					};
				};
				NifStream( shaderTextures[i2].unknownInt, out, info );
			};
		};
	};

	//--BEGIN POST-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::string NiTexturingProperty::asString( bool verbose ) const {
	//--BEGIN PRE-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//

	stringstream out;
	unsigned int array_output_count = 0;
	out << NiProperty::asString();
	numShaderTextures = (unsigned int)(shaderTextures.size());
	out << "  Flags:  " << flags << endl;
	out << "  Apply Mode:  " << applyMode << endl;
	out << "  Texture Count:  " << textureCount << endl;
	out << "  Has Base Texture:  " << hasBaseTexture << endl;
	if ( (hasBaseTexture != 0) ) {
		out << "    Source:  " << baseTexture.source << endl;
		out << "    Clamp Mode:  " << baseTexture.clampMode << endl;
		out << "    Filter Mode:  " << baseTexture.filterMode << endl;
		out << "    UV Set:  " << baseTexture.uvSet << endl;
		out << "    PS2 L:  " << baseTexture.ps2L << endl;
		out << "    PS2 K:  " << baseTexture.ps2K << endl;
		out << "    Unknown1:  " << baseTexture.unknown1 << endl;
		out << "    Has Texture Transform:  " << baseTexture.hasTextureTransform << endl;
		if ( (baseTexture.hasTextureTransform != 0) ) {
			out << "      Translation:  " << baseTexture.translation << endl;
			out << "      Tiling:  " << baseTexture.tiling << endl;
			out << "      W Rotation:  " << baseTexture.wRotation << endl;
			out << "      Transform Type?:  " << baseTexture.transformType_ << endl;
			out << "      Center Offset:  " << baseTexture.centerOffset << endl;
		};
	};
	out << "  Has Dark Texture:  " << hasDarkTexture << endl;
	if ( (hasDarkTexture != 0) ) {
		out << "    Source:  " << darkTexture.source << endl;
		out << "    Clamp Mode:  " << darkTexture.clampMode << endl;
		out << "    Filter Mode:  " << darkTexture.filterMode << endl;
		out << "    UV Set:  " << darkTexture.uvSet << endl;
		out << "    PS2 L:  " << darkTexture.ps2L << endl;
		out << "    PS2 K:  " << darkTexture.ps2K << endl;
		out << "    Unknown1:  " << darkTexture.unknown1 << endl;
		out << "    Has Texture Transform:  " << darkTexture.hasTextureTransform << endl;
		if ( (darkTexture.hasTextureTransform != 0) ) {
			out << "      Translation:  " << darkTexture.translation << endl;
			out << "      Tiling:  " << darkTexture.tiling << endl;
			out << "      W Rotation:  " << darkTexture.wRotation << endl;
			out << "      Transform Type?:  " << darkTexture.transformType_ << endl;
			out << "      Center Offset:  " << darkTexture.centerOffset << endl;
		};
	};
	out << "  Has Detail Texture:  " << hasDetailTexture << endl;
	if ( (hasDetailTexture != 0) ) {
		out << "    Source:  " << detailTexture.source << endl;
		out << "    Clamp Mode:  " << detailTexture.clampMode << endl;
		out << "    Filter Mode:  " << detailTexture.filterMode << endl;
		out << "    UV Set:  " << detailTexture.uvSet << endl;
		out << "    PS2 L:  " << detailTexture.ps2L << endl;
		out << "    PS2 K:  " << detailTexture.ps2K << endl;
		out << "    Unknown1:  " << detailTexture.unknown1 << endl;
		out << "    Has Texture Transform:  " << detailTexture.hasTextureTransform << endl;
		if ( (detailTexture.hasTextureTransform != 0) ) {
			out << "      Translation:  " << detailTexture.translation << endl;
			out << "      Tiling:  " << detailTexture.tiling << endl;
			out << "      W Rotation:  " << detailTexture.wRotation << endl;
			out << "      Transform Type?:  " << detailTexture.transformType_ << endl;
			out << "      Center Offset:  " << detailTexture.centerOffset << endl;
		};
	};
	out << "  Has Gloss Texture:  " << hasGlossTexture << endl;
	if ( (hasGlossTexture != 0) ) {
		out << "    Source:  " << glossTexture.source << endl;
		out << "    Clamp Mode:  " << glossTexture.clampMode << endl;
		out << "    Filter Mode:  " << glossTexture.filterMode << endl;
		out << "    UV Set:  " << glossTexture.uvSet << endl;
		out << "    PS2 L:  " << glossTexture.ps2L << endl;
		out << "    PS2 K:  " << glossTexture.ps2K << endl;
		out << "    Unknown1:  " << glossTexture.unknown1 << endl;
		out << "    Has Texture Transform:  " << glossTexture.hasTextureTransform << endl;
		if ( (glossTexture.hasTextureTransform != 0) ) {
			out << "      Translation:  " << glossTexture.translation << endl;
			out << "      Tiling:  " << glossTexture.tiling << endl;
			out << "      W Rotation:  " << glossTexture.wRotation << endl;
			out << "      Transform Type?:  " << glossTexture.transformType_ << endl;
			out << "      Center Offset:  " << glossTexture.centerOffset << endl;
		};
	};
	out << "  Has Glow Texture:  " << hasGlowTexture << endl;
	if ( (hasGlowTexture != 0) ) {
		out << "    Source:  " << glowTexture.source << endl;
		out << "    Clamp Mode:  " << glowTexture.clampMode << endl;
		out << "    Filter Mode:  " << glowTexture.filterMode << endl;
		out << "    UV Set:  " << glowTexture.uvSet << endl;
		out << "    PS2 L:  " << glowTexture.ps2L << endl;
		out << "    PS2 K:  " << glowTexture.ps2K << endl;
		out << "    Unknown1:  " << glowTexture.unknown1 << endl;
		out << "    Has Texture Transform:  " << glowTexture.hasTextureTransform << endl;
		if ( (glowTexture.hasTextureTransform != 0) ) {
			out << "      Translation:  " << glowTexture.translation << endl;
			out << "      Tiling:  " << glowTexture.tiling << endl;
			out << "      W Rotation:  " << glowTexture.wRotation << endl;
			out << "      Transform Type?:  " << glowTexture.transformType_ << endl;
			out << "      Center Offset:  " << glowTexture.centerOffset << endl;
		};
	};
	out << "  Has Bump Map Texture:  " << hasBumpMapTexture << endl;
	if ( (hasBumpMapTexture != 0) ) {
		out << "    Source:  " << bumpMapTexture.source << endl;
		out << "    Clamp Mode:  " << bumpMapTexture.clampMode << endl;
		out << "    Filter Mode:  " << bumpMapTexture.filterMode << endl;
		out << "    UV Set:  " << bumpMapTexture.uvSet << endl;
		out << "    PS2 L:  " << bumpMapTexture.ps2L << endl;
		out << "    PS2 K:  " << bumpMapTexture.ps2K << endl;
		out << "    Unknown1:  " << bumpMapTexture.unknown1 << endl;
		out << "    Has Texture Transform:  " << bumpMapTexture.hasTextureTransform << endl;
		if ( (bumpMapTexture.hasTextureTransform != 0) ) {
			out << "      Translation:  " << bumpMapTexture.translation << endl;
			out << "      Tiling:  " << bumpMapTexture.tiling << endl;
			out << "      W Rotation:  " << bumpMapTexture.wRotation << endl;
			out << "      Transform Type?:  " << bumpMapTexture.transformType_ << endl;
			out << "      Center Offset:  " << bumpMapTexture.centerOffset << endl;
		};
		out << "    Bump Map Luma Scale:  " << bumpMapLumaScale << endl;
		out << "    Bump Map Luma Offset:  " << bumpMapLumaOffset << endl;
		out << "    Bump Map Matrix:  " << bumpMapMatrix << endl;
	};
	out << "  Has Decal 0 Texture:  " << hasDecal0Texture << endl;
	if ( (hasDecal0Texture != 0) ) {
		out << "    Source:  " << decal0Texture.source << endl;
		out << "    Clamp Mode:  " << decal0Texture.clampMode << endl;
		out << "    Filter Mode:  " << decal0Texture.filterMode << endl;
		out << "    UV Set:  " << decal0Texture.uvSet << endl;
		out << "    PS2 L:  " << decal0Texture.ps2L << endl;
		out << "    PS2 K:  " << decal0Texture.ps2K << endl;
		out << "    Unknown1:  " << decal0Texture.unknown1 << endl;
		out << "    Has Texture Transform:  " << decal0Texture.hasTextureTransform << endl;
		if ( (decal0Texture.hasTextureTransform != 0) ) {
			out << "      Translation:  " << decal0Texture.translation << endl;
			out << "      Tiling:  " << decal0Texture.tiling << endl;
			out << "      W Rotation:  " << decal0Texture.wRotation << endl;
			out << "      Transform Type?:  " << decal0Texture.transformType_ << endl;
			out << "      Center Offset:  " << decal0Texture.centerOffset << endl;
		};
	};
	if ( (textureCount >= 8) ) {
		out << "    Has Decal 1 Texture:  " << hasDecal1Texture << endl;
	};
	if ( (((textureCount >= 8)) && ((hasDecal1Texture != 0))) ) {
		out << "    Source:  " << decal1Texture.source << endl;
		out << "    Clamp Mode:  " << decal1Texture.clampMode << endl;
		out << "    Filter Mode:  " << decal1Texture.filterMode << endl;
		out << "    UV Set:  " << decal1Texture.uvSet << endl;
		out << "    PS2 L:  " << decal1Texture.ps2L << endl;
		out << "    PS2 K:  " << decal1Texture.ps2K << endl;
		out << "    Unknown1:  " << decal1Texture.unknown1 << endl;
		out << "    Has Texture Transform:  " << decal1Texture.hasTextureTransform << endl;
		if ( (decal1Texture.hasTextureTransform != 0) ) {
			out << "      Translation:  " << decal1Texture.translation << endl;
			out << "      Tiling:  " << decal1Texture.tiling << endl;
			out << "      W Rotation:  " << decal1Texture.wRotation << endl;
			out << "      Transform Type?:  " << decal1Texture.transformType_ << endl;
			out << "      Center Offset:  " << decal1Texture.centerOffset << endl;
		};
	};
	if ( (textureCount >= 9) ) {
		out << "    Has Decal 2 Texture:  " << hasDecal2Texture << endl;
	};
	if ( (((textureCount >= 9)) && ((hasDecal2Texture != 0))) ) {
		out << "    Source:  " << decal2Texture.source << endl;
		out << "    Clamp Mode:  " << decal2Texture.clampMode << endl;
		out << "    Filter Mode:  " << decal2Texture.filterMode << endl;
		out << "    UV Set:  " << decal2Texture.uvSet << endl;
		out << "    PS2 L:  " << decal2Texture.ps2L << endl;
		out << "    PS2 K:  " << decal2Texture.ps2K << endl;
		out << "    Unknown1:  " << decal2Texture.unknown1 << endl;
		out << "    Has Texture Transform:  " << decal2Texture.hasTextureTransform << endl;
		if ( (decal2Texture.hasTextureTransform != 0) ) {
			out << "      Translation:  " << decal2Texture.translation << endl;
			out << "      Tiling:  " << decal2Texture.tiling << endl;
			out << "      W Rotation:  " << decal2Texture.wRotation << endl;
			out << "      Transform Type?:  " << decal2Texture.transformType_ << endl;
			out << "      Center Offset:  " << decal2Texture.centerOffset << endl;
		};
	};
	if ( (textureCount >= 10) ) {
		out << "    Has Decal 3 Texture:  " << hasDecal3Texture << endl;
	};
	if ( (((textureCount >= 10)) && ((hasDecal3Texture != 0))) ) {
		out << "    Source:  " << decal3Texture.source << endl;
		out << "    Clamp Mode:  " << decal3Texture.clampMode << endl;
		out << "    Filter Mode:  " << decal3Texture.filterMode << endl;
		out << "    UV Set:  " << decal3Texture.uvSet << endl;
		out << "    PS2 L:  " << decal3Texture.ps2L << endl;
		out << "    PS2 K:  " << decal3Texture.ps2K << endl;
		out << "    Unknown1:  " << decal3Texture.unknown1 << endl;
		out << "    Has Texture Transform:  " << decal3Texture.hasTextureTransform << endl;
		if ( (decal3Texture.hasTextureTransform != 0) ) {
			out << "      Translation:  " << decal3Texture.translation << endl;
			out << "      Tiling:  " << decal3Texture.tiling << endl;
			out << "      W Rotation:  " << decal3Texture.wRotation << endl;
			out << "      Transform Type?:  " << decal3Texture.transformType_ << endl;
			out << "      Center Offset:  " << decal3Texture.centerOffset << endl;
		};
	};
	out << "  Num Shader Textures:  " << numShaderTextures << endl;
	array_output_count = 0;
	for (unsigned int i1 = 0; i1 < shaderTextures.size(); i1++) {
		if ( !verbose && ( array_output_count > MAXARRAYDUMP ) ) {
			out << "<Data Truncated. Use verbose mode to see complete listing.>" << endl;
			break;
		};
		out << "    Is Used:  " << shaderTextures[i1].isUsed << endl;
		if ( (shaderTextures[i1].isUsed != 0) ) {
			out << "      Source:  " << shaderTextures[i1].textureData.source << endl;
			out << "      Clamp Mode:  " << shaderTextures[i1].textureData.clampMode << endl;
			out << "      Filter Mode:  " << shaderTextures[i1].textureData.filterMode << endl;
			out << "      UV Set:  " << shaderTextures[i1].textureData.uvSet << endl;
			out << "      PS2 L:  " << shaderTextures[i1].textureData.ps2L << endl;
			out << "      PS2 K:  " << shaderTextures[i1].textureData.ps2K << endl;
			out << "      Unknown1:  " << shaderTextures[i1].textureData.unknown1 << endl;
			out << "      Has Texture Transform:  " << shaderTextures[i1].textureData.hasTextureTransform << endl;
			if ( (shaderTextures[i1].textureData.hasTextureTransform != 0) ) {
				out << "        Translation:  " << shaderTextures[i1].textureData.translation << endl;
				out << "        Tiling:  " << shaderTextures[i1].textureData.tiling << endl;
				out << "        W Rotation:  " << shaderTextures[i1].textureData.wRotation << endl;
				out << "        Transform Type?:  " << shaderTextures[i1].textureData.transformType_ << endl;
				out << "        Center Offset:  " << shaderTextures[i1].textureData.centerOffset << endl;
			};
			out << "      Unknown Int:  " << shaderTextures[i1].unknownInt << endl;
		};
	};
	return out.str();

	//--BEGIN POST-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiTexturingProperty::FixLinks( const map<unsigned int,NiObjectRef> & objects, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiProperty::FixLinks( objects, link_stack, info );
	if ( (hasBaseTexture != 0) ) {
		baseTexture.source = FixLink<NiSourceTexture>( objects, link_stack, info );
	};
	if ( (hasDarkTexture != 0) ) {
		darkTexture.source = FixLink<NiSourceTexture>( objects, link_stack, info );
	};
	if ( (hasDetailTexture != 0) ) {
		detailTexture.source = FixLink<NiSourceTexture>( objects, link_stack, info );
	};
	if ( (hasGlossTexture != 0) ) {
		glossTexture.source = FixLink<NiSourceTexture>( objects, link_stack, info );
	};
	if ( (hasGlowTexture != 0) ) {
		glowTexture.source = FixLink<NiSourceTexture>( objects, link_stack, info );
	};
	if ( (hasBumpMapTexture != 0) ) {
		bumpMapTexture.source = FixLink<NiSourceTexture>( objects, link_stack, info );
	};
	if ( (hasDecal0Texture != 0) ) {
		decal0Texture.source = FixLink<NiSourceTexture>( objects, link_stack, info );
	};
	if ( (((textureCount >= 8)) && ((hasDecal1Texture != 0))) ) {
		decal1Texture.source = FixLink<NiSourceTexture>( objects, link_stack, info );
	};
	if ( (((textureCount >= 9)) && ((hasDecal2Texture != 0))) ) {
		decal2Texture.source = FixLink<NiSourceTexture>( objects, link_stack, info );
	};
	if ( (((textureCount >= 10)) && ((hasDecal3Texture != 0))) ) {
		decal3Texture.source = FixLink<NiSourceTexture>( objects, link_stack, info );
	};
	if ( info.version >= 0x0A000100 ) {
		for (unsigned int i2 = 0; i2 < shaderTextures.size(); i2++) {
			if ( (shaderTextures[i2].isUsed != 0) ) {
				shaderTextures[i2].textureData.source = FixLink<NiSourceTexture>( objects, link_stack, info );
			};
		};
	};

	//--BEGIN POST-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::list<NiObjectRef> NiTexturingProperty::GetRefs() const {
	list<Ref<NiObject> > refs;
	refs = NiProperty::GetRefs();
	if ( baseTexture.source != NULL )
		refs.push_back(StaticCast<NiObject>(baseTexture.source));
	if ( darkTexture.source != NULL )
		refs.push_back(StaticCast<NiObject>(darkTexture.source));
	if ( detailTexture.source != NULL )
		refs.push_back(StaticCast<NiObject>(detailTexture.source));
	if ( glossTexture.source != NULL )
		refs.push_back(StaticCast<NiObject>(glossTexture.source));
	if ( glowTexture.source != NULL )
		refs.push_back(StaticCast<NiObject>(glowTexture.source));
	if ( bumpMapTexture.source != NULL )
		refs.push_back(StaticCast<NiObject>(bumpMapTexture.source));
	if ( decal0Texture.source != NULL )
		refs.push_back(StaticCast<NiObject>(decal0Texture.source));
	if ( decal1Texture.source != NULL )
		refs.push_back(StaticCast<NiObject>(decal1Texture.source));
	if ( decal2Texture.source != NULL )
		refs.push_back(StaticCast<NiObject>(decal2Texture.source));
	if ( decal3Texture.source != NULL )
		refs.push_back(StaticCast<NiObject>(decal3Texture.source));
	for (unsigned int i1 = 0; i1 < shaderTextures.size(); i1++) {
		if ( shaderTextures[i1].textureData.source != NULL )
			refs.push_back(StaticCast<NiObject>(shaderTextures[i1].textureData.source));
	};
	return refs;
}

//--BEGIN MISC CUSTOM CODE--//

ApplyMode NiTexturingProperty::GetApplyMode() const {
	return applyMode;
}

void NiTexturingProperty::SetApplyMode( ApplyMode new_val ) {
	applyMode = new_val;
}

int NiTexturingProperty::GetTextureCount() const {
	//TODO:  Create a built in type for this array of textures so it can be a real array again?
	int count = 0;
	if ( hasBaseTexture ) { count++; }
	if ( hasBumpMapTexture ) { count++; }
	if ( hasDarkTexture ) { count++; }
	if ( hasDecal0Texture ) { count++; }
	if ( hasDecal1Texture ) { count++; }
	if ( hasDetailTexture ) { count++; }
	if ( hasGlossTexture ) { count++; }
	if ( hasGlowTexture ) { count++; }
	return count;
}

int NiTexturingProperty::GetShaderTextureCount() const {
	return int(shaderTextures.size());
}

TexDesc NiTexturingProperty::GetTexture( int n ) const {
	//TODO:  Create a built in type for this array of textures so it can be a real array again?
	//Copy the values to the right texture
	switch (n) {
		case BASE_MAP:
			return baseTexture;
		case DARK_MAP:
			return darkTexture;
		case DETAIL_MAP:
			return detailTexture;
		case GLOSS_MAP:
			return glossTexture;
		case GLOW_MAP:
			return glowTexture;
		case BUMP_MAP:
			return bumpMapTexture;
		case DECAL_0_MAP:
			return decal0Texture;
		case DECAL_1_MAP:
			return decal1Texture;
	};

	//If we get here, an invalid texture was requested.
	throw runtime_error("Invalid texture requested.");
}

TexDesc NiTexturingProperty::GetShaderTexture( int n ) const {
	return shaderTextures[n].textureData;
}

float NiTexturingProperty::GetLumaOffset() const {
	return bumpMapLumaOffset;
}

void NiTexturingProperty::SetLumaOffset( float new_val ) {
	bumpMapLumaOffset = new_val;
}

float NiTexturingProperty::GetLumaScale() const {
	return bumpMapLumaScale;
}

void NiTexturingProperty::SetLumaScale( float new_val ) {
	bumpMapLumaScale = new_val;
}

Matrix22 NiTexturingProperty::GetBumpMapMatrix() const {
	return bumpMapMatrix;
}

void NiTexturingProperty::SetBumpMapMatrix( Matrix22 & new_val ) {
	bumpMapMatrix = new_val;
}

void NiTexturingProperty::SetTextureCount( int new_count ) {

	if ( new_count < int(textureCount) ) {
		for ( int i = int(textureCount); i > new_count; --i ) {
			ClearTexture(i);
		}
	}
}

void NiTexturingProperty::SetShaderTextureCount( int new_count ) {
	//Resize array
	shaderTextures.resize( new_count );
}

void NiTexturingProperty::SetTexture( int n, TexDesc & new_val ) {
	//Make sure index is not out of range
	
	if ( n < 0 || n > int(textureCount) ) {
		throw runtime_error("SetTexture - Index out of range.  Call SetTextureCount to resize.");
	}

	//TODO:  Create a built in type for this array of textures so it can be a real array again?
	//Copy the values to the right texture
	switch (n) {
		case BASE_MAP:
			hasBaseTexture = true;
			baseTexture = new_val;
			break;
		case DARK_MAP:
			hasDarkTexture = true;
			darkTexture = new_val;
			break;
		case DETAIL_MAP:
			hasDetailTexture = true;
			detailTexture = new_val;
			break;
		case GLOSS_MAP:
			hasGlossTexture = true;
			glossTexture = new_val;
			break;
		case GLOW_MAP:
			hasGlowTexture = true;
			glowTexture = new_val;
			break;
		case BUMP_MAP:
			hasBumpMapTexture = true;
			bumpMapTexture = new_val;
			break;
		case DECAL_0_MAP:
			hasDecal0Texture = true;
			decal0Texture = new_val;
			break;
		case DECAL_1_MAP:
			hasDecal1Texture = true;
			decal1Texture = new_val;
			break;
	};
}

void NiTexturingProperty::SetShaderTexture( int n, TexDesc & new_val ) {
	//Make sure index is not out of range
	if ( n < 0 || n > int(shaderTextures.size()) ) {
		throw runtime_error("SetShaderTexture - Index out of range.  Call SetShaderTextureCount to resize.");
	}

	//Copy the values
	shaderTextures[n].textureData = new_val;
}

bool NiTexturingProperty::HasTexture( int n ) const {
	switch (n) {
		case BASE_MAP:
			return hasBaseTexture;
		case DARK_MAP:
			return hasDarkTexture;
		case DETAIL_MAP:
			return hasDetailTexture;
		case GLOSS_MAP:
			return hasGlossTexture;
		case GLOW_MAP:
			return hasGlowTexture;
		case BUMP_MAP:
			return hasBumpMapTexture;
		case DECAL_0_MAP:
			return hasDecal0Texture;
		case DECAL_1_MAP:
			return hasDecal1Texture;
		default:
			throw runtime_error("You have specified an invalid texture type.");
	};
}

void NiTexturingProperty::ClearTexture( int n ) {
	switch (n) {
		case BASE_MAP:
			hasBaseTexture = false;
			baseTexture.source = NULL;
			break;
		case DARK_MAP:
			hasDarkTexture = false;
			darkTexture.source = NULL;
			break;
		case DETAIL_MAP:
			hasDetailTexture = false;
			detailTexture.source = NULL;
			break;
		case GLOSS_MAP:
			hasGlossTexture = false;
			glossTexture.source = NULL;
			break;
		case GLOW_MAP:
			hasGlowTexture = false;
			glowTexture.source = NULL;
			break;
		case BUMP_MAP:
			hasBumpMapTexture = false;
			bumpMapTexture.source = NULL;
			break;
		case DECAL_0_MAP:
			hasDecal0Texture = false;
			decal0Texture.source = NULL;
			break;
		case DECAL_1_MAP:
			hasDecal1Texture = false;
			decal1Texture.source = NULL;
			break;
	};
}

//--END CUSTOM CODE--//
