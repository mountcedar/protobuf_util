#define ENABLE_DEBUGPRINT 1

#include <stdio.h>
#include <string>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "protobuf.net.hpp"
#include "comm.pb.h"

using comm::Data;
using std::string;

using protobuf::Recievable;
using protobuf::Serializable;
using protobuf::ProtocolBuffersServer;
using protobuf::ProtocolBuffersClient;
using protobuf::Message;

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
	Reciever recv_handler;
	boost::shared_ptr<ProtocolBuffersServer> server_ptr = ProtocolBuffersServer::create(1111, msg_impl, error);
	if (error) {
		ERROR_PRINTLN("failed to activate server.");
		return -1;
	}

	ProtocolBuffersServer& server(*server_ptr.get());	
	server.register_reciever(&recv_handler);
	server.start();

	DEBUG_PRINTLN("protocol buffers server activated.");

	comm::Message msg;
	msg.set_id(10);
	msg.set_type(comm::Message::ACK);
	msg.add_data()->set_status("hogehoge");

	Message<comm::Message> impl(msg);
	print_msg (impl.get());
	DEBUG_PRINTLN("serialized size: %d", impl.getSerializedSize());

	ProtocolBuffersClient client("localhost", 1111, msg_impl);

	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	for (int i = 0; i < 10; i++) {
		try {
			client.send(impl);
			boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
		} catch (...) {
			ERROR_PRINTLN("client send failed.");
			return -1;
		}
	}

	std::cout << "waiting... > ";
	string input;
	std::cin >> input;
	
	DEBUG_PRINTLN("server gonna send");	
	boost::thread bt(boost::bind(&ProtocolBuffersServer::send, 
				     &server, 
				     impl, 
				     error
				 )
		);


	DEBUG_PRINTLN("client recieving data");
	boost::shared_ptr<Serializable> data;
	client.recv(data, error);
	if (!error) {
		//MessageImpl* impl = (MessageImpl*)data.get();
		Message<comm::Message>* impl = (Message<comm::Message>*)data.get();
		print_msg (impl->get());
	} else {
		ERROR_PRINTLN("error occured while client recieving the data");
	}

	std::cout << "waiting ... > ";
	std::cin >> input;
	
	return 0;
}
