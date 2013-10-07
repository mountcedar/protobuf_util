libprotobufutil

はじめに
------------

このライブラリは、プロトコルバッファでパックされたバイナリをTCP/IP経由でやりとりするためのサーバ、クライアントを構成する機能を提供します。

protobuf, boost::asioについては以下のリンクを参照のこと。

* protobuf: https://code.google.com/p/protobuf/
* boost::asio: http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio.html

ライブラリの構成
----------------------

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

プロジェクトの中のファイル構造は上記のようになっており、存在するソースは、このライブラリの使い方を示すサンプルのみになっています。

</pre>

ビルド方法
---------------

サンプルは、以下のようにビルドしてください。

<pre>
$ mkdir build
$ cd build
$ cmake ..
$ make
</pre>

使い方
--------

使い方は以下のファイルを参照してください。

* sample/test_comm.cpp

次回のリリース以降、より詳細な使い方についてまとめていきます。

