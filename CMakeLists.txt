cmake_minimum_required ( VERSION 2.6 )
project (oodb)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")

	SET (warnings "-Wall -Wextra -Woverloaded-virtual -Werror")

	SET (common_flags "-std=c++0x -stdlib=libc++ -fPIC")

	SET (CMAKE_CXX_FLAGS
		"${CMAKE_CXX_FLAGS} ${warnings} ${common_flags}"
	)

	SET (CMAKE_CXX_FLAGS_DEBUG
		"${CMAKE_CXX_FLAGS_DEBUG} ${warnings} ${common_flags} -g -ggdb"
	)

	SET (CMAKE_CXX_FLAGS_RELEASE
		"${CMAKE_CXX_FLAGS_RELEASE} ${warnings} ${common_flags} -O2"
	)

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")

set ( oodb_src
	src/file_driver.cc
	src/oodb.cc
)

add_library ( oodb ${oodb_src} )
