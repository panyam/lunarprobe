/*****************************************************************************/
/*!
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *****************************************************************************
 *
 *  \file   DebugContext.cpp
 *
 *  \brief  Implementation of DebugContext
 *
 *  \version
 *      - S Panyam   05/12/2008
 *      Initial version.
 */
//*****************************************************************************

#include "DebugServer.h"

LUNARPROBE_NS_BEGIN

//*****************************************************************************
/*!
 *  \brief  Create a TCP based debug server.
 *
 *  \version
 *      - S Panyam  01/04/2009
 *      Initial version.
 */
//*****************************************************************************
TcpDebugServer::TcpDebugServer(int port, TcpClientIface *pIface) :
    SServer(port),
    pClientIface(pIface)
{
    SetConnectionFactory(this);
}

//*****************************************************************************
/*!
 *  \brief  No-op destructor
 *
 *  \version
 *      - S Panyam  01/04/2009
 *      Initial version.
 */
//*****************************************************************************
TcpDebugServer::~TcpDebugServer()
{
}

//*****************************************************************************
/*!
 *  \brief  Creates a new client handler.
 *
 *  \version
 *      - S Panyam  01/04/2009
 *      Initial version.
 */
//*****************************************************************************
SConnHandler *TcpDebugServer::NewHandler()
{
    return pClientIface;
}

//*****************************************************************************
/*!
 *  \brief  Releases the client handler (does nothing here).
 *
 *  \version
 *      - S Panyam  01/04/2009
 *      Initial version.
 */
//*****************************************************************************
void TcpDebugServer::ReleaseHandler(SConnHandler *)
{
}

//*****************************************************************************
/*!
 *  \brief  Create a HTTP based debug server.
 *
 *  \version
 *      - S Panyam  01/04/2009
 *      Initial version.
 */
//*****************************************************************************
HttpDebugServer::HttpDebugServer(int                    port,
                                 BayeuxClientIface *    pIface,
                                 const std::string &    msgBoundary) :
    SEvServer(port),
    pClientIface(pIface),
    contentModule(&writerModule),
    bayeuxModule(&contentModule, msgBoundary),
    fileModule(&contentModule, true),
    urlRouter(NULL)
{
    SetRequestReader(&requestReader);
    SetStage("RequestReader", &requestReader);
    SetStage("RequestHandler", &requestHandler);

    requestReader.SetHandlerStage(&requestHandler);
    requestHandler.SetRootModule(&urlRouter);

    assert("BayeuxClientIface is NULL." && (pIface != NULL));

    pClientIface->SetBayeuxModule(&bayeuxModule);
    bayeuxModule.RegisterChannel(pClientIface);

    // add more files and all that here to enable static html/js/css files
}

//*****************************************************************************
/*!
 *  \brief  No-op destructor
 *
 *  \version
 *      - S Panyam  01/04/2009
 *      Initial version.
 */
//*****************************************************************************
HttpDebugServer::~HttpDebugServer()
{
}

LUNARPROBE_NS_END
