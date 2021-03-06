//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
#include "TbusDelayQueueTester.h"
#include "TbusQueueDelayValue.h"
#include "ModuleAccess.h"
#include "testPacket_m.h"

#include <sstream>

Define_Module(TbusDelayQueueTester);

/**
 * Initialize base instance with this instance and add tests.
 */
TbusDelayQueueTester::TbusDelayQueueTester() : TbusTestBase(this) {
	addTest(&TbusDelayQueueTester::testConstantDelay, &TbusDelayQueueTester::handleConstantDelay, "Constant Delay Test");
	addTest(&TbusDelayQueueTester::testNoDelay, &TbusDelayQueueTester::handleNoDelay, "No Delay Test");
	addTest(&TbusDelayQueueTester::testChangingDelay, &TbusDelayQueueTester::handleChangingDelay, "Changing Delay Test");

}

/**
 * Empty destructor.
 */
TbusDelayQueueTester::~TbusDelayQueueTester() {}

/**
 * Simulation initialization.
 * Get gate ids and init queue. Then start first test.
 */
void TbusDelayQueueTester::initialize() {
	inGate = findGate("inGate");
	outGate = findGate("outGate");

	queue = ModuleAccess<TbusDelayQueue>("delayQueue").get();

	runNextTest();
}

/**
 * Test a constant delay.
 */
void TbusDelayQueueTester::testConstantDelay() {
	// Reset values
	queue->updateValue(NULL);
	queue->updateValue(NULL);

	TbusQueueDelayValue* delay = new TbusQueueDelayValue();
	delay->delay = 1.0; // 1sec delay
	queue->updateValue(delay);

	for (uint32_t i = 0; i < 10; ++i) {
		std::ostringstream outStream;
		outStream << "ConstantDelayTestPacket" << i;

		TestPacket* packet = new TestPacket(outStream.str().c_str(), 0);
		packet->setExpectedArrival(simTime() + delay->delay);
		send(packet, outGate);
	}

	#define CONSTANT_DELAY_END "constant.delay.end"
	scheduleAt(simTime() + 2.0, new cMessage(CONSTANT_DELAY_END, 0));
}

/**
 * Message handler for constant delay test.
 * @param msg Message to handle
 */
void TbusDelayQueueTester::handleConstantDelay(cMessage* msg) {
	static uint32_t arrivals = 0;

	if (msg->isSelfMessage() && strcmp(msg->getName(), CONSTANT_DELAY_END) == 0) {
		if (arrivals != 10) {
			success = false;
		}
		scheduleAt(simTime(), &nextTestMessage);
	} else {
		arrivals++;

		success = success && (abs(simTime().dbl() - ((TestPacket*) msg)->getExpectedArrival().dbl()) < 0.000001);
	}

	delete msg;
}

/**
 * Test for channel without delay (instant delivery).
 */
void TbusDelayQueueTester::testNoDelay() {
	// Reset values
	queue->updateValue(NULL);
	queue->updateValue(NULL);

	TbusQueueDelayValue* delay = new TbusQueueDelayValue();
	delay->delay = 0.0; // 0sec delay
	queue->updateValue(delay);

	for (uint32_t i = 0; i < 10; ++i) {
		std::ostringstream outStream;
		outStream << "NoDelayTestPacket" << i;

		TestPacket* packet = new TestPacket(outStream.str().c_str(), 0);
		packet->setExpectedArrival(simTime() + delay->delay);
		send(packet, outGate);
	}

	#define NO_DELAY_END "no.delay.end"
	scheduleAt(simTime() + 1.0, new cMessage(NO_DELAY_END, 0));
}

/**
 * Message handler for test without delay.
 * @param msg Message to handle
 */
void TbusDelayQueueTester::handleNoDelay(cMessage* msg) {
	static uint32_t arrivals = 0;

	if (msg->isSelfMessage() && strcmp(msg->getName(), NO_DELAY_END) == 0) {
		if (arrivals != 10) {
			success = false;
		}
		scheduleAt(simTime(), &nextTestMessage);
	} else {
		arrivals++;

		success = success && (abs(simTime().dbl() - ((TestPacket*) msg)->getExpectedArrival().dbl()) < 0.000001);
	}

	delete msg;
}

/**
 * Test a channel with changing delay throughout packet transmission.
 */
void TbusDelayQueueTester::testChangingDelay() {
	// Reset values
	queue->updateValue(NULL);
	queue->updateValue(NULL);

	TbusQueueDelayValue* delay1 = new TbusQueueDelayValue();
	delay1->delay = 2.0;
	queue->updateValue(delay1); // Delay is going to be halved at offset .5

	TestPacket* packet1 = new TestPacket("ChangingDelayTestPacket1", 0);
	packet1->setExpectedArrival(simTime() + 1.0);
	send(packet1, outGate);

	TestPacket* packet2 = new TestPacket("ChangingDelayTestPacket2", 0);
	packet2->setExpectedArrival(simTime() + 1.6);
	sendDelayed(packet2, .6, outGate);

	#define CHANGE_DELAY "change.delay"
	scheduleAt(simTime() + .5, new cMessage(CHANGE_DELAY));
}

/**
 * Message handler for changing delay test.
 * @param msg Message to handle
 */
void TbusDelayQueueTester::handleChangingDelay(cMessage* msg) {
	if (msg->isSelfMessage() && (strcmp(msg->getName(), CHANGE_DELAY) == 0)) {
		TbusQueueDelayValue* delay2 = new TbusQueueDelayValue();
		delay2->delay = 1.0;

		queue->updateValue(delay2);
	} else {
		success = success && (abs(simTime().dbl() - ((TestPacket*)msg)->getExpectedArrival().dbl()) < 0.000001);

		EV << simTime() << ": Packet " << msg->getName() << " had expected arrival time " << ((TestPacket*)msg)->getExpectedArrival() << std::endl;

		if (strcmp(msg->getName(), "ChangingDelayTestPacket2") == 0) {
			scheduleAt(simTime(), &nextTestMessage);
		}
	}
}
