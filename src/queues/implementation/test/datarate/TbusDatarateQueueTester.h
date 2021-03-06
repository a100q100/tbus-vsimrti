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

#ifndef TBUSDATARATEQUEUETESTER_H_
#define TBUSDATARATEQUEUETESTER_H_

#include "TbusTestBase.h"
#include "TbusDatarateQueue.h"
#include "TbusBaseQueue.h"
#include "omnetpp.h"

/**
 * Test class for #TbusDatarateQueue.
 */
class TbusDatarateQueueTester : public TbusTestBase<TbusDatarateQueueTester> {
	private:
		int inGate; ///< Input gate id
		int outGate; ///< Output gate id

		TbusDatarateQueue* queue; ///< Queue to test

	public:
		TbusDatarateQueueTester();
		virtual ~TbusDatarateQueueTester();

		void initialize();

		void testChangingDatarate();
		void handleChangingDatarate(cMessage* msg);

		void testNormalDatarate();
		void handleNormalDatarate(cMessage* msg);

		void testFullDrop();
		void handleFullDrop(cMessage* msg);

		void testHalfDrop();
		void handleHalfDrop(cMessage* msg);
};

#endif /* TBUSDATARATEQUEUETESTER_H_ */
