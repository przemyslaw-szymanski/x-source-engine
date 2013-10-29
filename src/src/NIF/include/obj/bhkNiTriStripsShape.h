/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for license. */

//-----------------------------------NOTICE----------------------------------//
// Some of this file is automatically filled in by a Python script.  Only    //
// add custom code in the designated areas or it will be overwritten during  //
// the next update.                                                          //
//-----------------------------------NOTICE----------------------------------//

#ifndef _BHKNITRISTRIPSSHAPE_H_
#define _BHKNITRISTRIPSSHAPE_H_

//--BEGIN FILE HEAD CUSTOM CODE--//
//--END CUSTOM CODE--//

#include "bhkShapeCollection.h"

// Include structures
#include "../Ref.h"
#include "../gen/OblivionColFilter.h"
namespace Niflib {

// Forward define of referenced NIF objects
class NiTriStripsData;
class bhkNiTriStripsShape;
typedef Ref<bhkNiTriStripsShape> bhkNiTriStripsShapeRef;

/*! A shape constructed from a bunch of strips. */
class bhkNiTriStripsShape : public bhkShapeCollection {
public:
	/*! Constructor */
	NIFLIB_API bhkNiTriStripsShape();

	/*! Destructor */
	NIFLIB_API virtual ~bhkNiTriStripsShape();

	/*!
	 * A constant value which uniquly identifies objects of this type.
	 */
	NIFLIB_API static const Type TYPE;

	/*!
	 * A factory function used during file reading to create an instance of this type of object.
	 * \return A pointer to a newly allocated instance of this type of object.
	 */
	NIFLIB_API static NiObject * Create();

	/*!
	 * Summarizes the information contained in this object in English.
	 * \param[in] verbose Determines whether or not detailed information about large areas of data will be printed out.
	 * \return A string containing a summary of the information within the object in English.  This is the function that Niflyze calls to generate its analysis, so the output is the same.
	 */
	NIFLIB_API virtual string asString( bool verbose = false ) const;

	/*!
	 * Used to determine the type of a particular instance of this object.
	 * \return The type constant for the actual type of the object.
	 */
	NIFLIB_API virtual const Type & GetType() const;

	//--BEGIN MISC CUSTOM CODE--//

	/*!
	 * Gets the scale. Usually (1.0, 1.0, 1.0).
	 * \return The scale.
	 */
	NIFLIB_API Vector3 GetScale() const;

	/*!
	 * Sets the scale. Usually (1.0, 1.0, 1.0).
	 * \param[in] n The new scale.
	 */
	NIFLIB_API void SetScale( const Vector3 & n );

	/*!
	 * Sets the number of NiTriStripsData objects referenced by this shape.
	 * \param[in] i The new number of NiTriStripsData objects.
	 */
	NIFLIB_API void	SetNumStripsData( int i );

	/*!
	 * Sets the NiTriStripsData object referenced by this shape at the specified index.
	 * \param[in] index The index at which the given NiTriStripsData object will be referenced.  Should be lower than the value set with bhkNiTriStripsShape::SetNumStripsData.
	 * \param[in] strips The new NiTriStripsData object to be referenced by this shape at the specified index.
	 */
	NIFLIB_API void	SetStripsData( int index, NiTriStripsData * strips );

	//--END CUSTOM CODE--//
protected:
	/*! The shape's material. */
	HavokMaterial material;
	/*! Unknown. */
	float unknownFloat1;
	/*! Unknown. */
	unsigned int unknownInt1;
	/*! Unknown. */
	array<4,unsigned int > unknownInts1;
	/*! Unknown */
	unsigned int unknownInt2;
	/*! Scale. Usually (1.0, 1.0, 1.0). */
	Vector3 scale;
	/*! Unknown. */
	unsigned int unknownInt3;
	/*! The number of strips data objects referenced. */
	mutable unsigned int numStripsData;
	/*! Refers to a bunch of NiTriStripsData objects that make up this shape. */
	vector<Ref<NiTriStripsData > > stripsData;
	/*! Number of Havok Layers, equal to Number of strips data objects. */
	mutable unsigned int numDataLayers;
	/*! Havok Layers for each strip data. */
	vector<OblivionColFilter > dataLayers;
public:
	/*! NIFLIB_HIDDEN function.  For internal use only. */
	NIFLIB_HIDDEN virtual void Read( istream& in, list<unsigned int> & link_stack, const NifInfo & info );
	/*! NIFLIB_HIDDEN function.  For internal use only. */
	NIFLIB_HIDDEN virtual void Write( ostream& out, const map<NiObjectRef,unsigned int> & link_map, const NifInfo & info ) const;
	/*! NIFLIB_HIDDEN function.  For internal use only. */
	NIFLIB_HIDDEN virtual void FixLinks( const map<unsigned int,NiObjectRef> & objects, list<unsigned int> & link_stack, const NifInfo & info );
	/*! NIFLIB_HIDDEN function.  For internal use only. */
	NIFLIB_HIDDEN virtual list<NiObjectRef> GetRefs() const;
};

//--BEGIN FILE FOOT CUSTOM CODE--//
//--END CUSTOM CODE--//

} //End Niflib namespace
#endif
