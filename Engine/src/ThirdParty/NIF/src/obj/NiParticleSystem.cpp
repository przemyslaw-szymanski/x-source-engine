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
#include "../../include/obj/NiParticleSystem.h"
#include "../../include/obj/NiPSysModifier.h"
using namespace Niflib;

//Definition of TYPE constant
const Type NiParticleSystem::TYPE("NiParticleSystem", &NiParticles::TYPE );

NiParticleSystem::NiParticleSystem() : unknownBool(false), numModifiers((unsigned int)0) {
	//--BEGIN CONSTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

NiParticleSystem::~NiParticleSystem() {
	//--BEGIN DESTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

const Type & NiParticleSystem::GetType() const {
	return TYPE;
}

NiObject * NiParticleSystem::Create() {
	return new NiParticleSystem;
}

void NiParticleSystem::Read( istream& in, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//

	unsigned int block_num;
	NiParticles::Read( in, link_stack, info );
	if ( info.version >= 0x0A010000 ) {
		NifStream( unknownBool, in, info );
		NifStream( numModifiers, in, info );
		modifiers.resize(numModifiers);
		for (unsigned int i2 = 0; i2 < modifiers.size(); i2++) {
			NifStream( block_num, in, info );
			link_stack.push_back( block_num );
		};
	};

	//--BEGIN POST-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiParticleSystem::Write( ostream& out, const map<NiObjectRef,unsigned int> & link_map, const NifInfo & info ) const {
	//--BEGIN PRE-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiParticles::Write( out, link_map, info );
	numModifiers = (unsigned int)(modifiers.size());
	if ( info.version >= 0x0A010000 ) {
		NifStream( unknownBool, out, info );
		NifStream( numModifiers, out, info );
		for (unsigned int i2 = 0; i2 < modifiers.size(); i2++) {
			if ( info.version < VER_3_3_0_13 ) {
				NifStream( (unsigned int)&(*modifiers[i2]), out, info );
			} else {
				if ( modifiers[i2] != NULL ) {
					NifStream( link_map.find( StaticCast<NiObject>(modifiers[i2]) )->second, out, info );
				} else {
					NifStream( 0xFFFFFFFF, out, info );
				}
			}
		};
	};

	//--BEGIN POST-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::string NiParticleSystem::asString( bool verbose ) const {
	//--BEGIN PRE-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//

	stringstream out;
	unsigned int array_output_count = 0;
	out << NiParticles::asString();
	numModifiers = (unsigned int)(modifiers.size());
	out << "  Unknown Bool:  " << unknownBool << endl;
	out << "  Num Modifiers:  " << numModifiers << endl;
	array_output_count = 0;
	for (unsigned int i1 = 0; i1 < modifiers.size(); i1++) {
		if ( !verbose && ( array_output_count > MAXARRAYDUMP ) ) {
			out << "<Data Truncated. Use verbose mode to see complete listing.>" << endl;
			break;
		};
		if ( !verbose && ( array_output_count > MAXARRAYDUMP ) ) {
			break;
		};
		out << "    Modifiers[" << i1 << "]:  " << modifiers[i1] << endl;
		array_output_count++;
	};
	return out.str();

	//--BEGIN POST-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiParticleSystem::FixLinks( const map<unsigned int,NiObjectRef> & objects, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiParticles::FixLinks( objects, link_stack, info );
	if ( info.version >= 0x0A010000 ) {
		for (unsigned int i2 = 0; i2 < modifiers.size(); i2++) {
			modifiers[i2] = FixLink<NiPSysModifier>( objects, link_stack, info );
		};
	};

	//--BEGIN POST-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::list<NiObjectRef> NiParticleSystem::GetRefs() const {
	list<Ref<NiObject> > refs;
	refs = NiParticles::GetRefs();
	for (unsigned int i1 = 0; i1 < modifiers.size(); i1++) {
		if ( modifiers[i1] != NULL )
			refs.push_back(StaticCast<NiObject>(modifiers[i1]));
	};
	return refs;
}

//--BEGIN MISC CUSTOM CODE--//
//--END CUSTOM CODE--//
