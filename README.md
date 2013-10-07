libprotobufutil

##はじめに

このライブラリは、プロトコルバッファでパックされたバイナリをTCP/IP経由でやりとりするためのサーバ、クライアントを構成する機能を提供します。

protobuf, boost::asioについては以下のリンクを参照のこと。

* protobuf: https://code.google.com/p/protobuf/
* boost::asio: http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio.html

##ライブラリの構成

このライブラリは、ヘッダファイルのみからなるライブラリです。従って、静的・動的ライブラリをビルドする必要はありません。include/protobuf_utilsの中身を必要なプロジェクトのヘッダファイルパスに追加することで使うことができます。

<pre>
├── CMakeLists.txt
├── Doxyfile
├── config
├── include
│   └── protobuf_utils
│       ├── debug.h
│       └── utils.hpp
├── mainpage.dox
└── sample
</pre>

プロジェクトの中のファイル構造は上記のようになっており、存在するソースは、このライブラリの使い方を示すサンプルのみになっています。

</pre>

##ビルド方法

サンプルは、以下のようにビルドしてください。

<pre>
$ mkdir build
$ cd build
$ cmake ..
$ make
</pre>

ビルドが成功すると、sampleディレクトリにtest_commというバイナリが生成されます。実行すると、サーバクライアントの通信サンプルが実行されます。

##使い方

使い方は以下のファイルを参照してください。

* sample/test_comm.cpp

次回のリリース以降、より詳細な使い方についてまとめていきます。

### 独自のprotoファイルとライブラリの連結

protocでprotoファイルから生成されるソースの内容はそれぞれ違います。このライブラリでは、それら異なる実装のファイルの差異を吸収するために以下の3つのラッパのためのインタフェースを用意しています。
 
* Serializable: シリアル化するときの処理を統一するためのインタフェースクラスです。
* DataBuilder：データクラスを生成するときの処理を統一するためのインタフェース
* Recievable：データ受信したときの処理を統一するためのインタフェース

これらのインタフェースの実装は別々のクラスで実装しても構わないですし、一つのクラスにまとめても構いません。クラスがむやみに多くなるのも良くないため、たいていはサンプルのようにまとめて実装するほうが効果的だと思われます。

```test_comm.cpp
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
```

### ProtocolBufferServerの作成

プロトコルバッファの送受信のためのサーバを立ち上げます。サーバの生成はProtocolBufferServerクラスの静的メソッドで行います。これは、サーバが完全な状態以外で生成されることを避けるための仕組みです。

```test_comm.cpp
	MessageImpl msg_impl;
	boost::system::error_code error;
	boost::shared_ptr<ProtocolBufferServer> server_ptr = ProtocolBufferServer::create(1111, msg_impl, error);
	if (error) {
		ERROR_PRINTLN("failed to activate server.");
		return -1;
	}
	ProtocolBufferServer& server(*server_ptr.get());	
	server.register_reciever(&msg_impl);
```

ここでは、smart_pointerを用いて、メモリの管理を行っています。

### ProtocolBufferClientの作成

プロトコルバッファの送受信のためのクライアントを立ち上げます。

```main.cpp
	MessageImpl msg_impl;
	...
	Message msg;
	msg.set_id(10);
	msg.set_type(Message::ACK);
	msg.add_data()->set_status("hogehoge");

	MessageImpl impl(msg);
	impl.print();

	ProtocolBufferClient client("localhost", 1111, msg_impl);
	…
	client.send(impl);
```

MessageImplが複数箇所で生成されているのは

* msg_impl: レシーバとして使用（Recievable)
* impl: データが内容された構造体として使用（Serializable)

と使う意図が異なるためである。




