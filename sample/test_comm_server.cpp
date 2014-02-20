#define ENABLE_DEBUGPRINT 1

#include <stdio.h>
#include <string>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "protobuf.net.hpp"
#include "comm.pb.h"

using comm::Message;
using comm::Data;
using std::string;

using protobuf::Serializable;
using protobuf::DataBuilder;
using protobuf::Recievable;
using protobuf::ProtocolBuffersServer;


class MessageImpl
	: public Serializable, 
	  public DataBuilder, 
	  public Recievable 
{
public:
	Message msg;
	
	MessageImpl(Message& data): msg(data), binary() {
		msg.SerializeToString(&binary);
	}

	MessageImpl(void): msg(), binary() {}

	virtual ~MessageImpl() {}

	int getSerializedSize() {
		return binary.size();
	}

	bool serialize(string& output) {
		msg.SerializeToString(&output);
		return true;
	}

	bool deserialize(const string& data) {
		msg.ParseFromString(data);
		binary = data;
		return true;
	}

	bool onRecv(Serializable* data) {
		DEBUG_PRINTLN("on recv");
		MessageImpl* impl = (MessageImpl*)data;
		impl->print();
		return true;
	}

	Serializable* create(string& data) {
		Message temp;
		temp.ParseFromString(data);
		return (Serializable*)new MessageImpl(temp);
	}

	void print(void) {
		fprintf(stderr, "%d\n", msg.id());
		fprintf(stderr, "data size: %d\n", msg.data_size());
		for (int i = 0; i < msg.data_size(); i++) {
			const Data& _data = msg.data(i);
			fprintf(stderr,
				"\tstatus: %s\n", 
				_data.status().c_str());
		}
	}

private:
	string binary;
};

int main(int argc, char* argv[])
{
	MessageImpl msg_impl;
	//ProtocolBuffersServer server(1111, msg_impl);
	//boost::shared_ptr<ProtocolBuffersServer> server_ptr(new ProtocolBuffersServer(1111, msg_impl));
	boost::system::error_code error;
	boost::shared_ptr<ProtocolBuffersServer> server_ptr = ProtocolBuffersServer::create(1111, msg_impl, error);
	if (error) {
		ERROR_PRINTLN("failed to activate server.");
		return -1;
	}
	//boost::shared_ptr<ProtocolBuffersServer> server_ptr((ProtocolBuffersServer*)NULL);
	ProtocolBuffersServer& server(*server_ptr.get());	
	server.register_reciever(&msg_impl);
	boost::thread (boost::bind(&ProtocolBuffersServer::start, &server));

	Message msg;
	msg.set_id(10);
	msg.set_type(Message::ACK);
	msg.add_data()->set_status("hogehoge");

	MessageImpl impl(msg);
	impl.print();
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
