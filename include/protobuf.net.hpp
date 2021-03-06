#pragma once

#include <cstdlib>

#include <vector>
#include <list>
#include <string>
#include <map>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>

using boost::asio::ip::tcp;
using std::vector;
using std::list;
using std::string;
using std::map;
using std::stringstream;
using std::pair;

#if !defined NAMESPACE_PROTOBUF_UTILS
#define NAMESPACE_PROTOBUF_UTILS protobuf
#endif //#if !defined NAMESPACE_PROTOBUF_UTILS

#define BEGIN_NAMESPACE_PROTOBUF_UTILS namespace NAMESPACE_PROTOBUF_UTILS {
#define END_NAMESPACE_PROTOBUF_UTILS }

/**
   @brief デバッグプリントの定義

   @details
   formatに従ってメッセージを関数名、ファイル名、行番号と共にDEBUGOUTに出力し、改行する．

   @param[in] format fprintfのformatと同じ
   @param[in] args fprintfの残りの引数と同義

   @note windowsの場合、プリプロセッサの変数がないので、普通のfprintf
 */

#if !defined(ENABLE_DEBUGPRINT)
   #define ENABLE_DEBUGPRINT 0
#endif // #if !defined(ENABLE_DEBUGPRINT)

#if !defined(DEBUGOUT)
   #define DEBUGOUT stderr
#endif // !defined(DEBUGOUT)

