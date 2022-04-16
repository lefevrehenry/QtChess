cmake_minimum_required(VERSION 3.16)
project(prog)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(HEADER_FILES
    src/QtChess/ChessBoard.h
)

set(SOURCE_FILES
    main.cpp
    src/QtChess/ChessBoard.cpp
    src/QtChess/StockfishEngine.cpp
)

set(MOCCABLE_FILES
    src/QtChess/StockfishEngine.h
)

set(QRC_FILES
    # data/qml.qrc
)

find_package(Qt5 COMPONENTS Core REQUIRED)#Gui Widgets Quick Qml REQUIRED)

qt5_wrap_cpp(MOCCED_FILES ${MOCCABLE_FILES})
# qt_add_resources(RESOURCE_FILES ${QRC_FILES})

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${MOCCABLE_FILES} ${MOCCED_FILES} ${QRC_FILES} ${RESOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} Qt5::Core)# Qt5::Gui Qt5::Widgets Qt5::Quick Qt5::Qml)
