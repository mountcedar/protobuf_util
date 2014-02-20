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
using protobuf::ProtocolBuffersClient;

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

int main(int argc, char* argv[])
{
	boost::system::error_code error;

	Message<comm::Message> msg_impl;
	comm::Message msg;
	msg.set_id(10);
	msg.set_type(comm::Message::ACK);
	msg.add_data()->set_status("hogehoge");
	Message<comm::Message> impl(msg);
	//impl.print();
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

	DEBUG_PRINTLN("client recieving data");
	boost::shared_ptr<Serializable> data;
	client.recv(data, error);
	if (!error) {
		Message<comm::Message>* impl = (Message<comm::Message>*)data.get();
		//impl->print();
		print_msg (impl->get());
	} else {
		ERROR_PRINTLN("error occured while client recieving the data");
		return -1;
	}

	return 0;
}
