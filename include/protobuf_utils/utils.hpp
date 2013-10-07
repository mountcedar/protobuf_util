#pragma once

#include <cstdlib>

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>

#if defined(__linux__) || defined(__APPLE__)
#include "debug.h"
#endif // #if defined(__linux__) || defined(__APPLE__)

using boost::asio::ip::tcp;
using std::vector;
using std::string;
using std::map;
using std::stringstream;
using std::pair;

#if !defined NAMESPACE_PROTOBUF_UTILS
#define NAMESPACE_PROTOBUF_UTILS protobuf_utils
#endif //#if !defined NAMESPACE_PROTOBUF_UTILS

#define BEGIN_NAMESPACE_PROTOBUF_UTILS namespace NAMESPACE_PROTOBUF_UTILS {
#define END_NAMESPACE_PROTOBUF_UTILS }

/**
   @brief the top namespace to make the namespace original.
 */
BEGIN_NAMESPACE_PROTOBUF_UTILS

static const string toString(int value) {
	stringstream ss;
	ss << value;
	return ss.str();
}

/**
 * The Interface Serializable.
 *
 * @brief
 * the wrapper for the protocol buffer class
 * @details
 * this code is original from
 * - https://github.com/mountcedar/local.protobuf.socket/blob/master/src/local/protobuf/socket/data/Serializable.java
 * @author sugiyama
 */
class Serializable {
public:
	virtual ~Serializable(void) {}
	virtual int getSerializedSize() = 0;
	virtual bool serialize(string& output) = 0;
	virtual bool deserialize(const string& data) = 0;
};

/**
 * The Interface Receivable.
 *
 * @brief
 * the receivable interface for protocol buffer server.
 * @details
 * this code is original from
 * - https://github.com/mountcedar/local.protobuf.socket/blob/master/src/local/protobuf/socket/data/Recievable.java
 * @author sugiyama
 */
class Recievable {
public:
	/**
	 * On recv.
	 *
	 * @param data the protobuf binary data
	 * @return true, if successful
	 */
	virtual bool onRecv(Serializable* data) = 0;
	virtual ~Recievable(void) {}
};

/**
 * The Interface DataBuilder.
 *
 * @brief
 * the builder for the serializable objects
 * @details
 * this code is original from
 * - https://github.com/mountcedar/local.protobuf.socket/blob/master/src/local/protobuf/socket/data/DataBuilder.java
 * @author sugiyama
 */
class DataBuilder {
public:
	/**
	 * Creates the.
	 *
	 * @param binary the binary
	 * @return the serializable
	 */
	virtual Serializable* create(string& data) = 0;
	virtual ~DataBuilder(void) {}
};


class ProtocolBufferServer;
class RequestHandler;

/**
   The Class RequestHandler.

   @brief 
   the request handler for receiving and sending the protocol buffer
   messages.

   @details
   this code is original from
   - https://github.com/mountcedar/local.protobuf.socket/blob/master/src/local/protobuf/socket/server/RequestHandler.java

   @author sugiyama
 */
class RequestHandler {
public:
	RequestHandler(ProtocolBufferServer& host,
		       boost::asio::io_service& io_service, 
		       DataBuilder& builder,
		       vector<Recievable*>& recievers);

	~RequestHandler(void);

	void finalize(void);

	tcp::socket& socket(void);

	void start(void);

	void send(Serializable& data, boost::system::error_code& error);

	void handle_read_size(const boost::system::error_code& error,
			      size_t bytes_transferred);
	
	void handle_read_data(const boost::system::error_code& error,
			      size_t bytes_transferred);

private:
	ProtocolBufferServer& host_;
	tcp::socket socket_;
	int buff_size_;
	boost::shared_ptr<char> buff_;
	DataBuilder& builder_;
	vector<Recievable*>& recievers_;
	bool active_;
	string hostname_;
};


/**
   The Class ProtocolBufferServer.
   @brief the server to stream or receive protocol buffer
   @details
   this code is original from
   - https://github.com/mountcedar/local.protobuf.socket/blob/master/src/local/protobuf/socket/server/ProtocolBufferServer.java
   @author sugiyama
 */
class ProtocolBufferServer {
public:
	static boost::shared_ptr<ProtocolBufferServer> create(
		short port, 
		DataBuilder& builder, 
		boost::system::error_code& error);

	~ProtocolBufferServer(void);

	void register_reciever(Recievable* reciever);

	void register_request(string hostname, RequestHandler* request);

	void deregister_request(string hostname);

