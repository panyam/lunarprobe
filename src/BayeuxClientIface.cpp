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
 *  \file   BayeuxClientIface.cpp
 *
 *  \brief  Bayeux (comet) Implementation of the Debugger.
 *
 *  \version
 *      - S Panyam   31/03/2009
 *      Initial version.
 */
//*****************************************************************************

#include <iostream> 
#include <string> 
#include <sstream> 
#include <errno.h>
#include <assert.h>
#include <netdb.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/sendfile.h>

#include "lpfwddefs.h"
#include "halley.h"
#include "BayeuxClientIface.h"
#include "DebugContext.h"
#include "LuaBindings.h"

LUNARPROBE_NS_BEGIN

//*****************************************************************************
/*!
 *  \brief  Create a new debugger client.
 *
 *  \version
 *      - S Panyam  31/03/2009
 *      Initial version.
 */
//*****************************************************************************
BayeuxClientIface::BayeuxClientIface(const std::string &name, SBayeuxModule *pModule)
    : SBayeuxChannel(name, pModule)
{
}

//*****************************************************************************
/*!
 *  \brief  Destructor
 *
 *  \version
 *      - S Panyam  31/03/2009
 *      Initial version.
 */
//*****************************************************************************
BayeuxClientIface::~BayeuxClientIface()
{
}

//*****************************************************************************
/*!
 *  \brief  Reads a string from the socket.  
 *
 *  Read a string from the socket.  All strings will be ascii only and
 *  preceeded by the size (also in ascii).  Remember this is simple
 *  protocol for transferring commands and nothing fancy.  Even binary data
 *  can be sent with 64bit encoding without much loss.
 *
 *  Blocks till a complete string is read.
 *
 *  \return 0 if successfull, anything else if no more messages and/or
 *  channel is closed.
 *
 *  \version
 *      - S Panyam  04/11/2008
 *      Initial version.
 */
//*****************************************************************************
void BayeuxClientIface::HandleEvent(const JsonNodePtr &event, JsonNodePtr &output)
{
    // SString message(event->Get<SString>("command", ""));
    JsonNodePtr message = event->Get("command");

    std::string result;
    GetLuaBindings()->HandleMessage(message, result);

    DefaultJsonInputStream<std::string::const_iterator> instream(result.begin(), result.end());
    DefaultJsonBuilder jbuilder;
    output = jbuilder.Build(&instream);
}


//*****************************************************************************
/*!
 *  \brief  Sends a string to the client.  
 *
 *  Writes a string to a socket - string is written as (len/data) pair All
 *  strings will be ascii only and preceeded by the size (also in ascii).
 *  Remember this is simple protocol for transferring commands and nothing
 *  fancy.  Even binary data can be sent with 64bit encoding without much
 *  loss of bandwidth.
 *
 *  \version
 *      - S Panyam  04/11/2008
 *      Initial version.
 */
//*****************************************************************************
int BayeuxClientIface::SendMessage(const char *data, unsigned datasize)
{
    std::cerr << "Here Sending Message: " << pModule << std::endl;
    if (pModule != NULL)
    {
        std::cerr << "Sending Message: " << data << std::endl;
        SMutexLock socketWriteLock(socketWriteMutex);
        JsonNodePtr value = JsonNodeFactory::StringNode(SString(data, datasize));
        pModule->DeliverEvent(this, value);
    }

    return 0;
}

LUNARPROBE_NS_END

