/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#include <com/sun/star/xml/sax/FastToken.hpp>
#include <com/sun/star/drawing/LineStyle.hpp>
#include <com/sun/star/beans/XMultiPropertySet.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/container/XNamed.hpp>

#include "oox/helper/attributelist.hxx"
#include "oox/drawingml/shapecontext.hxx"
#include "oox/drawingml/shapestylecontext.hxx"
#include "oox/drawingml/fillpropertiesgroupcontext.hxx"
#include "oox/drawingml/lineproperties.hxx"
#include "oox/drawingml/drawingmltypes.hxx"
#include "oox/drawingml/customshapegeometry.hxx"
#include "oox/drawingml/textbodycontext.hxx"

using rtl::OUString;
using namespace oox::core;
using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::drawing;
using namespace ::com::sun::star::beans;
using namespace ::com::sun::star::text;
using namespace ::com::sun::star::xml::sax;

namespace oox { namespace drawingml {

// CT_Shape
ShapeContext::ShapeContext( ContextHandler& rParent, ShapePtr pMasterShapePtr, ShapePtr pShapePtr )
: ContextHandler( rParent )
, mpMasterShapePtr( pMasterShapePtr )
, mpShapePtr( pShapePtr )
{
}

ShapeContext::~ShapeContext()
{
	if ( mpMasterShapePtr.get() && mpShapePtr.get() )
		mpMasterShapePtr->addChild( mpShapePtr );
}

ShapePtr ShapeContext::getShape()
{
	return mpShapePtr;
}

void ShapeContext::endFastElement( sal_Int32 /* aElementToken */ ) throw( SAXException, RuntimeException )
{
}

Reference< XFastContextHandler > ShapeContext::createFastChildContext( sal_Int32 aElementToken, const Reference< XFastAttributeList >& xAttribs ) throw (SAXException, RuntimeException)
{
	Reference< XFastContextHandler > xRet;

    switch( getBaseToken( aElementToken ) )
	{
	// nvSpPr CT_ShapeNonVisual begin
//	case XML_drElemPr:
//		break;
	case XML_cNvPr:
	{
		AttributeList aAttribs( xAttribs );
		mpShapePtr->setHidden( aAttribs.getBool( XML_hidden, false ) );
		mpShapePtr->setId( xAttribs->getOptionalValue( XML_id ) );
		mpShapePtr->setName( xAttribs->getOptionalValue( XML_name ) );
		break;
	}
	case XML_ph:
		mpShapePtr->setSubType( xAttribs->getOptionalValueToken( XML_type, XML_obj ) );
		mpShapePtr->setSubTypeIndex( xAttribs->getOptionalValue( XML_idx ).toInt32() );
		break;
	// nvSpPr CT_ShapeNonVisual end

	case XML_spPr:
        xRet = new ShapePropertiesContext( *this, *mpShapePtr );
		break;

	case XML_style:
        xRet = new ShapeStyleContext( *this, *mpShapePtr );
		break;

	case XML_txBody:
	{
        TextBodyPtr xTextBody( new TextBody );
        mpShapePtr->setTextBody( xTextBody );
        xRet = new TextBodyContext( *this, *xTextBody );
		break;
	}
	}

	if( !xRet.is() )
    {
        uno::Reference<XFastContextHandler> xTmp(this);
		xRet.set( xTmp );
    }

	return xRet;
}


} }