	void send(Serializable& data, boost::system::error_code& error);

	const vector<Recievable*> get_recievers(void);

	void handle_accept(RequestHandler* new_session,
			   const boost::system::error_code& error);

private:
	boost::asio::io_service io_service_;
	tcp::acceptor acceptor_;
	DataBuilder& builder_;
	vector<Recievable*> recievers_;
	boost::thread thread_;
	bool terminate_;
	map<string, RequestHandler*> requests_;
	static boost::system::error_code connection_error_;

	ProtocolBufferServer(short port,
			     DataBuilder& builder);
};

///////////////////////////////////////////////////////////////////////////////////////
// Definition of RequestHandler
///////////////////////////////////////////////////////////////////////////////////////

RequestHandler::RequestHandler(ProtocolBufferServer& host,
			       boost::asio::io_service& io_service, 
			       DataBuilder& builder,
			       vector<Recievable*>& recievers)
	: host_(host),
	  socket_(io_service),
	  buff_size_(0),
	  builder_(builder),
	  recievers_(recievers),
	  active_(false),
	  hostname_()
{
}

RequestHandler::~RequestHandler(void) {}

void
RequestHandler::finalize(void) {
	active_ = false;
	host_.deregister_request(hostname_);
	DEBUG_PRINTLN("request handler terminated.");
}

tcp::socket& 
RequestHandler::socket() {
	return socket_;
}

void 
RequestHandler::start() {
#if defined(__linux__) || defined(__APPLE__)
	DEBUG_PRINTLN("STARTED");
#endif // #if defined(__linux__) || defined(__APPLE__)

	hostname_ = socket_.remote_endpoint().address().to_string();
	host_.register_request(hostname_, this);
	char* buff = reinterpret_cast<char*>(&buff_size_);
	socket_.async_read_some(
		//boost::asio::buffer(data, max_length),
		boost::asio::buffer(buff, sizeof(int)),
		boost::bind(
			&RequestHandler::handle_read_size, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
			)
		);
	active_ = true;
}

void
RequestHandler::send(Serializable& data, boost::system::error_code& error) {
	string serialized_data;
	data.serialize(serialized_data);
	int size = data.getSerializedSize();
	// convert to long for network stream
	size = htonl(size);
	const char * px = reinterpret_cast<const char*>(&size);
	
	//DEBUG_PRINTLN("writing size: <%s><%d>", px, size);
	boost::asio::write(
		socket_,
		boost::asio::buffer(px, sizeof(int)),
		boost::asio::transfer_all(),
		error
		);

	if (error) {
#if defined(__linux__) || defined(__APPLE__)
		ERROR_PRINTLN("Failed to send size: %d", size);
#endif // #if defined(__linux__) || defined(__APPLE__)
		return;
	}

	//DEBUG_PRINTLN("writing data stream: <%s>", 
	//	      serialized_data.c_str());

	boost::asio::write(
		socket_, 
		boost::asio::buffer(serialized_data, size),
		boost::asio::transfer_all(),
		error
		);
	
	if (error) {
#if defined(__linux__) || defined(__APPLE__)
		ERROR_PRINTLN("Failed to send binary: %s", 
			      serialized_data.c_str());
#endif // #if defined(__linux__) || defined(__APPLE__)
	}

	//DEBUG_PRINTLN("sending completed");
	return;	
}

void 
RequestHandler::handle_read_size(const boost::system::error_code& error,
				 size_t bytes_transferred) {
	if (!error) {
		buff_size_ = ntohl(buff_size_);
		/*
		buff_ = boost::shared_ptr<char>(
			(char*)malloc(buff_size_), 
			std::ptr_fun(free)
			);
		*/
		buff_ = boost::shared_ptr<char>(new char[buff_size_]);

		socket_.async_read_some(
			boost::asio::buffer(buff_.get(), (size_t)buff_size_),
			boost::bind(
				&RequestHandler::handle_read_data, 
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
				)
			);
		
	} else {
#if defined(__linux__) || defined(__APPLE__)
		ERROR_PRINTLN("FAILED");
#endif // #if defined(__linux__) || defined(__APPLE__)
		//delete this;
		finalize();
	}
}

