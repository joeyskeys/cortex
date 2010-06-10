//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010, Image Engine Design Inc. All rights reserved.
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

#ifndef IE_CORE_SMOOTHSKINNINGDATA_H
#define IE_CORE_SMOOTHSKINNINGDATA_H

#include "IECore/Data.h"
#include "IECore/VectorTypedData.h"

namespace IECore
{
/// Defines a data class for storing smooth skinning data along with influenceNames and their respective pre-bind matrices.

IE_CORE_FORWARDDECLARE( SmoothSkinningData )

class SmoothSkinningData : public Data
{
	public:

		IE_CORE_DECLAREOBJECT( SmoothSkinningData, Data );

		const StringVectorData *influenceNames() const;
		StringVectorData *influenceNames() ;

		const M44fVectorData *influencePose() const;
		M44fVectorData *influencePose() ;

		const IntVectorData *pointIndexOffsets() const;
		IntVectorData *pointIndexOffsets() ;

		const IntVectorData *pointInfluenceCounts() const;
		IntVectorData *pointInfluenceCounts() ;

		const IntVectorData *pointInfluenceIndices() const;
		IntVectorData *pointInfluenceIndices() ;

		const FloatVectorData *pointInfluenceWeights() const;
		FloatVectorData *pointInfluenceWeights() ;

		/// Assign-All Constructor
		SmoothSkinningData( ConstStringVectorDataPtr influenceNames,
							ConstM44fVectorDataPtr influencePose,
							ConstIntVectorDataPtr pointIndexOffsets,
							ConstIntVectorDataPtr pointInfluenceCounts,
							ConstIntVectorDataPtr pointInfluenceIndices,
							ConstFloatVectorDataPtr pointInfluenceWeights);

		/// Default constructor
		SmoothSkinningData();

		virtual ~SmoothSkinningData();

		/// raises an exception if the smooth skinning data is not valid
		void validate() const;

	private:

		static const unsigned int m_ioVersion;

		StringVectorDataPtr m_influenceNames;
		M44fVectorDataPtr m_influencePose;
		IntVectorDataPtr m_pointIndexOffsets;
		IntVectorDataPtr m_pointInfluenceCounts;
		IntVectorDataPtr m_pointInfluenceIndices;
		FloatVectorDataPtr m_pointInfluenceWeights;

		void validateSizes() const;
		void validateCounts() const;
		void validateIds() const;
		void validateOffsets() const;
};

} // namespace

#endif // IE_CORE_SMOOTHSKINNINGDATA_H