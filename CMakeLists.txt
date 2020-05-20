cmake_minimum_required(VERSION 3.16)

project(audio_out_example VERSION 0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(RESOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/resources)

configure_file(
${CMAKE_CURRENT_LIST_DIR}/config.h.in
${CMAKE_CURRENT_BINARY_DIR}/config.h
)

find_package(Qt5 COMPONENTS Core Quick Multimedia REQUIRED)

add_executable(
	${PROJECT_NAME}
	src/main.cpp
	src/controller.cpp
	src/play_sound.cpp
	src/player.cpp
	src/main.qrc
)

target_compile_definitions(
	${PROJECT_NAME}
	PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QMLLDebug>
)
target_link_libraries(
	${PROJECT_NAME}
  	PRIVATE Qt5::Core Qt5::Quick Qt5::Multimedia
)
