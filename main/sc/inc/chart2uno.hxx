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



#ifndef SC_CHART2UNO_HXX
#define SC_CHART2UNO_HXX

#include "cellsuno.hxx"     // for XModifyListenerArr_Impl / ScLinkListener
#include "rangelst.hxx"
#include "externalrefmgr.hxx"
#include "token.hxx"
#include "chartlis.hxx"

#include <svl/lstner.hxx>
#include <com/sun/star/chart/ChartDataRowSource.hpp>
#include <com/sun/star/chart2/data/XDataProvider.hpp>
#include <com/sun/star/chart2/data/XRangeXMLConversion.hpp>
#include <com/sun/star/chart2/data/XDataSource.hpp>
#include <com/sun/star/chart2/data/XDataSequence.hpp>
#include <com/sun/star/chart2/data/XTextualDataSequence.hpp>
#include <com/sun/star/chart2/data/XNumericalDataSequence.hpp>
#include <com/sun/star/chart2/data/XLabeledDataSequence.hpp>
#include <com/sun/star/chart2/data/DataSequenceRole.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/util/XCloneable.hpp>
#include <com/sun/star/util/XModifyBroadcaster.hpp>
// #ifndef _COM_SUN_STAR_LANG_XUNOTUNNEL_HPP_
// #include <com/sun/star/lang/XUnoTunnel.hpp>
// #endif
#include <cppuhelper/implbase2.hxx>
#include <cppuhelper/implbase4.hxx>
#include <cppuhelper/implbase6.hxx>
#include <cppuhelper/implbase7.hxx>
#include <rtl/ustring.hxx>
#include <svl/itemprop.hxx>

#include <hash_set>
#include <list>
#include <vector>
#include <memory>
#include <boost/shared_ptr.hpp>

#define USE_CHART2_EMPTYDATASEQUENCE 0

class ScDocument;

// DataProvider ==============================================================

