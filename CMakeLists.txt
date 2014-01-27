# -*- CMakeLists.txt -*-
cmake_minimum_required(VERSION 2.6)

project (protobufutil)
set(PROJECT_VERSION "0.0.1")
include_directories ("${PROJECT_SOURCE_DIR}/include")

# バージョン番号
SET(serial "0.0.1")

# 共有ライブラリのバージョン番号
SET(soserial "0")

add_subdirectory (sample)
# add_subdirectory (sample/boost.asio)

# クリーン対象の追加
SET_DIRECTORY_PROPERTIES(
  PROPERTIES
  ADDITIONAL_MAKE_CLEAN_FILES "*~"
  )


if ( WIN32 )

   MESSAGE("no custom target for windows architecture")

else ( WIN32 )

   SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "*~") 

   # make distの実装(svn版)
   set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
   set(DISTRIBUTE_FILE_NAME ${ARCHIVE_NAME}.tar.gz)

   add_custom_target(dist
     COMMAND svn export . ${ARCHIVE_NAME}\;
     tar czf ${DISTRIBUTE_FILE_NAME} ${ARCHIVE_NAME}\;
     rm -rf ${ARCHIVE_NAME}; 
     )

   # make distcleanの簡易実装
   add_custom_target(distclean
     COMMAND for m in `find . -name \"Makefile\"`\; do rm $$m\; done\;
     for d in `find . -name \"CMakeFiles\"`\; do rm -rf $$d\; done\;
     for d in `find . -name \"cmake_install.cmake\"`\; do rm -rf $$d\; done\;
     rm -rf CMakeFiles\; 
     rm -f cmake_install.cmake\; 
     rm -f CMakeCache.txt\;
     rm -f ${DISTRIBUTE_FILE_NAME}\;
     )  

   # clean temporary file *~
   #add_custom_target(temp_clean
   #  COMMAND rm -rf *~
   #  )

    #add_dependencies (clean temp_clean)

endif ()

# 共有ライブラリ用ヘッダーファイルのインストール指定
#install (FILES "${PROJECT_SOURCE_DIR}/include/*.h" DESTINATION include)
