#define ENABLE_DEBUGPRINT 1

#include <stdio.h>
#include <string>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "protobuf.net.hpp"
#include "comm.pb.h"

using std::string;
using comm::Data;
using protobuf::Message;
using protobuf::Serializable;
using protobuf::DataBuilder;
using protobuf::Recievable;
using protobuf::ProtocolBuffersServer;


inline void print_msg (const comm::Message& msg) {
	fprintf(stderr, "%d\n", msg.id());
	fprintf(stderr, "data size: %d\n", msg.data_size());
	for (int i = 0; i < msg.data_size(); i++) {
		const Data& _data = msg.data(i);
		fprintf(stderr,
			"\tstatus: %s\n", 
			_data.status().c_str());
	}	
}

class Reciever : public Recievable {
public:
	bool onRecv (Serializable* data) {
		DEBUG_PRINTLN("on recv");
		Message<comm::Message>* impl = (Message<comm::Message>*)data;
		print_msg (impl->get());
		return true;

	}
};

int main(int argc, char* argv[])
{
	Message<comm::Message> msg_impl;
	boost::system::error_code error;
	boost::shared_ptr<ProtocolBuffersServer> server_ptr = ProtocolBuffersServer::create(1111, msg_impl, error);
	if (error) {
		ERROR_PRINTLN("failed to activate server.");
		return -1;
	}
	//boost::shared_ptr<ProtocolBuffersServer> server_ptr((ProtocolBuffersServer*)NULL);
	ProtocolBuffersServer& server(*server_ptr.get());	
	Reciever recv_impl;
	server.register_reciever(&recv_impl);
	boost::thread (boost::bind(&ProtocolBuffersServer::start, &server));

	comm::Message msg;
	msg.set_id(10);
	msg.set_type(comm::Message::ACK);
	msg.add_data()->set_status("hogehoge");

	Message<comm::Message> impl(msg);
	print_msg (impl.get());
	DEBUG_PRINTLN("serialized size: %d", impl.getSerializedSize());

	std::cout << "waiting... > ";
	string input;
	std::cin >> input;
	
	server.send(impl, error);
	if (error) {
		ERROR_PRINTLN("error occured while server sending the data.");
		return -1;
	}
	
	std::cout << "waiting... > ";	
	std::cin >> input;
	
	return 0;
}
