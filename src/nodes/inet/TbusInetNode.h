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

#ifndef TBUSROUTERNODE_H_
#define TBUSROUTERNODE_H_

#include "ccompoundmodule.h"

/**
 * A TBUS inet node.
 */
class TbusInetNode : public cCompoundModule {
	private:
		int channelInGate; ///< Channel input gate id
		int channelOutGate; ////< Channel output gate id
		int inetInGate; ///< Internet input gate id
		int inetOutGate; ///< Internet output gate id
		int upperInGate; ///< Upper layer input gate id
		int upperOutGate; ///< Upper layer output gate id

	public:
		TbusInetNode();
		virtual ~TbusInetNode();

		void initialize(int stage);
		void handleMessage(cMessage* msg);
};

#endif /* TBUSROUTERNODE_H_ */
