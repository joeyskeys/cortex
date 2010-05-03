//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007-2008, Image Engine Design Inc. All rights reserved.
//
//  Copyright 2010 Dr D Studios Pty Limited (ACN 127 184 954) (Dr. D Studios),
//  its affiliates and/or its licensors.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the name of Image Engine Design nor the names of any
//       other contributors to this software may be used to endorse or
//       promote products derived from this software without specific prior
//       written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

#include "IECore/SmoothSkinningData.h"
#include "IECore/VectorTypedData.h"
#include "boost/format.hpp"

using namespace IECore;
using namespace std;
using namespace boost;

const unsigned int SmoothSkinningData::m_ioVersion = 0;

IE_CORE_DEFINEOBJECTTYPEDESCRIPTION(SmoothSkinningData);

SmoothSkinningData::SmoothSkinningData() :
	m_influenceNames( new StringVectorData ),
	m_influencePoses( new M44fVectorData ),
	m_pointIndexOffsets( new IntVectorData ),
	m_pointInfluenceCounts( new IntVectorData ),
	m_pointInfluenceIndices( new IntVectorData ),
	m_pointInfluenceWeights( new FloatVectorData )
{
}

SmoothSkinningData::SmoothSkinningData( ConstStringVectorDataPtr influenceNames,
										ConstM44fVectorDataPtr influencePoses,
										ConstIntVectorDataPtr pointIndexOffsets,
										ConstIntVectorDataPtr pointInfluenceCounts,
										ConstIntVectorDataPtr pointInfluenceIndices,
										ConstFloatVectorDataPtr pointInfluenceWeights)
{
	assert( influenceNames );
	assert( influencePoses );
	assert( pointIndexOffsets );
	assert( pointInfluenceCounts );
	assert( pointInfluenceIndices );
	assert( pointInfluenceWeights );

	m_influenceNames = influenceNames->copy();
	m_influencePoses = influencePoses->copy();
	m_pointIndexOffsets = pointIndexOffsets->copy();
	m_pointInfluenceCounts = pointInfluenceCounts->copy();
	m_pointInfluenceIndices = pointInfluenceIndices->copy();
	m_pointInfluenceWeights = pointInfluenceWeights->copy();
}

SmoothSkinningData::~SmoothSkinningData()
{
}

ConstStringVectorDataPtr SmoothSkinningData::influenceNames() const
{
	return m_influenceNames;
}

StringVectorDataPtr SmoothSkinningData::influenceNames()
{
	return m_influenceNames;
}

ConstM44fVectorDataPtr SmoothSkinningData::influencePoses() const
{
	return m_influencePoses;
}

M44fVectorDataPtr SmoothSkinningData::influencePoses()
{
	return m_influencePoses;
}

ConstIntVectorDataPtr SmoothSkinningData::pointIndexOffsets() const
{
	return m_pointIndexOffsets;
}

IntVectorDataPtr SmoothSkinningData::pointIndexOffsets()
{
	return m_pointIndexOffsets;
}

ConstIntVectorDataPtr SmoothSkinningData::pointInfluenceCounts() const
{
	return m_pointInfluenceCounts;
}

IntVectorDataPtr SmoothSkinningData::pointInfluenceCounts()
{
	return m_pointInfluenceCounts;
}

ConstIntVectorDataPtr SmoothSkinningData::pointInfluenceIndices() const
{
	return m_pointInfluenceIndices;
}

IntVectorDataPtr SmoothSkinningData::pointInfluenceIndices()
{
	return m_pointInfluenceIndices;
}

ConstFloatVectorDataPtr SmoothSkinningData::pointInfluenceWeights() const
{
	return m_pointInfluenceWeights;
}

FloatVectorDataPtr SmoothSkinningData::pointInfluenceWeights()
{
	return m_pointInfluenceWeights;
}

