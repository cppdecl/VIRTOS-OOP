cmake_minimum_required(VERSION 3.2.0)

add_definitions(-D__CMAKE_ROOT_DIRECTORY__="${CMAKE_CURRENT_SOURCE_DIR}/..")

add_executable(${PROJECT_NAME} 
    Main.cpp
    Core/System.cpp
    Core/Filesystem/Filesystem.cpp
    Core/ProgramManager/ProgramManager.cpp
    Core/Shell/Shell.cpp
    Core/UserAccountControl/UserAccountControl.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
)

target_include_directories(${PROJECT_NAME} PUBLIC .)