void 
RequestHandler::handle_read_data(const boost::system::error_code& error,
				 size_t bytes_transferred) {
	if (!error) {
		string input(reinterpret_cast<char const*>(buff_.get()), 
			     bytes_transferred);
		//DEBUG_PRINTLN("input data size: %ld", input.size());
		Serializable* inst = builder_.create(input);
		boost::shared_ptr<Serializable> data_obj(inst);
		if (inst != NULL) {
			Recievable* reciever = NULL;
			BOOST_FOREACH(reciever, recievers_) {
				reciever->onRecv(data_obj.get());
			}
		} else {
#if defined(__linux__) || defined(__APPLE__)
			ERROR_PRINTLN("FAILED");
#endif // #if defined(__linux__) || defined(__APPLE__)
		}
		char* buff = reinterpret_cast<char*>(&buff_size_);
		socket_.async_read_some(
			boost::asio::buffer(buff, sizeof(int)),
			boost::bind(&RequestHandler::handle_read_size, this,
				    boost::asio::placeholders::error,
				    boost::asio::placeholders::bytes_transferred));
		
	} else {
#if defined(__linux__) || defined(__APPLE__)
		ERROR_PRINTLN("FAILED");
#endif // #if defined(__linux__) || defined(__APPLE__)
		//delete this;
		finalize();
	}	
}


///////////////////////////////////////////////////////////////////////////////////////
// Definition of Protocol Buffer Server.
///////////////////////////////////////////////////////////////////////////////////////

boost::system::error_code 
ProtocolBufferServer::connection_error_((int)boost::system::errc::address_not_available,
					boost::system::system_category());

ProtocolBufferServer::ProtocolBufferServer(short port,
					   DataBuilder& builder)
	: io_service_(),
	  acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
	  builder_(builder),
	  recievers_(),
	  thread_(boost::bind(
			  &boost::asio::io_service::run, 
			  &io_service_
			  )
		  ),
	  terminate_(false),
	  requests_()
{
	acceptor_.set_option(tcp::acceptor::reuse_address(true));
	/*
	RequestHandler* new_session = new RequestHandler(
		*this,
		io_service_, 
		builder_, 
		recievers_);
	*/
	boost::shared_ptr<RequestHandler> request(
		new RequestHandler(
			*this,
			io_service_, 
			builder_, 
			recievers_)
		);

	acceptor_.async_accept(
		request.get()->socket(),
		boost::bind(&ProtocolBufferServer::handle_accept, 
			    this, 
			    request.get(),
			    boost::asio::placeholders::error)
		);
}

ProtocolBufferServer::~ProtocolBufferServer(void) {
	io_service_.stop();
	terminate_ = true;
	thread_.timed_join(boost::posix_time::milliseconds(500));
	terminate_ = false;
#if defined(__linux__) || defined(__APPLE__)
	DEBUG_PRINTLN("ProtocolBufferServer terminated.");
#endif // #if defined(__linux__) || defined(__APPLE__)
}


boost::shared_ptr<ProtocolBufferServer> 
ProtocolBufferServer::create(short port, DataBuilder& builder,
			     boost::system::error_code& error) {
	try {
		ProtocolBufferServer* serverp = new ProtocolBufferServer(port, builder);
		if (!serverp) {
#if defined(__linux__) || defined(__APPLE__)
			ERROR_PRINTLN("cannot create ProtocolBufferServer object.");
#endif // #if defined(__linux__) || defined(__APPLE__)
			error = connection_error_;
		}
		boost::shared_ptr<ProtocolBufferServer> server(serverp);
		return server;
	} catch (...) {
#if defined(__linux__) || defined(__APPLE__)
		ERROR_PRINTLN("failed to created ProtocolBufferServer object.");
#endif // #if defined(__linux__) || defined(__APPLE__)
		error = connection_error_;
		boost::shared_ptr<ProtocolBufferServer> server((ProtocolBufferServer*)NULL);
		return server;
	}
}

void 
ProtocolBufferServer::register_reciever(Recievable* reciever) {
	recievers_.push_back(reciever);
}

const vector<Recievable*> 
ProtocolBufferServer::get_recievers(void) {
	return recievers_;
}

void
ProtocolBufferServer::register_request(string hostname, 
				       RequestHandler* request) {
	requests_.insert(map<string, RequestHandler*>::value_type(
				 hostname,
				 request)
		);
}

void
ProtocolBufferServer::deregister_request(string hostname) {
	requests_.erase(hostname);
}

void
ProtocolBufferServer::send(Serializable& data,
			   boost::system::error_code& error) {
	pair<string, RequestHandler*> pair_;
	BOOST_FOREACH(pair_, requests_) {
		RequestHandler* request = pair_.second;
		request->send(data, error);
		if (error) {
#if defined(__linux__) || defined(__APPLE__)
			ERROR_PRINTLN("failed to send data");
			break;
#endif // #if defined(__linux__) || defined(__APPLE__)
		}
	}
	return;
}

