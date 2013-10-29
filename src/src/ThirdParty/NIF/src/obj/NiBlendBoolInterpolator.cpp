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
#include "../../include/obj/NiBlendBoolInterpolator.h"
using namespace Niflib;

//Definition of TYPE constant
const Type NiBlendBoolInterpolator::TYPE("NiBlendBoolInterpolator", &NiBlendInterpolator::TYPE );

NiBlendBoolInterpolator::NiBlendBoolInterpolator() : boolValue((byte)0) {
	//--BEGIN CONSTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

NiBlendBoolInterpolator::~NiBlendBoolInterpolator() {
	//--BEGIN DESTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

const Type & NiBlendBoolInterpolator::GetType() const {
	return TYPE;
}

NiObject * NiBlendBoolInterpolator::Create() {
	return new NiBlendBoolInterpolator;
}

void NiBlendBoolInterpolator::Read( istream& in, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiBlendInterpolator::Read( in, link_stack, info );
	NifStream( boolValue, in, info );

	//--BEGIN POST-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiBlendBoolInterpolator::Write( ostream& out, const map<NiObjectRef,unsigned int> & link_map, const NifInfo & info ) const {
	//--BEGIN PRE-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiBlendInterpolator::Write( out, link_map, info );
	NifStream( boolValue, out, info );

	//--BEGIN POST-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::string NiBlendBoolInterpolator::asString( bool verbose ) const {
	//--BEGIN PRE-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//

	stringstream out;
	unsigned int array_output_count = 0;
	out << NiBlendInterpolator::asString();
	out << "  Bool Value:  " << boolValue << endl;
	return out.str();

	//--BEGIN POST-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiBlendBoolInterpolator::FixLinks( const map<unsigned int,NiObjectRef> & objects, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiBlendInterpolator::FixLinks( objects, link_stack, info );

	//--BEGIN POST-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::list<NiObjectRef> NiBlendBoolInterpolator::GetRefs() const {
	list<Ref<NiObject> > refs;
	refs = NiBlendInterpolator::GetRefs();
	return refs;
}

//--BEGIN MISC CUSTOM CODE--//

byte NiBlendBoolInterpolator::GetBoolValue() const {
	return boolValue;
}

void NiBlendBoolInterpolator::SetBoolValue( byte value ) {
	boolValue = value;
}

//--END CUSTOM CODE--//