class ScChart2DataProvider : public
                ::cppu::WeakImplHelper4<
                    ::com::sun::star::chart2::data::XDataProvider,
                    ::com::sun::star::chart2::data::XRangeXMLConversion,
                    ::com::sun::star::beans::XPropertySet,
                    ::com::sun::star::lang::XServiceInfo>,
                SfxListener
{
public:

    explicit ScChart2DataProvider( ScDocument* pDoc );
    virtual ~ScChart2DataProvider();
    virtual void Notify( SfxBroadcaster& rBC, const SfxHint& rHint );

    // XDataProvider ---------------------------------------------------------

    virtual ::sal_Bool SAL_CALL createDataSourcePossible(
        const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& aArguments )
        throw (::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Reference<
        ::com::sun::star::chart2::data::XDataSource > SAL_CALL createDataSource(
            const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& aArguments )
            throw (::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Sequence<
        ::com::sun::star::beans::PropertyValue > SAL_CALL detectArguments(
            const ::com::sun::star::uno::Reference< ::com::sun::star::chart2::data::XDataSource >& xDataSource )
            throw (::com::sun::star::uno::RuntimeException);

    virtual ::sal_Bool SAL_CALL createDataSequenceByRangeRepresentationPossible(
        const ::rtl::OUString& aRangeRepresentation )
        throw (::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Reference<
        ::com::sun::star::chart2::data::XDataSequence > SAL_CALL createDataSequenceByRangeRepresentation(
            const ::rtl::OUString& aRangeRepresentation )
            throw (::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Reference< ::com::sun::star::sheet::XRangeSelection > SAL_CALL getRangeSelection()
        throw (::com::sun::star::uno::RuntimeException);

/*    virtual ::com::sun::star::uno::Reference< ::com::sun::star::util::XNumberFormatsSupplier > SAL_CALL getNumberFormatsSupplier()
        throw (::com::sun::star::uno::RuntimeException);*/

    // XRangeXMLConversion ---------------------------------------------------

    virtual ::rtl::OUString SAL_CALL convertRangeToXML( const ::rtl::OUString& sRangeRepresentation )
        throw ( ::com::sun::star::uno::RuntimeException, ::com::sun::star::lang::IllegalArgumentException );

    virtual ::rtl::OUString SAL_CALL convertRangeFromXML( const ::rtl::OUString& sXMLRange )
        throw ( ::com::sun::star::uno::RuntimeException, ::com::sun::star::lang::IllegalArgumentException );

    // XPropertySet ----------------------------------------------------------

    virtual ::com::sun::star::uno::Reference<
        ::com::sun::star::beans::XPropertySetInfo> SAL_CALL
        getPropertySetInfo() throw( ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL setPropertyValue(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Any& rValue)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::beans::PropertyVetoException,
                ::com::sun::star::lang::IllegalArgumentException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Any SAL_CALL getPropertyValue(
            const ::rtl::OUString& rPropertyName)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL addPropertyChangeListener(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Reference<
            ::com::sun::star::beans::XPropertyChangeListener>& xListener)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL removePropertyChangeListener(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Reference<
            ::com::sun::star::beans::XPropertyChangeListener>& rListener)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL addVetoableChangeListener(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Reference<
            ::com::sun::star::beans::XVetoableChangeListener>& rListener)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL removeVetoableChangeListener(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Reference<
            ::com::sun::star::beans::XVetoableChangeListener>& rListener)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    // XServiceInfo ----------------------------------------------------------

    virtual ::rtl::OUString SAL_CALL getImplementationName() throw(
            ::com::sun::star::uno::RuntimeException);

    virtual sal_Bool SAL_CALL supportsService( const ::rtl::OUString&
            rServiceName) throw( ::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString> SAL_CALL
        getSupportedServiceNames() throw(
                ::com::sun::star::uno::RuntimeException);

private:

    ScDocument*                 m_pDocument;
    SfxItemPropertySet          m_aPropSet;
    sal_Bool                    m_bIncludeHiddenCells;
};


// DataSource ================================================================

class ScChart2DataSource : public
                ::cppu::WeakImplHelper2<
                    ::com::sun::star::chart2::data::XDataSource,
                    ::com::sun::star::lang::XServiceInfo>,
                SfxListener
{
public:

    explicit ScChart2DataSource( ScDocument* pDoc);
    virtual ~ScChart2DataSource();
    virtual void Notify( SfxBroadcaster& rBC, const SfxHint& rHint );

    // XDataSource -----------------------------------------------------------

    virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Reference<
            ::com::sun::star::chart2::data::XLabeledDataSequence > > SAL_CALL
        getDataSequences() throw (::com::sun::star::uno::RuntimeException);

    // XServiceInfo ----------------------------------------------------------

    virtual ::rtl::OUString SAL_CALL getImplementationName() throw(
            ::com::sun::star::uno::RuntimeException);

    virtual sal_Bool SAL_CALL supportsService( const ::rtl::OUString&
            rServiceName) throw( ::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString> SAL_CALL
        getSupportedServiceNames() throw(
                ::com::sun::star::uno::RuntimeException);

    // implementation

    void AddLabeledSequence(const com::sun::star::uno::Reference < com::sun::star::chart2::data::XLabeledDataSequence >& xNew);

private:

    ScDocument*                 m_pDocument;
    typedef std::list < com::sun::star::uno::Reference< com::sun::star::chart2::data::XLabeledDataSequence > >  LabeledList;
    LabeledList                 m_aLabeledSequences;

};


// DataSequence ==============================================================

class ScChart2DataSequence : public
                ::cppu::WeakImplHelper7<
                    ::com::sun::star::chart2::data::XDataSequence,
                    ::com::sun::star::chart2::data::XTextualDataSequence,
                    ::com::sun::star::chart2::data::XNumericalDataSequence,
                    ::com::sun::star::util::XCloneable,
                    ::com::sun::star::util::XModifyBroadcaster,
                    ::com::sun::star::beans::XPropertySet,
//                     ::com::sun::star::lang::XUnoTunnel,
                    ::com::sun::star::lang::XServiceInfo>,
                SfxListener
{
public:
    explicit ScChart2DataSequence( ScDocument* pDoc,
            const com::sun::star::uno::Reference< com::sun::star::chart2::data::XDataProvider >& xDP,
            ::std::vector<ScSharedTokenRef>* pTokens, bool bIncludeHiddenCells );

    virtual ~ScChart2DataSequence();
    virtual void Notify( SfxBroadcaster& rBC, const SfxHint& rHint );

    // XDataSequence ---------------------------------------------------------

    virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Any >
        SAL_CALL getData() throw (::com::sun::star::uno::RuntimeException);
    virtual ::rtl::OUString SAL_CALL getSourceRangeRepresentation()
        throw (::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString >
        SAL_CALL generateLabel(::com::sun::star::chart2::data::LabelOrigin nOrigin)
        throw (::com::sun::star::uno::RuntimeException);
    virtual ::sal_Int32 SAL_CALL getNumberFormatKeyByIndex( ::sal_Int32 nIndex )
        throw (::com::sun::star::lang::IndexOutOfBoundsException,
               ::com::sun::star::uno::RuntimeException);

    // XNumericalDataSequence --------------------------------------------------

    virtual ::com::sun::star::uno::Sequence< double >
        SAL_CALL getNumericalData(  ) throw (::com::sun::star::uno::RuntimeException);

    // XTextualDataSequence --------------------------------------------------

    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString >
        SAL_CALL getTextualData(  ) throw (::com::sun::star::uno::RuntimeException);

    // XPropertySet ----------------------------------------------------------

    virtual ::com::sun::star::uno::Reference<
        ::com::sun::star::beans::XPropertySetInfo> SAL_CALL
        getPropertySetInfo() throw( ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL setPropertyValue(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Any& rValue)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::beans::PropertyVetoException,
                ::com::sun::star::lang::IllegalArgumentException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Any SAL_CALL getPropertyValue(
            const ::rtl::OUString& rPropertyName)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL addPropertyChangeListener(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Reference<
            ::com::sun::star::beans::XPropertyChangeListener>& xListener)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL removePropertyChangeListener(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Reference<
            ::com::sun::star::beans::XPropertyChangeListener>& rListener)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL addVetoableChangeListener(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Reference<
            ::com::sun::star::beans::XVetoableChangeListener>& rListener)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL removeVetoableChangeListener(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Reference<
            ::com::sun::star::beans::XVetoableChangeListener>& rListener)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    // XCloneable ------------------------------------------------------------

    virtual ::com::sun::star::uno::Reference< ::com::sun::star::util::XCloneable > SAL_CALL createClone()
        throw (::com::sun::star::uno::RuntimeException);

    // XModifyBroadcaster ----------------------------------------------------

    virtual void SAL_CALL addModifyListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::util::XModifyListener >& aListener )
        throw (::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL removeModifyListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::util::XModifyListener >& aListener )
        throw (::com::sun::star::uno::RuntimeException);

    // XServiceInfo ----------------------------------------------------------

    virtual ::rtl::OUString SAL_CALL getImplementationName() throw(
            ::com::sun::star::uno::RuntimeException);

    virtual sal_Bool SAL_CALL supportsService( const ::rtl::OUString&
            rServiceName) throw( ::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString> SAL_CALL
        getSupportedServiceNames() throw(
                ::com::sun::star::uno::RuntimeException);

    // XUnoTunnel ------------------------------------------------------------

// 	virtual sal_Int64 SAL_CALL getSomething( const ::com::sun::star::uno::Sequence<
// 									sal_Int8 >& aIdentifier )
// 								throw(::com::sun::star::uno::RuntimeException);

// 	static const com::sun::star::uno::Sequence<sal_Int8>& getUnoTunnelId();
// 	static ScChart2DataSequence* getImplementation( const com::sun::star::uno::Reference<
// 									com::sun::star::uno::XInterface> xObj );

private:
    void setDataChangedHint(bool b);

    // Implementation --------------------------------------------------------

    void    RefChanged();
    DECL_LINK( ValueListenerHdl, SfxHint* );

private:
    ScChart2DataSequence(); // disabled
    ScChart2DataSequence(const ScChart2DataSequence& r); // disabled

    class ExternalRefListener : public ScExternalRefManager::LinkListener
    {
    public:
        ExternalRefListener(ScChart2DataSequence& rParent, ScDocument* pDoc);
        virtual ~ExternalRefListener(); 
        virtual void notify(sal_uInt16 nFileId, ScExternalRefManager::LinkUpdateType eType);
        void addFileId(sal_uInt16 nFileId);
        void removeFileId(sal_uInt16 nFileId);
        const ::std::hash_set<sal_uInt16>& getAllFileIds();

    private:
        ExternalRefListener();
        ExternalRefListener(const ExternalRefListener& r);

        ScChart2DataSequence&       mrParent;
        ::std::hash_set<sal_uInt16> maFileIds;
        ScDocument*                 mpDoc;
    };

    /** 
     * Build an internal data array to cache the data ranges, and other 
     * information such as hidden values. 
     */
    void BuildDataCache();

    void RebuildDataCache();

    sal_Int32 FillCacheFromExternalRef(const ScSharedTokenRef& pToken);

    void UpdateTokensFromRanges(const ScRangeList& rRanges);

    ExternalRefListener* GetExtRefListener();

    void StopListeningToAllExternalRefs();

    void CopyData(const ScChart2DataSequence& r);

private:

    // data array
    struct Item
    {
        double              mfValue;
        ::rtl::OUString     maString;
        bool                mbIsValue;
        Item();
    };

    class HiddenRangeListener : public ScChartHiddenRangeListener
    {
    public:
        HiddenRangeListener(ScChart2DataSequence& rParent);
        virtual ~HiddenRangeListener();

        virtual void notify();

    private:
        ScChart2DataSequence& mrParent;
    };

    ::std::list<Item>           m_aDataArray;

    /** 
     * Cached data for getData.  We may also need to cache data for the 
     * numerical and textural data series if they turn out to be bottlenecks 
     * under certain scenarios.
     */ 
    ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Any > m_aMixedDataCache;

    ::com::sun::star::uno::Sequence<sal_Int32>  m_aHiddenValues;

    // properties
    ::com::sun::star::chart2::data::DataSequenceRole  m_aRole;
    sal_Bool                    m_bIncludeHiddenCells;

    // internals
    typedef ::std::auto_ptr< ::std::vector<ScSharedTokenRef> >  TokenListPtr;
    typedef ::std::auto_ptr< ::std::vector<sal_uInt32> >        RangeIndexMapPtr;
    typedef ::std::auto_ptr<ExternalRefListener>                ExtRefListenerPtr;

    sal_Int64                   m_nObjectId;
    ScDocument*                 m_pDocument;
    TokenListPtr                m_pTokens;
    RangeIndexMapPtr            m_pRangeIndices;
    ExtRefListenerPtr           m_pExtRefListener;
    com::sun::star::uno::Reference < com::sun::star::chart2::data::XDataProvider > m_xDataProvider;
	SfxItemPropertySet		    m_aPropSet;

    ::std::auto_ptr<HiddenRangeListener> m_pHiddenListener;
    ScLinkListener*             m_pValueListener;
    XModifyListenerArr_Impl     m_aValueListeners;

    bool                        m_bGotDataChangedHint;
    bool                        m_bExtDataRebuildQueued;
};

#if USE_CHART2_EMPTYDATASEQUENCE
// DataSequence ==============================================================

class ScChart2EmptyDataSequence : public
                ::cppu::WeakImplHelper6<
                    ::com::sun::star::chart2::data::XDataSequence,
                    ::com::sun::star::chart2::data::XTextualDataSequence,
                    ::com::sun::star::util::XCloneable,
                    ::com::sun::star::util::XModifyBroadcaster,
                    ::com::sun::star::beans::XPropertySet,
//                     ::com::sun::star::lang::XUnoTunnel,
                    ::com::sun::star::lang::XServiceInfo>,
                SfxListener
{
public:

    explicit ScChart2EmptyDataSequence( ScDocument* pDoc,
            const com::sun::star::uno::Reference< com::sun::star::chart2::data::XDataProvider >& xDP,
            const ScRangeListRef& rRangeList, sal_Bool bColumn );
    virtual ~ScChart2EmptyDataSequence();
    virtual void Notify( SfxBroadcaster& rBC, const SfxHint& rHint );

    // XDataSequence ---------------------------------------------------------

    virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Any >
        SAL_CALL getData() throw (::com::sun::star::uno::RuntimeException);
    virtual ::rtl::OUString SAL_CALL getSourceRangeRepresentation()
        throw (::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString >
        SAL_CALL generateLabel(::com::sun::star::chart2::data::LabelOrigin nOrigin)
        throw (::com::sun::star::uno::RuntimeException);
    virtual ::sal_Int32 SAL_CALL getNumberFormatKeyByIndex( ::sal_Int32 nIndex )
        throw (::com::sun::star::lang::IndexOutOfBoundsException,
               ::com::sun::star::uno::RuntimeException);

    // XTextualDataSequence --------------------------------------------------

    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString >
        SAL_CALL getTextualData(  ) throw (::com::sun::star::uno::RuntimeException);

    // XPropertySet ----------------------------------------------------------

    virtual ::com::sun::star::uno::Reference<
        ::com::sun::star::beans::XPropertySetInfo> SAL_CALL
        getPropertySetInfo() throw( ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL setPropertyValue(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Any& rValue)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::beans::PropertyVetoException,
                ::com::sun::star::lang::IllegalArgumentException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Any SAL_CALL getPropertyValue(
            const ::rtl::OUString& rPropertyName)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL addPropertyChangeListener(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Reference<
            ::com::sun::star::beans::XPropertyChangeListener>& xListener)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL removePropertyChangeListener(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Reference<
            ::com::sun::star::beans::XPropertyChangeListener>& rListener)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL addVetoableChangeListener(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Reference<
            ::com::sun::star::beans::XVetoableChangeListener>& rListener)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL removeVetoableChangeListener(
            const ::rtl::OUString& rPropertyName,
            const ::com::sun::star::uno::Reference<
            ::com::sun::star::beans::XVetoableChangeListener>& rListener)
        throw( ::com::sun::star::beans::UnknownPropertyException,
                ::com::sun::star::lang::WrappedTargetException,
                ::com::sun::star::uno::RuntimeException);

    // XCloneable ------------------------------------------------------------

    virtual ::com::sun::star::uno::Reference< ::com::sun::star::util::XCloneable > SAL_CALL createClone()
        throw (::com::sun::star::uno::RuntimeException);

    // XModifyBroadcaster ----------------------------------------------------

    virtual void SAL_CALL addModifyListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::util::XModifyListener >& aListener )
        throw (::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL removeModifyListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::util::XModifyListener >& aListener )
        throw (::com::sun::star::uno::RuntimeException);

    // XServiceInfo ----------------------------------------------------------

    virtual ::rtl::OUString SAL_CALL getImplementationName() throw(
            ::com::sun::star::uno::RuntimeException);

    virtual sal_Bool SAL_CALL supportsService( const ::rtl::OUString&
            rServiceName) throw( ::com::sun::star::uno::RuntimeException);

    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString> SAL_CALL
        getSupportedServiceNames() throw(
                ::com::sun::star::uno::RuntimeException);

    // XUnoTunnel ------------------------------------------------------------

// 	virtual sal_Int64 SAL_CALL getSomething( const ::com::sun::star::uno::Sequence<
// 									sal_Int8 >& aIdentifier )
// 								throw(::com::sun::star::uno::RuntimeException);

// 	static const com::sun::star::uno::Sequence<sal_Int8>& getUnoTunnelId();
// 	static ScChart2DataSequence* getImplementation( const com::sun::star::uno::Reference<
// 									com::sun::star::uno::XInterface> xObj );

    // Implementation --------------------------------------------------------

    ScRangeListRef GetRangeList() { return m_xRanges; }

private:

    // properties
    ::com::sun::star::chart2::data::DataSequenceRole  m_aRole;
    sal_Bool                    m_bIncludeHiddenCells;
    // internals
    ScRangeListRef              m_xRanges;
    ScDocument*                 m_pDocument;
    com::sun::star::uno::Reference < com::sun::star::chart2::data::XDataProvider > m_xDataProvider;
	SfxItemPropertySet		    m_aPropSet;
    sal_Bool                    m_bColumn; // defines the orientation to create the right labels

};
#endif

#endif // SC_CHART2UNO_HXX
