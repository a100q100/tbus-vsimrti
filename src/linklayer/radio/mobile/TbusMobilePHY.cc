//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
//
#include "TbusChannelControl.h"
#include "ChannelControl.h"
#include "TbusMobilePHY.h"
#include "ModuleAccess.h"
#include "TbusQueueControl.h"
#include "TbusQueueControlInfo.h"

Define_Module(TbusMobilePHY);

/**
 * Constructor.
 * Invalidate host reference.
 */
TbusMobilePHY::TbusMobilePHY() {
	myHostRef = NULL;
}

/**
 * Empty destructor.
 */
TbusMobilePHY::~TbusMobilePHY() {}

/**
 * Simulation initialization.
 * - Stage 0: Gate id assignment, subscribe to position updates, get references
 * - Stage 3: Register IP
 * @param stage Init stage level
 */
void TbusMobilePHY::initialize(int stage) {
	ChannelAccessExtended::initialize(stage);

	if (stage == 0) {
		upperLayerIn 	= findGate("upperLayerIn");
		upperLayerOut 	= findGate("upperLayerOut");
		radioIn 		= findGate("radioIn");

		tbusCC = check_and_cast<TbusChannelControl*>(ChannelControlExtended::get());
	} else if (stage == 3) {
		// Register ip address at channel control
		tbusCC->registerIP(myHostRef);
	}
}

/**
 * Handle message from upper layer.
 * Removes control info and forwards message.
 * @param msg Message to handle
 */
void TbusMobilePHY::handleUpperMessage(cMessage* msg) {
	// Remove control info
	TbusQueueControlInfo* controlInfo = check_and_cast<TbusQueueControlInfo*>(msg->removeControlInfo());
	delete controlInfo;

	sendToChannel(msg);
}

/**
 * Handle message from lower layer.
 * Adds control info and forwards message.
 * Only works when queue control is online.
 * @param msg Message to handle
 */
void TbusMobilePHY::handleLowerMessage(cMessage* msg) {
	msg->setControlInfo(new TbusQueueControlInfo());
	send(msg, upperLayerOut);
}

/**
 * Handle incoming message.
 * @param msg Message to handle
 */
void TbusMobilePHY::handleMessage(cMessage* msg) {
	EV << "TbusRadioPHY received message on " << msg->getArrivalGate()->getName() << endl;
	if (msg->isSelfMessage()) {
		// Self message
		delete msg;
	} else if (msg->arrivedOn(upperLayerIn)) {
		// Message from upper layer
		handleUpperMessage(msg);
	} else {
		// Message from lower layer
		handleLowerMessage(msg);
	}
}

/**
 * Send message to channel via channel control.
 * @param msg Message to send
 */
void TbusMobilePHY::sendToChannel(cMessage* msg) {
	tbusCC->sendToChannel(msg, myHostRef);
}
