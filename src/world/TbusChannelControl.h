//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef TBUSCHANNELCONTROL_H_
#define TBUSCHANNELCONTROL_H_

#include "ChannelControlExtended.h"
#include "ChannelAccess.h"
#include "IPAddress.h"

//#define TBUS_DEBUG

#ifdef TBUS_DEBUG
#include <sqlite3.h>
#endif /* TBUS_DEBUG */

class TbusMobilePHY;

/**
 * Provides a Control layer for NICs attached to the air interface
 */
class TbusChannelControl : public ChannelControlExtended {
private:
	int routerInGate; ///< Router input gate
	int routerOutGate; ///< Router output gate

	typedef std::map<IPAddress, ChannelControl::HostRef> ip2hostMap; ///< Typedef of the IP to host hashmap
	ip2hostMap hostMap; ///< Map for mapping IP address to a host

	typedef std::pair<IPAddress, ChannelControl::HostRef> ip2host; ///< Tuple of IP address and host
public:
    TbusChannelControl();
    virtual ~TbusChannelControl();

    virtual void initialize();
    virtual void handleMessage(cMessage* msg);

    void registerIP(ChannelControl::HostRef hostRef);

    void sendToChannel(cMessage* msg, HostRef h);
};

#endif /* TBUSCHANNELCONTROL_H_ */
