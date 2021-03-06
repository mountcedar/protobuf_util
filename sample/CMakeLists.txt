# -*- CMakeLists.txt -*-

if( APPLE )
  ##
  ## homebrewでboostがインストールされている前提の設定です
  ##

  #set (CMAKE_CXX_FLAGS "-DSITT -DDARWIN")
  set (CMAKE_OSX_ARCHITECTURES "x86_64;")
  #set (CMAKE_OSX_ARCHITECTURES "i386;")
  set(CMAKE_BUILD_TYPE Debug)

  INCLUDE_DIRECTORIES (
     "${PROJECT_SOURCE_DIR}/include"
    "/usr/local/include"
    )

  ## LDFLAGSの設定
  LINK_DIRECTORIES (
     /usr/local/lib
     )

elseif ( UNIX )
  #SET (CMAKE_CXX_FLAGS "-DSITT")
  set(CMAKE_BUILD_TYPE Debug)

  INCLUDE_DIRECTORIES (
     "${PROJECT_SOURCE_DIR}/include"
    ${Boost_INCLUDE_DIR}
    "/usr/local/include"
    )

  ## LDFLAGSの設定
  LINK_DIRECTORIES (
     "${PROJECT_SOURCE_DIR}/src"
     "${PROJECT_SOURCE_DIR}/lib"
     "/usr/local/lib"
     )

elseif ( WIN32 )

  ## to build with windows, you have to specify the location of headers
  ## for protobuf and boost. please change the following path based on
  ## your local environment

  INCLUDE_DIRECTORIES (
    "${PROJECT_SOURCE_DIR}/include"
    "C:/Program Files (x86)/boost/boost_1_51"
    "C:/protobuf-2.4.1/src"
    )

  ## to build with windows, you have to copy all the necessary libs into
  ## the visual studio libs/ directory. The necessary libs are indicated
  ## in TARGET_LINK_LIBRARIES

  LINK_DIRECTORIES (
     "${PROJECT_SOURCE_DIR}/src"
     "${PROJECT_SOURCE_DIR}/lib"
#     "C:/protobuf-2.4.1/vsprojects/Debug"
     )

endif ()

find_package (Boost)

if (Boost_FOUND)

   INCLUDE_DIRECTORIES (
    ${Boost_INCLUDE_DIR}
    )
    ADD_DEFINITIONS( "-DHAS_BOOST" )

endif()

# プログラムの追加
add_executable(
	test_comm_server
	test_comm_server.cpp
	comm.pb.cc
	)

add_executable(
  test_comm_client
  test_comm_client.cpp
  comm.pb.cc
  )

add_executable(
  test_comm
  test_comm.cpp
  comm.pb.cc
  )

add_executable(
  async_echo_server
  async_echo_server.cpp
  )

add_executable(
  echo_client
  echo_client.cpp
)


# add_executable(
# 	test_sensor_comm
# 	test_sensor_comm.cpp
# 	sensor_comm.pb.cc
# 	)


if ( APPLE )

  TARGET_LINK_LIBRARIES (test_comm_server
    	${Boost_LIBRARIES}
    	boost_system-mt
    	boost_date_time-mt
    	boost_thread-mt
    	protobuf
    	stdc++)

  TARGET_LINK_LIBRARIES (test_comm_client
      ${Boost_LIBRARIES}
      boost_system-mt
      boost_date_time-mt
      boost_thread-mt
      protobuf
      stdc++)

  TARGET_LINK_LIBRARIES (test_comm
      ${Boost_LIBRARIES}
      boost_system-mt
      boost_date_time-mt
      boost_thread-mt
      protobuf
      stdc++
      )

  TARGET_LINK_LIBRARIES (async_echo_server
      ${Boost_LIBRARIES}
      boost_system-mt
      boost_date_time-mt
      boost_thread-mt
      stdc++
      )

  TARGET_LINK_LIBRARIES (echo_client
      ${Boost_LIBRARIES}
      boost_system-mt
      boost_date_time-mt
      boost_thread-mt
      stdc++
      )

elseif ( UNIX )

  TARGET_LINK_LIBRARIES (test_comm_server
    ${Boost_LIBRARIES}
  	boost_system
  	boost_date_time
  	boost_thread
  	protobuf
  	stdc++
  )

   TARGET_LINK_LIBRARIES (test_comm_client
    ${Boost_LIBRARIES}
    boost_system
    boost_date_time
    boost_thread
    protobuf
    stdc++
  )

  TARGET_LINK_LIBRARIES (test_comm
    ${Boost_LIBRARIES}
    boost_system
    boost_date_time
    boost_thread
    protobuf
    stdc++
  )

  TARGET_LINK_LIBRARIES (async_echo_server
      ${Boost_LIBRARIES}
      boost_system
      boost_date_time
      boost_thread
      stdc++
      )

  TARGET_LINK_LIBRARIES (echo_client
      ${Boost_LIBRARIES}
      boost_system
      boost_date_time
      boost_thread
      pthread
      stdc++
      )


 #   TARGET_LINK_LIBRARIES (test_sensor_comm
 #   	${Boost_LIBRARIES}
	# boost_system-mt
	# boost_date_time-mt
	# boost_thread-mt
	# protobuf
	# stdc++)

elseif ( WIN32 )

   #set(CMAKE_CXX_FLAGS "-D_DEBUG")

   ##
   ## vcのバージョンは古い可能性があるので、確認して適宜書き換えてください
   ##

   TARGET_LINK_LIBRARIES (test_comm_server
	libboost_system-vc100-mt-gd-1_51
	libboost_date_time-vc100-mt-gd-1_51
	libboost_thread-vc100-mt-gd-1_51
	libprotobuf
	)

   TARGET_LINK_LIBRARIES (test_comm_client
  libboost_system-vc100-mt-gd-1_51
  libboost_date_time-vc100-mt-gd-1_51
  libboost_thread-vc100-mt-gd-1_51
  libprotobuf
  )

   TARGET_LINK_LIBRARIES (test_comm
  libboost_system-vc100-mt-gd-1_51
  libboost_date_time-vc100-mt-gd-1_51
  libboost_thread-vc100-mt-gd-1_51
  libprotobuf
  )

 #   TARGET_LINK_LIBRARIES (test_sensor_comm
	# libboost_system-vc100-mt-gd-1_51
	# libboost_date_time-vc100-mt-gd-1_51
	# libboost_thread-vc100-mt-gd-1_51
	# libprotobuf
	# )

endif ()


# クリーン対象の追加
SET_DIRECTORY_PROPERTIES(
  PROPERTIES
  ADDITIONAL_MAKE_CLEAN_FILES
  "*~"
  )
