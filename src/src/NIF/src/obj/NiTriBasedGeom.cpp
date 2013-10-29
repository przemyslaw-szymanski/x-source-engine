/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for license. */

//-----------------------------------NOTICE----------------------------------//
// Some of this file is automatically filled in by a Python script.  Only    //
// add custom code in the designated areas or it will be overwritten during  //
// the next update.                                                          //
//-----------------------------------NOTICE----------------------------------//

//--BEGIN FILE HEAD CUSTOM CODE--//
#include "../../include/obj/NiSkinInstance.h"
#include "../../include/obj/NiSkinData.h"
#include "../../include/obj/NiSkinPartition.h"
#include "../../include/obj/NiTriBasedGeomData.h"
#include "../../include/obj/NiBinaryExtraData.h"
//--END CUSTOM CODE--//

#include "../../include/FixLink.h"
#include "../../include/ObjectRegistry.h"
#include "../../include/NIF_IO.h"
#include "../../include/obj/NiTriBasedGeom.h"
using namespace Niflib;

//Definition of TYPE constant
const Type NiTriBasedGeom::TYPE("NiTriBasedGeom", &NiGeometry::TYPE );

NiTriBasedGeom::NiTriBasedGeom() {
	//--BEGIN CONSTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

NiTriBasedGeom::~NiTriBasedGeom() {
	//--BEGIN DESTRUCTOR CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

const Type & NiTriBasedGeom::GetType() const {
	return TYPE;
}

NiObject * NiTriBasedGeom::Create() {
	return new NiTriBasedGeom;
}

void NiTriBasedGeom::Read( istream& in, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiGeometry::Read( in, link_stack, info );

	//--BEGIN POST-READ CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiTriBasedGeom::Write( ostream& out, const map<NiObjectRef,unsigned int> & link_map, const NifInfo & info ) const {
	//--BEGIN PRE-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiGeometry::Write( out, link_map, info );

	//--BEGIN POST-WRITE CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::string NiTriBasedGeom::asString( bool verbose ) const {
	//--BEGIN PRE-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//

	stringstream out;
	unsigned int array_output_count = 0;
	out << NiGeometry::asString();
	return out.str();

	//--BEGIN POST-STRING CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

void NiTriBasedGeom::FixLinks( const map<unsigned int,NiObjectRef> & objects, list<unsigned int> & link_stack, const NifInfo & info ) {
	//--BEGIN PRE-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//

	NiGeometry::FixLinks( objects, link_stack, info );

	//--BEGIN POST-FIXLINKS CUSTOM CODE--//
	//--END CUSTOM CODE--//
}

std::list<NiObjectRef> NiTriBasedGeom::GetRefs() const {
	list<Ref<NiObject> > refs;
	refs = NiGeometry::GetRefs();
	return refs;
}

//--BEGIN MISC CUSTOM CODE--//

void NiTriBasedGeom::ClearHardareSkinInfo() {
   // Clear the partition info in both places.
   NiSkinInstanceRef skinInst = GetSkinInstance();
   if ( skinInst != NULL ) {
      skinInst->SetSkinPartition( NULL );

      NiSkinDataRef skinData = skinInst->GetSkinData();
      if (skinData != NULL) {
         skinData->SetSkinPartition( NULL );
      }
   }
}


void NiTriBasedGeom::GenHardwareSkinInfo( int max_bones_per_partition /*= 4*/, int max_bones_per_vertex /*= INT_MAX*/ ) {
   NiSkinPartitionRef skinPart; 
   if ( max_bones_per_partition == 0 ) //old method
      skinPart = new NiSkinPartition( this );
   else
      skinPart = new NiSkinPartition( this, max_bones_per_partition, max_bones_per_vertex );

   // Set the partition info in both places and it will be handled when exported.
   NiSkinInstanceRef skinInst = GetSkinInstance();
   if ( skinInst != NULL ) {
      skinInst->SetSkinPartition( skinPart );

      NiSkinDataRef skinData = skinInst->GetSkinData();
      if (skinData != NULL) {
         skinData->SetSkinPartition( skinPart );
      }
   }
}

void NiTriBasedGeom::UpdateTangentSpace() {

	NiTriBasedGeomDataRef niTriGeomData = DynamicCast<NiTriBasedGeomData>(this->data);

	/* No data, no tangent space */
	if( niTriGeomData == NULL ) {
		throw runtime_error("There is no NiTriBasedGeomData attached the NiGeometry upon which UpdateTangentSpace was called.");
	}

	//Check if there are any UVs or Vertices before trying to retrive them
	if ( niTriGeomData->GetUVSetCount() == 0 ) {
		//There are no UVs, do nothing
		return;
	}

	if ( niTriGeomData->GetVertexCount() == 0 ) {
		//There are no Vertices, do nothing
		return;
	}

	//Get mesh data from data object
	vector<Vector3> verts = niTriGeomData->GetVertices();
	vector<Vector3> norms = niTriGeomData->GetNormals();
	vector<Triangle> tris = niTriGeomData->GetTriangles();
	vector<TexCoord> uvs = niTriGeomData->GetUVSet(0);

	/* check for data validity */
	if(
		verts.size() != norms.size() ||
		verts.size() != uvs.size() ||
		tris.empty()
	) {
		//Do nothing, there is no shape in this data.
		return;
	}

	vector<Vector3> tangents( verts.size() );
	vector<Vector3> binormals( verts.size() );

	for( int t = 0; t < (int)tris.size(); t++ ) {
		Triangle & tri = tris[t];

		int i1 = tri[0];
		int i2 = tri[1];
		int i3 = tri[2];
		
		const Vector3 v1 = verts[i1];
		const Vector3 v2 = verts[i2];
		const Vector3 v3 = verts[i3];
		
		const TexCoord w1 = uvs[i1];
		const TexCoord w2 = uvs[i2];
		const TexCoord w3 = uvs[i3];
		
		Vector3 v2v1 = v2 - v1;
		Vector3 v3v1 = v3 - v1;
		
		TexCoord w2w1(w2.u - w1.u, w2.v - w1.v);
		TexCoord w3w1(w3.u - w1.u, w3.v - w1.v);
		
		float r = w2w1.u * w3w1.v - w3w1.u * w2w1.v;
		
		if ( abs( r ) <= 10e-5 ){
			continue;
		}
		
		r = 1.0f / r;
		
		Vector3 sdir( 
			( w3w1.v * v2v1.x - w2w1.v * v3v1.x ) * r,
			( w3w1.v * v2v1.y - w2w1.v * v3v1.y ) * r,
			( w3w1.v * v2v1.z - w2w1.v * v3v1.z ) * r
		);
		
		Vector3 tdir( 
			( w2w1.u * v3v1.x - w3w1.u * v2v1.x ) * r,
			( w2w1.u * v3v1.y - w3w1.u * v2v1.y ) * r,
			( w2w1.u * v3v1.z - w3w1.u * v2v1.z ) * r
		);

		// no duplication, just smoothing
		for ( int j = 0; j < 3; j++ ) {	
			int i = tri[j];
			
			tangents[i] += sdir.Normalized();
			binormals[i] += tdir.Normalized();
		}
	}

	// for each vertex calculate tangent and binormal
	for ( unsigned i = 0; i < verts.size(); i++ ) {	
		const Vector3 & n = norms[i];
		
		Vector3 & t = tangents[i];
		Vector3 & b = binormals[i];
		
		if ( t == Vector3() || b == Vector3() ) {
			t.x = n.y;
			t.y = n.z;
			t.z = n.x;
			b = n.CrossProduct(t);
		} else {
			t = ( t - n * n.DotProduct(t) );
			t = t.Normalized();
			b = ( b - n * n.DotProduct(b) );
			b = b.Normalized();
		}
	}

	// generate the byte data
	unsigned vCount = verts.size();
	int fSize = sizeof(float[3]);
	vector<byte> binData( 2 * vCount * fSize );

	for( unsigned i = 0; i < verts.size(); i++ ) {
		float tan_xyz[3], bin_xyz[3];

		tan_xyz[0] = tangents[i].x;
		tan_xyz[1] = tangents[i].y;
		tan_xyz[2] = tangents[i].z;

		bin_xyz[0] = binormals[i].x;
		bin_xyz[1] = binormals[i].y;
		bin_xyz[2] = binormals[i].z;

		char * tan_Bytes = (char*)tan_xyz;
		char * bin_Bytes = (char*)bin_xyz;

		for( int j = 0; j < fSize; j++ ) {
			binData[ i           * fSize + j] = tan_Bytes[j];
			binData[(i + vCount) * fSize + j] = bin_Bytes[j];
		}
	}

	// update or create the tangent space extra data
	NiBinaryExtraDataRef TSpaceRef;

	std::list<NiExtraDataRef> props = this->GetExtraData();
	std::list<NiExtraDataRef>::iterator prop;

	for( prop = props.begin(); prop != props.end(); ++prop ){
		if((*prop)->GetName() == "Tangent space (binormal & tangent vectors)") {
			TSpaceRef = DynamicCast<NiBinaryExtraData>(*prop);
			break;
		}
	}

	if( TSpaceRef == NULL ) {
		TSpaceRef = new NiBinaryExtraData();
		TSpaceRef->SetName( "Tangent space (binormal & tangent vectors)" );
		this->AddExtraData( StaticCast<NiExtraData>(TSpaceRef) );
	}

	TSpaceRef->SetData(binData);
}

//--END CUSTOM CODE--//
