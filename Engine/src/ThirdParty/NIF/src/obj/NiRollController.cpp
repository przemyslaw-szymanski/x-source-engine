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
#include "../../include/obj/NiRollController.h"
#include "../../include/obj/NiFloatData.h"
using namespace Niflib;

//Definition of TYPE constant
const Type NiRollController::TYPE("NiRollController", &NiSingleInterpController::TYPE );

NiRollController::NiRollController() : data(NULL) {
	//--BEGIN CONSTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

NiRollController::~NiRollController() {
	//--BEGIN DESTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

const Type & NiRollController::GetType() const {
	return TYPE;
}

NiObject * NiRollController::Create() {
	return new NiRollController;
}

void NiRollController::Read( istream& in, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//

	unsigned int block_num;
	NiSingleInterpController::Read( in, link_stack, info );
	NifStream( block_num, in, info );
	link_stack.push_back( block_num );

	//--BEGIN POST-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiRollController::Write( ostream& out, const map<NiObjectRef,unsigned int> & link_map, const NifInfo & info ) const {
	//--BEGIN PRE-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiSingleInterpController::Write( out, link_map, info );
	if ( info.version < VER_3_3_0_13 ) {
		NifStream( (unsigned int)&(*data), out, info );
	} else {
		if ( data != NULL ) {
			NifStream( link_map.find( StaticCast<NiObject>(data) )->second, out, info );
		} else {
			NifStream( 0xFFFFFFFF, out, info );
		}
	}

	//--BEGIN POST-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::string NiRollController::asString( bool verbose ) const {
	//--BEGIN PRE-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//

	stringstream out;
	unsigned int array_output_count = 0;
	out << NiSingleInterpController::asString();
	out << "  Data:  " << data << endl;
	return out.str();

	//--BEGIN POST-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiRollController::FixLinks( const map<unsigned int,NiObjectRef> & objects, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiSingleInterpController::FixLinks( objects, link_stack, info );
	data = FixLink<NiFloatData>( objects, link_stack, info );

	//--BEGIN POST-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::list<NiObjectRef> NiRollController::GetRefs() const {
	list<Ref<NiObject> > refs;
	refs = NiSingleInterpController::GetRefs();
	if ( data != NULL )
		refs.push_back(StaticCast<NiObject>(data));
	return refs;
}

//--BEGIN MISC CUSTOM CODE--//
//--END CUSTOM CODE--//
