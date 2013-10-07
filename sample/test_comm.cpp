#include <stdio.h>
#include <string>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#if defined(__linux__) || defined(__APPLE__)
#include "protobuf_utils/debug.h"
#endif // #if defined(__linux__) || defined(__APPLE__)
#include "protobuf_utils/utils.hpp"
#include "comm.pb.h"

using comm::Message;
using comm::Data;
using std::string;

using protobuf_utils::Serializable;
using protobuf_utils::DataBuilder;
using protobuf_utils::Recievable;
using protobuf_utils::ProtocolBufferServer;
using protobuf_utils::ProtocolBufferClient;


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
	//ProtocolBufferServer server(1111, msg_impl);
	//boost::shared_ptr<ProtocolBufferServer> server_ptr(new ProtocolBufferServer(1111, msg_impl));
	boost::system::error_code error;
	boost::shared_ptr<ProtocolBufferServer> server_ptr = ProtocolBufferServer::create(1111, msg_impl, error);
	if (error) {
		ERROR_PRINTLN("failed to activate server.");
		return -1;
	}
	//boost::shared_ptr<ProtocolBufferServer> server_ptr((ProtocolBufferServer*)NULL);
	ProtocolBufferServer& server(*server_ptr.get());	
	server.register_reciever(&msg_impl);

	Message msg;
	msg.set_id(10);
	msg.set_type(Message::ACK);
	msg.add_data()->set_status("hogehoge");

	MessageImpl impl(msg);
	impl.print();

#if defined(__linux__) || defined(__APPLE__)
	DEBUG_PRINTLN("serialized size: %d", impl.getSerializedSize());
#else
	printf("serialized size: %d\n", impl.getSerializedSize());
#endif 

	ProtocolBufferClient client("localhost", 1111, msg_impl);

	for (int i = 0; i < 10; i++) {
		client.send(impl);
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	}


	/*
	server.send(impl, error);
	if (error) {
		ERROR_PRINTLN("error occured while server sending the data.");
		return -1;
	}
	*/
	boost::thread bt(boost::bind(&ProtocolBufferServer::send, 
				     &server, 
				     impl, 
				     error
				 )
		);


#if defined(__linux__) || defined(__APPLE__)
	DEBUG_PRINTLN("client recieving data");
#else
	printf("client recieving data\n");
#endif

	boost::shared_ptr<Serializable> data;
	client.recv(data, error);
	if (!error) {
		MessageImpl* impl = (MessageImpl*)data.get();
		impl->print();
	} else {
#if defined(__linux__) || defined(__APPLE__)
		ERROR_PRINTLN("error occured while client recieving the data");
#endif
		return -1;
	}

	return 0;
}
