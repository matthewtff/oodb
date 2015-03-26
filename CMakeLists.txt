cmake_minimum_required ( VERSION 2.6 )
project (oodb)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR
    ${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR
    ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")

  SET (warnings "-Wall -Wextra -Woverloaded-virtual -Werror")

  SET (common_flags "-std=c++0x -fPIC")

  if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR
      ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    SET (clang_flags "-stdlib=libc++")
  endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR
         ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")

  SET (CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} ${warnings} ${common_flags} ${clang_flags}"
  )

  SET (CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} ${warnings} ${common_flags} ${clang_flags} -g -ggdb"
  )

  SET (CMAKE_CXX_FLAGS_RELEASE
    "${CMAKE_CXX_FLAGS_RELEASE} ${warnings} ${common_flags} ${clang_flags} -O2"
  )

  set (oodb_platform_src
    src/file_driver_posix.cc
  )

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

  set (oodb_platform_src
    src/file_driver_win.cc
  )

endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR
       ${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR
       ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

include_directories (
  /opt/local/include
)

endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

include_directories (
  ${CMAKE_SOURCE_DIR}/src
)
set (oodb_src
  ${oodb_platform_src}
  src/oodb.cc
)

add_library ( oodb ${oodb_src} )