void SmoothSkinningData::validateSizes() const
{
	int cin = m_influenceNames->readable().size();
	int cip = m_influencePoses->readable().size();

	// check vector sizes
	if (cin != cip)
	{
		string error = str( format( "SmoothSkinningData: Number of influenceNames '%e' does not match number of influencePoses '%e'!" ) % cin % cip);
		throw Exception( error );
	}

	int cpio = m_pointIndexOffsets->readable().size();
	int cpic =  m_pointInfluenceCounts->readable().size();

	if (cpio != cpic)
	{
		string error = str( format( "SmoothSkinningData: Number of pointIndexOffsets '%e' does not match number of pointInfluenceCounts '%e'!" ) % cpio % cpic);
		throw Exception( error );
	}

	int cpii = m_pointInfluenceIndices->readable().size();
	int cpiw =  m_pointInfluenceWeights->readable().size();

	if (cpii != cpiw)
	{
		string error = str( format( "SmoothSkinningData: Number of pointInfluenceIndices '%e' does not match number of pointInfluenceWeights '%e'!" ) % cpii % cpiw);
		throw Exception( error );
	}
}

void SmoothSkinningData::validateCounts() const
{
	// check for wrong counts in m_pointInfluenceCounts
	int sum =0;
	for( std::vector<int>::const_iterator it = m_pointInfluenceCounts->readable().begin();
		 it!=m_pointInfluenceCounts->readable().end(); ++it )
	{
		sum += (*it);
	}

	int cpii = m_pointInfluenceIndices->readable().size();
	if ( sum != cpii )
	{
		string error = str( format( "SmoothSkinningData: Sum of all pointInfluenceCounts '%e' does not match size of pointInfluenceIndices and pointInfluenceWeightsmatch number of pointInfluenceWeights '%e'!" ) % sum % cpii);
		throw Exception( error );
	}
}

void SmoothSkinningData::validateIds() const
{
	int cpii = m_pointInfluenceIndices->readable().size();
	int cin = m_influenceNames->readable().size();
	// check for invalid ids in m_pointIndexOffset
	for( std::vector<int>::const_iterator it = m_pointIndexOffsets->readable().begin();
		 it!=m_pointIndexOffsets->readable().end(); ++it )
	{
		int o = (*it);
		if ( ( o < 0 ) or ( o > (cpii-1) ) )
		{
			int id = it - m_pointIndexOffsets->readable().begin();
			string error = str( format( "SmoothSkinningData: pointIndexOffset[%e] with value '%e' is not pointing to valid index in pointInfluenceWeights vector range [ 0, %e ]!" ) % id % o % (cpii-1) );
			throw Exception( error );
		}
	}

	// check for invalid ids in_pointInfluenceIndices
	for( std::vector<int>::const_iterator it = m_pointInfluenceIndices->readable().begin();
		 it!=m_pointInfluenceIndices->readable().end(); ++it )
	{
		int o = (*it);
		if ( ( o < 0 ) or ( o > (cin-1) ) )
		{
			int id = it - m_pointInfluenceIndices->readable().begin();
			string error = str( format( "SmoothSkinningData: pointInfluenceIndices[%e] with value '%e' is not pointing to valid index in influenceNames vector range [ 0, %e ]!" ) % id % o % (cin-1) );
			throw Exception( error );
		}
	}
}

void SmoothSkinningData::validateOffsets() const
{
	// check for mismatches of m_pointInfluenceCounts and m_pointIndexOffsets
	std::vector<int>::const_iterator pio_it = m_pointIndexOffsets->readable().begin();
	int sum = 0;
	int lastOffset = 0;
	for( std::vector<int>::const_iterator pic_it = m_pointInfluenceCounts->readable().begin();
		 pic_it!=m_pointInfluenceCounts->readable().end(); ++pic_it )
	{
		int pic = (*pic_it);
		int pio = (*pio_it);


		if (sum != pio)
		{
			int id = pio_it - m_pointIndexOffsets->readable().begin();
			string error = str( format( "SmoothSkinningData: pointInfluenceOffsets[%e] is pointing to index '%e', but sum of all pointInfluenceCounts up to this id is '%e'!" ) % id % pio % sum );
			throw Exception( error );
		}
		sum += pic;

		lastOffset = pio;
		++pio_it;
	}
}

