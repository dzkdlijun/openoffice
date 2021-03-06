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



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_framework.hxx"

//_________________________________________________________________________________________________________________
//	my own includes
//_________________________________________________________________________________________________________________


#include <uielement/addonstoolbarwrapper.hxx>
#include <threadhelp/resetableguard.hxx>
#include <framework/actiontriggerhelper.hxx>
#include <uielement/constitemcontainer.hxx>
#include <uielement/rootitemcontainer.hxx>
#include <uielement/addonstoolbarmanager.hxx>

#ifndef __FRAMEWORK_UIELEMENT_TOOLBARW_HXX_
#include <uielement/toolbar.hxx>
#endif

//_________________________________________________________________________________________________________________
//	interface includes
//_________________________________________________________________________________________________________________
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/lang/DisposedException.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/awt/XSystemDependentMenuPeer.hpp>
#include <com/sun/star/awt/XMenuBar.hpp>
#include <com/sun/star/container/XIndexContainer.hpp>
#include <com/sun/star/container/XNameAccess.hpp>
#include <com/sun/star/ui/UIElementType.hpp>

//_________________________________________________________________________________________________________________
//	other includes
//_________________________________________________________________________________________________________________

#ifndef _TOOLKIT_HELPER_VCLUNOHELPER_HXX_
#include <toolkit/helper/vclunohelper.hxx>
#endif
#include <toolkit/awt/vclxwindow.hxx>
#include <comphelper/processfactory.hxx>

#include <svtools/miscopt.hxx>
#include <vcl/svapp.hxx>
#include <vcl/toolbox.hxx>
#include <rtl/logfile.hxx>

using namespace com::sun::star::uno;
using namespace com::sun::star::beans;
using namespace com::sun::star::frame;
using namespace com::sun::star::lang;
using namespace com::sun::star::container;
using namespace com::sun::star::awt;
using namespace ::com::sun::star::ui;

namespace framework
{

AddonsToolBarWrapper::AddonsToolBarWrapper( const Reference< XMultiServiceFactory >& xServiceManager ) :
    UIElementWrapperBase( UIElementType::TOOLBAR ),
    m_xServiceManager( xServiceManager )
{
}

AddonsToolBarWrapper::~AddonsToolBarWrapper()
{
}

// XComponent
void SAL_CALL AddonsToolBarWrapper::dispose() throw ( RuntimeException )
{
    Reference< XComponent > xThis( static_cast< OWeakObject* >(this), UNO_QUERY );

    com::sun::star::lang::EventObject aEvent( xThis );
    m_aListenerContainer.disposeAndClear( aEvent );

    ResetableGuard aLock( m_aLock );

    if ( m_xToolBarManager.is() )
        m_xToolBarManager->dispose();
    m_xToolBarManager.clear();
    m_xToolBarWindow.clear();

    m_bDisposed = sal_True;
}

// XInitialization
void SAL_CALL AddonsToolBarWrapper::initialize( const Sequence< Any >& aArguments ) throw ( Exception, RuntimeException )
{
    ResetableGuard aLock( m_aLock );

    if ( m_bDisposed )
        throw DisposedException();

    if ( !m_bInitialized )
    {
        UIElementWrapperBase::initialize( aArguments );

        for ( sal_Int32 n = 0; n < aArguments.getLength(); n++ )
        {
            PropertyValue aPropValue;
            if ( aArguments[n] >>= aPropValue )
            {
                if ( aPropValue.Name.equalsAscii( "ConfigurationData" ))
                    aPropValue.Value >>= m_aConfigData;
            }
        }

        Reference< XFrame > xFrame( m_xWeakFrame );
        if ( xFrame.is() && m_aConfigData.getLength() > 0 )
        {
            // Create VCL based toolbar which will be filled with settings data
            ToolBar* pToolBar = 0;
            AddonsToolBarManager* pToolBarManager = 0;
            {
                vos::OGuard aSolarMutexGuard( Application::GetSolarMutex() );
                Window* pWindow = VCLUnoHelper::GetWindow( xFrame->getContainerWindow() );
                if ( pWindow )
                {
                    sal_uLong nStyles = WB_LINESPACING | WB_BORDER | WB_SCROLL | WB_MOVEABLE | WB_3DLOOK | WB_DOCKABLE | WB_SIZEABLE | WB_CLOSEABLE;

                    pToolBar = new ToolBar( pWindow, nStyles );
                    m_xToolBarWindow = VCLUnoHelper::GetInterface( pToolBar );
                    pToolBarManager = new AddonsToolBarManager( m_xServiceManager, xFrame, m_aResourceURL, pToolBar );
                    pToolBar->SetToolBarManager( pToolBarManager );
                    m_xToolBarManager = Reference< XComponent >( static_cast< OWeakObject *>( pToolBarManager ), UNO_QUERY );
                }
            }

            try
            {
                if (( m_aConfigData.getLength() > 0 ) && pToolBar && pToolBarManager )
                {
                    // Fill toolbar with container contents
                    pToolBarManager->FillToolbar( m_aConfigData );
                    pToolBar->SetOutStyle( SvtMiscOptions().GetToolboxStyle() );
                    pToolBar->EnableCustomize( sal_True );
                    ::Size aActSize( pToolBar->GetSizePixel() );
                    ::Size aSize( pToolBar->CalcWindowSizePixel() );
                    aSize.Width() = aActSize.Width();
			        pToolBar->SetSizePixel( aSize );
                }
            }
            catch ( NoSuchElementException& )
            {
            }
        }
    }
}

// XUIElement interface
Reference< XInterface > SAL_CALL AddonsToolBarWrapper::getRealInterface() throw (::com::sun::star::uno::RuntimeException)
{
    ResetableGuard aLock( m_aLock );

    if ( m_xToolBarManager.is() )
    {
        AddonsToolBarManager* pToolBarManager = static_cast< AddonsToolBarManager *>( m_xToolBarManager.get() );
        if ( pToolBarManager )
        {
            Window* pWindow = (Window *)pToolBarManager->GetToolBar();
            return Reference< XInterface >( VCLUnoHelper::GetInterface( pWindow ), UNO_QUERY );
        }
    }

    return Reference< XInterface >();
}

} // namespace framework

