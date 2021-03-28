cmake_minimum_required( VERSION 3.17 )
project( G5games_test )

set( CMAKE_CXX_STANDARD 14 )

file(GLOB HEADER_FILES "*.h")
file(GLOB SOURCE_FILES "*.cpp")

add_executable( ${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} )