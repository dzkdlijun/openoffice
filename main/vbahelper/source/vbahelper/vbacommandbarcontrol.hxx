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


#ifndef SC_VBA_COMMANDBARCONTROL_HXX
#define SC_VBA_COMMANDBARCONTROL_HXX

#include <ooo/vba/XCommandBarControl.hpp>
#include <ooo/vba/XCommandBarPopup.hpp>
#include <ooo/vba/XCommandBarButton.hpp>
#include <ooo/vba/office/MsoControlType.hpp>
#include <com/sun/star/awt/XMenu.hpp>
#include <vbahelper/vbahelperinterface.hxx>
#include "vbacommandbarhelper.hxx"
#include <cppuhelper/implbase1.hxx>

typedef InheritedHelperInterfaceImpl1< ov::XCommandBarControl > CommandBarControl_BASE;

class ScVbaCommandBarControl : public CommandBarControl_BASE
{
protected:
    VbaCommandBarHelperRef pCBarHelper;
    rtl::OUString       m_sResourceUrl;
    css::uno::Reference< css::container::XIndexAccess > m_xCurrentSettings;
    css::uno::Reference< css::container::XIndexAccess > m_xBarSettings;
    css::uno::Sequence< css::beans::PropertyValue >     m_aPropertyValues;

    sal_Int32           m_nPosition;
    sal_Bool            m_bTemporary;

private:
    void ApplyChange() throw (css::uno::RuntimeException);

public:
    ScVbaCommandBarControl( const css::uno::Reference< ov::XHelperInterface >& xParent, const css::uno::Reference< css::uno::XComponentContext >& xContext, const css::uno::Reference< css::container::XIndexAccess >& xSettings, VbaCommandBarHelperRef pHelper, const css::uno::Reference< css::container::XIndexAccess >& xBarSettings, const rtl::OUString& sResourceUrl ) throw (css::uno::RuntimeException);
    ScVbaCommandBarControl( const css::uno::Reference< ov::XHelperInterface >& xParent, const css::uno::Reference< css::uno::XComponentContext >& xContext, const css::uno::Reference< css::container::XIndexAccess >& xSettings, VbaCommandBarHelperRef pHelper, const css::uno::Reference< css::container::XIndexAccess >& xBarSettings, const rtl::OUString& sResourceUrl, sal_Int32 nPosition, sal_Bool bTemporary ) throw (css::uno::RuntimeException);
    
    // Attributes
    virtual ::rtl::OUString SAL_CALL getCaption() throw (css::uno::RuntimeException);
    virtual void SAL_CALL setCaption( const ::rtl::OUString& _caption ) throw (css::uno::RuntimeException);
    virtual ::rtl::OUString SAL_CALL getOnAction() throw (css::uno::RuntimeException);
    virtual void SAL_CALL setOnAction( const ::rtl::OUString& _onaction ) throw (css::uno::RuntimeException);
    virtual ::sal_Bool SAL_CALL getVisible() throw (css::uno::RuntimeException);
    virtual void SAL_CALL setVisible( ::sal_Bool _visible ) throw (css::uno::RuntimeException);
    virtual ::sal_Bool SAL_CALL getEnabled() throw (css::uno::RuntimeException);
    virtual void SAL_CALL setEnabled( ::sal_Bool _enabled ) throw (css::uno::RuntimeException);
    virtual ::sal_Bool SAL_CALL getBeginGroup() throw (css::uno::RuntimeException);
    virtual void SAL_CALL setBeginGroup( ::sal_Bool _begin ) throw (css::uno::RuntimeException);
    virtual sal_Int32 SAL_CALL getType() throw (css::uno::RuntimeException)
    {
        return ov::office::MsoControlType::msoControlButton;
    }

    // Methods
    virtual void SAL_CALL Delete(  ) throw (css::script::BasicErrorException, css::uno::RuntimeException);
    virtual css::uno::Any SAL_CALL Controls( const css::uno::Any& aIndex ) throw (css::script::BasicErrorException, css::uno::RuntimeException);

	// XHelperInterface
	virtual rtl::OUString& getServiceImplName();
	virtual css::uno::Sequence<rtl::OUString> getServiceNames();
};

typedef cppu::ImplInheritanceHelper1< ScVbaCommandBarControl, ov::XCommandBarPopup > CommandBarPopup_BASE;
class ScVbaCommandBarPopup : public CommandBarPopup_BASE
{
public:
    ScVbaCommandBarPopup( const css::uno::Reference< ov::XHelperInterface >& xParent, const css::uno::Reference< css::uno::XComponentContext >& xContext, const css::uno::Reference< css::container::XIndexAccess >& xSettings, VbaCommandBarHelperRef pHelper, const css::uno::Reference< css::container::XIndexAccess >& xBarSettings, const rtl::OUString& sResourceUrl, sal_Int32 nPosition, sal_Bool bTemporary ) throw (css::uno::RuntimeException);

    virtual sal_Int32 SAL_CALL getType() throw (css::uno::RuntimeException)
    {
        return ov::office::MsoControlType::msoControlPopup;
    }
	// XHelperInterface
	virtual rtl::OUString& getServiceImplName();
	virtual css::uno::Sequence<rtl::OUString> getServiceNames();
};

typedef cppu::ImplInheritanceHelper1< ScVbaCommandBarControl, ov::XCommandBarButton > CommandBarButton_BASE;
class ScVbaCommandBarButton : public CommandBarButton_BASE
{
public:
    ScVbaCommandBarButton( const css::uno::Reference< ov::XHelperInterface >& xParent, const css::uno::Reference< css::uno::XComponentContext >& xContext, const css::uno::Reference< css::container::XIndexAccess >& xSettings, VbaCommandBarHelperRef pHelper, const css::uno::Reference< css::container::XIndexAccess >& xBarSettings, const rtl::OUString& sResourceUrl, sal_Int32 nPosition, sal_Bool bTemporary ) throw (css::uno::RuntimeException);

    virtual sal_Int32 SAL_CALL getType() throw (css::uno::RuntimeException)
    {
        return ov::office::MsoControlType::msoControlButton;
    }
	// XHelperInterface
	virtual rtl::OUString& getServiceImplName();
	virtual css::uno::Sequence<rtl::OUString> getServiceNames();
};

#endif//SC_VBA_COMMANDBARCONTROL_HXX
