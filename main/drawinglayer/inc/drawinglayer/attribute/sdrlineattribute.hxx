/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sdrattribute.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: aw $ $Date: 2008-05-27 14:11:16 $
 *
 *  The Contents of this file are made available subject to
 *  the terms of GNU Lesser General Public License Version 2.1.
 *
 *
 *    GNU Lesser General Public License Version 2.1
 *    =============================================
 *    Copyright 2005 by Sun Microsystems, Inc.
 *    901 San Antonio Road, Palo Alto, CA 94303, USA
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License version 2.1, as published by the Free Software Foundation.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *    MA  02111-1307  USA
 *
 ************************************************************************/

#ifndef INCLUDED_DRAWINGLAYER_ATTRIBUTE_SDRLINEATTRIBUTE_HXX
#define INCLUDED_DRAWINGLAYER_ATTRIBUTE_SDRLINEATTRIBUTE_HXX

#include <drawinglayer/drawinglayerdllapi.h>
#include <basegfx/vector/b2enums.hxx>
#include <vector>

//////////////////////////////////////////////////////////////////////////////
// predefines

namespace basegfx {
	class BColor;
}

namespace drawinglayer { namespace attribute {
	class ImpSdrLineAttribute;
}}

//////////////////////////////////////////////////////////////////////////////

namespace drawinglayer
{
	namespace attribute
	{
		class DRAWINGLAYER_DLLPUBLIC SdrLineAttribute
		{
        private:
            ImpSdrLineAttribute*                mpSdrLineAttribute;

		public:
            /// constructors/assignmentoperator/destructor
			SdrLineAttribute(
				basegfx::B2DLineJoin eJoin, 
                double fWidth, 
                double fTransparence, 
                const basegfx::BColor& rColor, 
				const ::std::vector< double >& rDotDashArray, 
                double fFullDotDashLen);
            SdrLineAttribute(const basegfx::BColor& rColor);
			SdrLineAttribute();
			SdrLineAttribute(const SdrLineAttribute& rCandidate);
			SdrLineAttribute& operator=(const SdrLineAttribute& rCandidate);
			~SdrLineAttribute();

            // checks if the incarnation is default constructed
            bool isDefault() const;

            // compare operator
			bool operator==(const SdrLineAttribute& rCandidate) const;

			// data access
			basegfx::B2DLineJoin getJoin() const;
			double getWidth() const;
			double getTransparence() const;
			const basegfx::BColor& getColor() const;
			const ::std::vector< double >& getDotDashArray() const;
			double getFullDotDashLen() const;

			// bool access
			bool isDashed() const;
		};
	} // end of namespace attribute
} // end of namespace drawinglayer

//////////////////////////////////////////////////////////////////////////////

#endif //INCLUDED_DRAWINGLAYER_ATTRIBUTE_SDRLINEATTRIBUTE_HXX

// eof