#if ENABLE_DEBUGPRINT
	#if defined(__linux__) || defined(__APPLE__)

		#define DEBUG_PRINTLN(format, args...)				\
			fprintf(DEBUGOUT, "[%s : %s, line %d] "format"\n",	\
				__FUNCTION__, __FILE__, __LINE__, ##args)

		#define DEBUG_COLOR_PRINTLN(color, format, args...)			 \
			fprintf(DEBUGOUT,						 \
				"\x1b["color"m""[%s : %s, line %d] "format"\x1b[0m""\n", \
				 __FUNCTION__, __FILE__, __LINE__, ##args)

		#define DEBUGPRINT_COLOR_FG_RED    "31" ///< 赤(文字色) 
		#define DEBUGPRINT_FONT_BOLD       "1"  ///< 太字  

		#define ERROR_PRINTLN(format, args...)					  \
			DEBUG_COLOR_PRINTLN(DEBUGPRINT_COLOR_FG_RED";"DEBUGPRINT_FONT_BOLD, \
					  format,					  \
					  ##args)

	#else // #if defined(__linux__) || defined(__APPLE__)

		#define DEBUG_PRINTLN(format, args...)				\
			fprintf(DEBUGOUT, format"\n", ##args)

		#define ERROR_PRINTLN(format, args...)				\
			fprintf(DEBUGOUT, format"\n", ##args)

	#endif // #if defined(__linux__) || defined(__APPLE__)
#else // #if ENABLE_DEBUGPRINT
	#define DEBUG_PRINTLN(format, args...)
	#define ERROR_PRINTLN(format, args...)
#endif // #if ENABLE_DEBUGPRINT

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

template <class T>
class Message
	: public Serializable, 
	  public DataBuilder
{
public:	
	Message(T& data): msg(data), binary() {
		msg.SerializeToString(&binary);
	}

	Message(void): msg(), binary() {}

	virtual ~Message() {}

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

	Serializable* create(string& data) {
		T temp;
		temp.ParseFromString(data);
		return (Serializable*)new Message(temp);
	}

	const T& get (void) {
		return msg;
	} 

private:
	T msg;	
	string binary;
};

class ProtocolBuffersServer;
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
	friend class ProtocolBuffersServer;

public:
	RequestHandler(ProtocolBuffersServer& host,
		       boost::asio::io_service& io_service, 
		       DataBuilder& builder,
		       vector<Recievable*>& recievers);

	~RequestHandler(void);

	void finalize(void);

	tcp::socket& socket(void);

	bool start (void);

	void stop (void);

	void send(Serializable& data, boost::system::error_code& error);

	void handle_read_size(const boost::system::error_code& error,
			      size_t bytes_transferred);
	
	void handle_read_data(const boost::system::error_code& error,
			      size_t bytes_transferred);

	bool is_active(void);

	void debugprint_hostname (void);

private:
	static list<RequestHandler*> on_duties;
	static list<RequestHandler*> off_duties;
	static boost::mutex m;

	static void on_duty (RequestHandler* obj);
	static void off_duty (RequestHandler* obj);
	static void update_duties (void);
	static void done (void);

	ProtocolBuffersServer& host_;
	tcp::socket socket_;
	int buff_size_;
	boost::shared_ptr<char> buff_;
	DataBuilder& builder_;
	vector<Recievable*>& recievers_;
	bool active_;
	string hostname_;
};


/**
   The Class ProtocolBuffersServer.
   @brief the server to stream or receive protocol buffer
   @details
   this code is original from
   - https://github.com/mountcedar/local.protobuf.socket/blob/master/src/local/protobuf/socket/server/ProtocolBuffersServer.java
   @author sugiyama
 */
class ProtocolBuffersServer {
public:
	static boost::shared_ptr<ProtocolBuffersServer> create(
		short port, 
		DataBuilder& builder, 
		boost::system::error_code& error);

	~ProtocolBuffersServer (void);

	bool start (void);

	void stop (void);

	void start_accept (void);

	void register_reciever (Recievable* reciever);

	void send (Serializable& data, boost::system::error_code& error);

	const vector<Recievable*> get_recievers (void);

	void handle_accept(RequestHandler* new_session,
			   const boost::system::error_code& error);

private:
	boost::asio::io_service io_service_;
	tcp::acceptor acceptor_;
	DataBuilder& builder_;
	vector<Recievable*> recievers_;
	boost::thread* thread_;
	bool terminate_;
	vector<RequestHandler*> requests_;
	static boost::system::error_code connection_error_;

	ProtocolBuffersServer(short port, DataBuilder& builder);
};

///////////////////////////////////////////////////////////////////////////////////////
// Definition of RequestHandler
///////////////////////////////////////////////////////////////////////////////////////

list<RequestHandler*> RequestHandler::on_duties;
list<RequestHandler*> RequestHandler::off_duties;
boost::mutex RequestHandler::m;

void 
RequestHandler::on_duty (RequestHandler* obj) {
	boost::mutex::scoped_lock lock(m);
	on_duties.push_back (obj);
}

void
RequestHandler::off_duty (RequestHandler* obj) {
	boost::mutex::scoped_lock lock(m);
	off_duties.push_back (obj);
}

void 
RequestHandler::update_duties (void) {
	boost::mutex::scoped_lock lock(m);
	RequestHandler* obj;
	BOOST_FOREACH(obj, off_duties) {
		// vector<RequestHandler*>::iterator end_it = on_duties.remove (on_duties.begin(), on_duties.end(), obj);
		// on_duties.erase (end_it, on_duties.end());
		on_duties.remove (obj);
		delete obj;
	}
	off_duties.clear ();
}

void 
RequestHandler::done (void) {
	boost::mutex::scoped_lock lock(m);
	RequestHandler* obj;
	BOOST_FOREACH (obj, on_duties) {
		delete obj;
	}
	on_duties.clear();
}

RequestHandler::RequestHandler(ProtocolBuffersServer& host,
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
	on_duty (this);
}

RequestHandler::~RequestHandler(void) {}

void
RequestHandler::finalize(void) {
	active_ = false;
	//host_.deregister_request(hostname_);
	//host_.deregister_request(this);

	DEBUG_PRINTLN("request handler terminated.");
}

tcp::socket& 
RequestHandler::socket() {
	return socket_;
}

bool
RequestHandler::start() {
	DEBUG_PRINTLN("STARTING");
	char* buff = reinterpret_cast<char*>(&buff_size_);
	DEBUG_PRINTLN("ASYNC READ SOME");

	socket_.async_read_some(
		//boost::asio::buffer(data, max_length),
		boost::asio::buffer(buff, sizeof(int)),
		boost::bind(
			&RequestHandler::handle_read_size, 
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
			)
		);
	active_ = true;
	DEBUG_PRINTLN("STARTED");	
	return true;
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
		ERROR_PRINTLN("Failed to send size: %d", size);
		return;
	} else {
		DEBUG_PRINTLN ("Succeeded to send size: %d", size);
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
		ERROR_PRINTLN("Failed to send binary: %s", 
			      serialized_data.c_str());
	} else {
		DEBUG_PRINTLN ("succeeded to send binary");
	}

	//DEBUG_PRINTLN("sending completed");
	return;	
}

void 
RequestHandler::handle_read_size(const boost::system::error_code& error,
				 size_t bytes_transferred) {
	if (!error) {
		buff_size_ = ntohl(buff_size_);
		DEBUG_PRINTLN("buff size: %d", buff_size_);
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
		ERROR_PRINTLN("FAILED");
		off_duty (this);
	}
}

void 
RequestHandler::handle_read_data(const boost::system::error_code& error,
				 size_t bytes_transferred) {
	if (!error) {
		string input(reinterpret_cast<char const*>(buff_.get()), 
			     bytes_transferred);
		DEBUG_PRINTLN("input data size: %lu", input.size());
		Serializable* inst = builder_.create(input);
		boost::shared_ptr<Serializable> data_obj(inst);
		if (inst != NULL) {
			Recievable* reciever = NULL;
			BOOST_FOREACH(reciever, recievers_) {
				reciever->onRecv(data_obj.get());
			}
		} else {
			ERROR_PRINTLN("FAILED");
		}
		char* buff = reinterpret_cast<char*>(&buff_size_);
		socket_.async_read_some(
			boost::asio::buffer(buff, sizeof(int)),
			boost::bind(&RequestHandler::handle_read_size, this,
				    boost::asio::placeholders::error,
				    boost::asio::placeholders::bytes_transferred));
		
	} else {
		ERROR_PRINTLN("FAILED");
		off_duty (this);
	}	
}

bool
RequestHandler::is_active(void) {
	return active_;
}


void 
RequestHandler::debugprint_hostname (void) {
/**
	@note
	print out hostname. In mac os x, this procedure is not properly working for some reason, 
	so only activate it in the linux.
*/
#if defined(__linux__)
	boost::system::error_code error_;
	boost::asio::ip::tcp::endpoint endpoint = socket_.remote_endpoint(error_);
	if (error_) {
		DEBUG_PRINTLN("endpoint error<%d>: %s", error_.value(), error_.message().c_str());
	} else {
		string hostname_ = endpoint.address().to_string();
		DEBUG_PRINTLN("HOSTNAME: %s", hostname_.c_str());
	}
#endif // #if defined(__linux__)
}

///////////////////////////////////////////////////////////////////////////////////////
// Definition of Protocol Buffer Server.
///////////////////////////////////////////////////////////////////////////////////////

boost::system::error_code 
ProtocolBuffersServer::connection_error_((int)boost::system::errc::address_not_available,
					boost::system::system_category());

ProtocolBuffersServer::ProtocolBuffersServer(short port, DataBuilder& builder)
	: io_service_(),
	  acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
	  builder_(builder),
	  recievers_(),
	  //thread_(boost::bind(&boost::asio::io_service::run, &io_service_)),
	  thread_(NULL),
	  terminate_(false),
	  requests_()
{
	acceptor_.set_option(tcp::acceptor::reuse_address(true));
	start_accept();
}


ProtocolBuffersServer::~ProtocolBuffersServer(void) {
	stop();
	DEBUG_PRINTLN("ProtocolBuffersServer terminated.");
}

boost::shared_ptr<ProtocolBuffersServer> 
ProtocolBuffersServer::create(short port, DataBuilder& builder,
			     boost::system::error_code& error) {
	try {
		ProtocolBuffersServer* serverp = new ProtocolBuffersServer(port, builder);
		if (!serverp) {
			ERROR_PRINTLN("cannot create ProtocolBuffersServer object.");
			error = connection_error_;
		}
		boost::shared_ptr<ProtocolBuffersServer> server(serverp);
		return server;
	} catch (...) {
		ERROR_PRINTLN("failed to created ProtocolBuffersServer object.");
		error = connection_error_;
		boost::shared_ptr<ProtocolBuffersServer> server((ProtocolBuffersServer*)NULL);
		return server;
	}
}

bool 
ProtocolBuffersServer::start (void) {
	if (thread_) return true;
	DEBUG_PRINTLN("starting io service");
	thread_ = new boost::thread(
		boost::bind(&boost::asio::io_service::run, &io_service_)
	);
	DEBUG_PRINTLN("io service started.");
	return true;
}

void 
ProtocolBuffersServer::stop (void) {
	if (!thread_) return;
	io_service_.stop();
	thread_->timed_join(boost::posix_time::milliseconds(500));
	delete thread_;
	thread_ = NULL;
	return;
}

void 
ProtocolBuffersServer::register_reciever(Recievable* reciever) {
	recievers_.push_back(reciever);
}

const vector<Recievable*> 
ProtocolBuffersServer::get_recievers(void) {
	return recievers_;
}

void
ProtocolBuffersServer::send(Serializable& data,
			   boost::system::error_code& error) {
	//pair<string, RequestHandler*> pair_;
	//reconfigure_handlers();
	RequestHandler::update_duties();
	RequestHandler* request = NULL;
	BOOST_FOREACH(request, RequestHandler::on_duties) {
		request->send(data, error);
		if (error) {
			ERROR_PRINTLN("failed to send data");
			break;
		}
	}
	return;
}

void
ProtocolBuffersServer::start_accept (void) {
	// boost::shared_ptr<RequestHandler> request(
	// 	new RequestHandler(*this, io_service_, builder_, recievers_)
	// 	);

	RequestHandler* new_session = new RequestHandler (*this, io_service_, builder_, recievers_);
	acceptor_.async_accept(
		new_session->socket(),
		boost::bind(&ProtocolBuffersServer::handle_accept, 
			    this, 
			    new_session,
			    boost::asio::placeholders::error)
		);
}

void 
ProtocolBuffersServer::handle_accept(RequestHandler* new_session,
				    const boost::system::error_code& error) {
	DEBUG_PRINTLN("ACCEPTED");
	if (!error || !terminate_) {
		new_session->debugprint_hostname();
		if(!new_session->start()) {
			DEBUG_PRINTLN("new session start failed.");
		}
		start_accept();
	}
	else {
		ERROR_PRINTLN("FAILED to create new session");
		delete new_session;
	}
}


class ProtocolBuffersClient {
public:
	ProtocolBuffersClient(const string& hostname, 
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
		DEBUG_PRINTLN("connecting %s:%d", hostname.c_str(), port);
		socket_.connect(*iterator, error);
		if (error) {
			ERROR_PRINTLN("FAILED TO CONNECT SERVER: %s", 
				      hostname.c_str());
		} else {
			DEBUG_PRINTLN("Connected...");
		}
	}


	bool send(Serializable& data) {
		string serialized_data;
		data.serialize(serialized_data);
		int size = data.getSerializedSize();
		DEBUG_PRINTLN("writing size: <%d>", size);
		// convert to long for network stream
		size = htonl(size);
		const char * px = reinterpret_cast<const char*>(&size);
		
		DEBUG_PRINTLN("writing size: <%s><%d>", px, size);
		boost::asio::write(
			socket_,
			boost::asio::buffer(px, sizeof(int)),
			boost::asio::transfer_all()
			);

		DEBUG_PRINTLN("writing data stream: <%s>", 
			      serialized_data.c_str());
		
		boost::asio::write(
			socket_, 
			boost::asio::buffer(serialized_data, size),
			boost::asio::transfer_all()
			);

		DEBUG_PRINTLN("sending completed");
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
			ERROR_PRINTLN("failed to recieve the data");
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
			ERROR_PRINTLN("failed to recieve the data");
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