void 
ProtocolBufferServer::handle_accept(RequestHandler* new_session,
				    const boost::system::error_code& error) {
#if defined(__linux__) || defined(__APPLE__)
	DEBUG_PRINTLN("ACCEPTED");
#endif // #if defined(__linux__) || defined(__APPLE__)
	if (!error || !terminate_) {
		new_session->start();
		boost::shared_ptr<RequestHandler> request(
			new RequestHandler(
				*this,
				io_service_, 
				builder_, 
				recievers_
				)
			);
		acceptor_.async_accept(
			request.get()->socket(),
			boost::bind(
				&ProtocolBufferServer::handle_accept, 
				this, 
				request.get(),
				boost::asio::placeholders::error
				)
			);
	}
	else {
#if defined(__linux__) || defined(__APPLE__)
		ERROR_PRINTLN("FAILED to create new session");
#endif // #if defined(__linux__) || defined(__APPLE__)
		//delete new_session;
	}
}


class ProtocolBufferClient {
public:
	ProtocolBufferClient(const string& hostname, 
			     int port, 
			     DataBuilder& builder)
		: io_service(), 
		  resolver(io_service),
		  query(tcp::v4(), hostname.c_str(), toString(port)),
		  socket_(io_service),
		  builder_(builder)
	{
		boost::system::error_code error = boost::asio::error::host_not_found;
		iterator = resolver.resolve(query);
#if defined(__linux__) || defined(__APPLE__)
		DEBUG_PRINTLN("connecting %s:%d", hostname.c_str(), port);
#endif // #if defined(__linux__) || defined(__APPLE__)
		socket_.connect(*iterator, error);
		if (error) {
#if defined(__linux__) || defined(__APPLE__)
			ERROR_PRINTLN("FAILED TO CONNECT SERVER: %s", 
				      hostname.c_str());
#endif // #if defined(__linux__) || defined(__APPLE__)
		} else {
#if defined(__linux__) || defined(__APPLE__)
			DEBUG_PRINTLN("Connected...");
#endif
		}
	}


	bool send(Serializable& data) {
		string serialized_data;
		data.serialize(serialized_data);
		int size = data.getSerializedSize();
		// convert to long for network stream
		size = htonl(size);
		const char * px = reinterpret_cast<const char*>(&size);
		
		//DEBUG_PRINTLN("writing size: <%s><%d>", px, size);
		boost::asio::write(
			socket_,
			boost::asio::buffer(px, sizeof(int)),
			boost::asio::transfer_all()
			);

		/*
		DEBUG_PRINTLN("writing data stream: <%s>", 
			      serialized_data.c_str());
		*/
		boost::asio::write(
			socket_, 
			boost::asio::buffer(serialized_data, size),
			boost::asio::transfer_all()
			);

		//DEBUG_PRINTLN("sending completed");
		return true;
	}

	void recv(boost::shared_ptr<Serializable>& data,
		  boost::system::error_code& error) {
		int buff_size = 0;
		char* size_p = reinterpret_cast<char*>(&buff_size);

		boost::asio::read(
			socket_, 
			boost::asio::buffer(size_p, sizeof(int)),
			error
			);

		if (error) {
#if defined(__linux__) || defined(__APPLE__)
			ERROR_PRINTLN("failed to recieve the data");
#endif // #if defined(__linux__) || defined(__APPLE__)
			return;
		}
		
		buff_size = ntohl(buff_size);
		boost::shared_ptr<char> buff((char*)malloc(buff_size), std::ptr_fun(free));
		//DEBUG_PRINTLN("size: %d", buff_size);
		boost::asio::read(
			socket_, 
			boost::asio::buffer(buff.get(), buff_size),
			error
			);
		if (error) {
#if defined(__linux__) || defined(__APPLE__)
			ERROR_PRINTLN("failed to recieve the data");
#endif // #if defined(__linux__) || defined(__APPLE__)
			return;
		}
		string input(
			reinterpret_cast<char const*>(buff.get()), 
			buff_size);
		Serializable* inst = builder_.create(input);
		data = boost::shared_ptr<Serializable>(inst);
		return;
	}

private:
	boost::asio::io_service io_service;
	tcp::resolver resolver;
	tcp::resolver::query query;
	tcp::resolver::iterator iterator;
	tcp::socket socket_;
	
	DataBuilder& builder_;
};

END_NAMESPACE_PROTOBUF_UTILS