void SmoothSkinningData::validate() const
{
	validateSizes();
	validateIds();
	validateCounts();
	validateOffsets();
}

void SmoothSkinningData::copyFrom( ConstObjectPtr other, IECore::Object::CopyContext *context )
{
	Data::copyFrom( other, context );
	const SmoothSkinningData *tOther = static_cast<const SmoothSkinningData *>( other.get() );
	m_influenceNames = tOther->m_influenceNames->copy();
	m_influencePoses = tOther->m_influencePoses->copy();
	m_pointIndexOffsets = tOther->m_pointIndexOffsets->copy();
	m_pointInfluenceCounts = tOther->m_pointInfluenceCounts->copy();
	m_pointInfluenceIndices = tOther->m_pointInfluenceIndices->copy();
	m_pointInfluenceWeights = tOther->m_pointInfluenceWeights->copy();
}

void SmoothSkinningData::save( IECore::Object::SaveContext *context ) const
{
	Data::save(context);
	IndexedIOInterfacePtr container = context->container( staticTypeName(), m_ioVersion );
	context->save( m_influenceNames, container, "influenceNames" );
	context->save( m_influencePoses, container, "influencePoses" );
	context->save( m_pointIndexOffsets, container, "pointIndexOffsets" );
	context->save( m_pointInfluenceCounts, container, "pointInfluenceCounts" );
	context->save( m_pointInfluenceIndices, container, "pointInfluenceIndices" );
	context->save( m_pointInfluenceWeights, container, "pointInfluenceWeights" );
}

void SmoothSkinningData::load( IECore::Object::LoadContextPtr context )
{
	Data::load(context);
	unsigned int v = m_ioVersion;

	IndexedIOInterfacePtr container = context->container( staticTypeName(), v );

	m_influenceNames = context->load<StringVectorData>( container, "influenceNames" );
	m_influencePoses = context->load<M44fVectorData>( container, "influencePoses" );
	m_pointIndexOffsets = context->load<IntVectorData>( container, "pointIndexOffsets" );
	m_pointInfluenceCounts = context->load<IntVectorData>( container, "pointInfluenceCounts" );
	m_pointInfluenceIndices = context->load<IntVectorData>( container, "pointInfluenceIndices" );
	m_pointInfluenceWeights = context->load<FloatVectorData>( container, "pointInfluenceWeights" );

}

bool SmoothSkinningData::isEqualTo( ConstObjectPtr other ) const
{

	if( !Data::isEqualTo( other ) )
	{
		return false;
	}

	const SmoothSkinningData *tOther = static_cast<const SmoothSkinningData *>( other.get() );

	if(	!m_influenceNames->isEqualTo( tOther->m_influenceNames ) )
	{
		return false;
	}

	if(	!m_influencePoses->isEqualTo( tOther->m_influencePoses ) )
	{
		return false;
	}

	if(	!m_influencePoses->isEqualTo( tOther->m_influencePoses ) )
	{
		return false;
	}

	if(	!m_pointInfluenceCounts->isEqualTo( tOther->m_pointInfluenceCounts ) )
	{
		return false;
	}

	if(	!m_pointInfluenceIndices->isEqualTo( tOther->m_pointInfluenceIndices ) )
	{
		return false;
	}

	if(	!m_pointInfluenceWeights->isEqualTo( tOther->m_pointInfluenceWeights ) )
	{
		return false;
	}

	return true;
}

void SmoothSkinningData::memoryUsage( Object::MemoryAccumulator &a ) const
{
	Data::memoryUsage( a );
	a.accumulate( m_influenceNames );
	a.accumulate( m_influencePoses );
	a.accumulate( m_pointIndexOffsets );
	a.accumulate( m_pointInfluenceCounts );
	a.accumulate( m_pointInfluenceIndices );
	a.accumulate( m_pointInfluenceWeights );
}