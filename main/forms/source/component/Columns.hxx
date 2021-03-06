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



#ifndef _FRM_COLUMNS_HXX
#define _FRM_COLUMNS_HXX

#include "cloneable.hxx"
#include "frm_strings.hxx"

/** === begin UNO includes === **/
#include <com/sun/star/io/XObjectInputStream.hpp>
#include <com/sun/star/io/XObjectOutputStream.hpp>
#include <com/sun/star/lang/XUnoTunnel.hpp>
#include <com/sun/star/util/XCloneable.hpp>
/** === end UNO includes === **/

#include <comphelper/broadcasthelper.hxx>
#include <comphelper/componentcontext.hxx>
#include <comphelper/propagg.hxx>
#include <comphelper/proparrhlp.hxx>
#include <comphelper/uno3.hxx>
#include <cppuhelper/compbase2.hxx>
#include <cppuhelper/component.hxx>

using namespace comphelper;

//.........................................................................
namespace frm
{
//.........................................................................

//==================================================================
// OGridColumn
//==================================================================
typedef ::cppu::WeakAggComponentImplHelper2 <   ::com::sun::star::lang::XUnoTunnel
                                            ,   ::com::sun::star::util::XCloneable > OGridColumn_BASE;
class OGridColumn	:public ::comphelper::OBaseMutex
					,public OGridColumn_BASE
					,public OPropertySetAggregationHelper
					,public OCloneableAggregation
{
protected:
// [properties]
	::com::sun::star::uno::Any	m_aWidth;					// column width
	::com::sun::star::uno::Any	m_aAlign;					// column alignment
	::com::sun::star::uno::Any	m_aHidden;					// column hidden?
// [properties]

    ::comphelper::ComponentContext  m_aContext;
	::rtl::OUString				    m_aModelName;

// [properties]
	::rtl::OUString				m_aLabel;					// Name der Spalte
// [properties]

public:
	OGridColumn(const ::comphelper::ComponentContext& _rContext, const ::rtl::OUString& _sModelName = ::rtl::OUString());
	OGridColumn(const OGridColumn* _pOriginal );
	virtual ~OGridColumn();

	// UNO Anbindung
	DECLARE_UNO3_AGG_DEFAULTS(OGridControlModel, OGridColumn_BASE);
	virtual ::com::sun::star::uno::Any SAL_CALL queryAggregation( const ::com::sun::star::uno::Type& _rType ) throw (::com::sun::star::uno::RuntimeException);

	static const ::com::sun::star::uno::Sequence<sal_Int8>& getUnoTunnelImplementationId();
	// XUnoTunnel
    virtual sal_Int64 SAL_CALL getSomething( const ::com::sun::star::uno::Sequence<sal_Int8>& _rIdentifier) throw(::com::sun::star::uno::RuntimeException);

    // XTypeProvider
	virtual ::com::sun::star::uno::Sequence<sal_Int8>			SAL_CALL getImplementationId() throw(::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type>	SAL_CALL getTypes() throw(::com::sun::star::uno::RuntimeException);

    // OComponentHelper
	virtual void SAL_CALL disposing();

    // XEventListener
	virtual void SAL_CALL disposing(const ::com::sun::star::lang::EventObject& _rSource) throw(::com::sun::star::uno::RuntimeException);

    // XPersistObject
	virtual void SAL_CALL write(const ::com::sun::star::uno::Reference< ::com::sun::star::io::XObjectOutputStream>& _rxOutStream);
	virtual void SAL_CALL read(const ::com::sun::star::uno::Reference< ::com::sun::star::io::XObjectInputStream>& _rxInStream);

    // XPropertySet
	virtual ::cppu::IPropertyArrayHelper& SAL_CALL getInfoHelper() = 0;
	virtual void SAL_CALL getFastPropertyValue(::com::sun::star::uno::Any& rValue, sal_Int32 nHandle ) const;
	virtual sal_Bool SAL_CALL convertFastPropertyValue(::com::sun::star::uno::Any& rConvertedValue, ::com::sun::star::uno::Any& rOldValue,
										  sal_Int32 nHandle, const ::com::sun::star::uno::Any& rValue )
										throw(::com::sun::star::lang::IllegalArgumentException);
	virtual void SAL_CALL setFastPropertyValue_NoBroadcast(sal_Int32 nHandle, const ::com::sun::star::uno::Any& rValue) throw (::com::sun::star::uno::Exception);

    using OPropertySetAggregationHelper::getFastPropertyValue;

    // ::com::sun::star::beans::XPropertyState
	virtual ::com::sun::star::uno::Any getPropertyDefaultByHandle( sal_Int32 nHandle ) const;

    // XCloneable
	virtual ::com::sun::star::uno::Reference< ::com::sun::star::util::XCloneable > SAL_CALL createClone(  ) throw (::com::sun::star::uno::RuntimeException);

    const ::rtl::OUString& getModelName() const { return m_aModelName; }

protected:
	static void clearAggregateProperties(::com::sun::star::uno::Sequence< ::com::sun::star::beans::Property>& seqProps, sal_Bool bAllowDropDown);
	static void setOwnProperties(::com::sun::star::uno::Sequence< ::com::sun::star::beans::Property>& seqProps);

    virtual OGridColumn* createCloneColumn() const = 0;
};

#define DECL_COLUMN(ClassName)										                                        \
class ClassName														                                        \
    :public OGridColumn												                                        \
    ,public OAggregationArrayUsageHelper< ClassName >				                                        \
{																	                                        \
public:																                                        \
    ClassName(const ::comphelper::ComponentContext& _rContext );                                            \
    ClassName(const ClassName* _pCloneFrom);                                                                \
                                                                                                            \
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySetInfo> SAL_CALL getPropertySetInfo() throw(::com::sun::star::uno::RuntimeException);	\
    virtual ::cppu::IPropertyArrayHelper& SAL_CALL getInfoHelper();	                                        \
                                                                                                            \
    virtual void fillProperties(	                                                                        \
        ::com::sun::star::uno::Sequence< ::com::sun::star::beans::Property >& /* [out] */ _rProps,	        \
        ::com::sun::star::uno::Sequence< ::com::sun::star::beans::Property >& /* [out] */ _rAggregateProps	\
        ) const;	                                                                                        \
                                                                                                            \
    virtual OGridColumn* createCloneColumn() const;                                                         \
};


#define IMPL_COLUMN(ClassName, Model, bAllowDropDown)								\
    ClassName::ClassName( const ::comphelper::ComponentContext& _rContext ) \
	:OGridColumn(_rContext, Model) \
{ \
} \
ClassName::ClassName( const ClassName* _pCloneFrom ) \
    :OGridColumn( _pCloneFrom ) \
{ \
} \
::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySetInfo>  ClassName::getPropertySetInfo() throw(::com::sun::star::uno::RuntimeException) \
{ \
	::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySetInfo>  xInfo( createPropertySetInfo( getInfoHelper() ) ); \
	return xInfo; \
} \
::cppu::IPropertyArrayHelper& ClassName::getInfoHelper() \
{ \
	return *const_cast<ClassName*>(this)->getArrayHelper(); \
} \
void ClassName::fillProperties( \
	::com::sun::star::uno::Sequence< ::com::sun::star::beans::Property >& /* [out] */ _rProps, \
	::com::sun::star::uno::Sequence< ::com::sun::star::beans::Property >& /* [out] */ _rAggregateProps \
	) const \
{ \
	if (m_xAggregateSet.is()) \
	{ \
		_rAggregateProps = m_xAggregateSet->getPropertySetInfo()->getProperties(); \
		clearAggregateProperties(_rAggregateProps, bAllowDropDown); \
		setOwnProperties(_rProps); \
	} \
} \
OGridColumn* ClassName::createCloneColumn() const \
{ \
    return new ClassName( this ); \
} \
 \
// column type ids
#define TYPE_CHECKBOX		0
#define TYPE_COMBOBOX		1
#define TYPE_CURRENCYFIELD  2
#define TYPE_DATEFIELD		3
#define TYPE_FORMATTEDFIELD	4
#define TYPE_LISTBOX		5
#define TYPE_NUMERICFIELD	6
#define TYPE_PATTERNFIELD	7
#define TYPE_TEXTFIELD		8
#define TYPE_TIMEFIELD		9

// liste aller bekannten columns
const StringSequence& getColumnTypes();
sal_Int32 getColumnTypeByModelName(const ::rtl::OUString& aModelName);

// Columns
DECL_COLUMN(TextFieldColumn)
DECL_COLUMN(PatternFieldColumn)
DECL_COLUMN(DateFieldColumn)
DECL_COLUMN(TimeFieldColumn)
DECL_COLUMN(NumericFieldColumn)
DECL_COLUMN(CurrencyFieldColumn)
DECL_COLUMN(CheckBoxColumn)
DECL_COLUMN(ComboBoxColumn)
DECL_COLUMN(ListBoxColumn)
DECL_COLUMN(FormattedFieldColumn)

#endif // _FRM_COLUMNS_HXX

//.........................................................................
}	// namespace frm
//.........................................................................

