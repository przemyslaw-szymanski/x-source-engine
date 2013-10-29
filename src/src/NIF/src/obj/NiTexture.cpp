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
#include "../../include/obj/NiTexture.h"
using namespace Niflib;

//Definition of TYPE constant
const Type NiTexture::TYPE("NiTexture", &NiObjectNET::TYPE );

NiTexture::NiTexture() {
	//--BEGIN CONSTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

NiTexture::~NiTexture() {
	//--BEGIN DESTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

const Type & NiTexture::GetType() const {
	return TYPE;
}

NiObject * NiTexture::Create() {
	return new NiTexture;
}

void NiTexture::Read( istream& in, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiObjectNET::Read( in, link_stack, info );

	//--BEGIN POST-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiTexture::Write( ostream& out, const map<NiObjectRef,unsigned int> & link_map, const NifInfo & info ) const {
	//--BEGIN PRE-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiObjectNET::Write( out, link_map, info );

	//--BEGIN POST-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::string NiTexture::asString( bool verbose ) const {
	//--BEGIN PRE-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//

	stringstream out;
	unsigned int array_output_count = 0;
	out << NiObjectNET::asString();
	return out.str();

	//--BEGIN POST-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiTexture::FixLinks( const map<unsigned int,NiObjectRef> & objects, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiObjectNET::FixLinks( objects, link_stack, info );

	//--BEGIN POST-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::list<NiObjectRef> NiTexture::GetRefs() const {
	list<Ref<NiObject> > refs;
	refs = NiObjectNET::GetRefs();
	return refs;
}

//--BEGIN MISC CUSTOM CODE--//
//--END CUSTOM CODE--//
