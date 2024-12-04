cmake_minimum_required(VERSION 3.2.0)

add_definitions(-D__CMAKE_ROOT_DIRECTORY__="${CMAKE_CURRENT_SOURCE_DIR}/..")

add_executable(${PROJECT_NAME} 
    Main.cpp
    Core/System.cpp
    Core/Shell/Shell.cpp
    Core/UserAccountControl/UserAccountControl.cpp
    Core/ProgramManager/ProgramManager.cpp
    Core/ProgramManager/Programs/ListFilesProgram/ListFilesProgram.cpp
    Core/ProgramManager/Programs/ChangeDirProgram/ChangeDirProgram.cpp
    Core/ProgramManager/Programs/CopyProgram/CopyProgram.cpp
    Core/ProgramManager/Programs/MoveProgram/MoveProgram.cpp
    Core/ProgramManager/Programs/MakeDirProgram/MakeDirProgram.cpp
    Core/ProgramManager/Programs/RemoveProgram/RemoveProgram.cpp
    Core/ProgramManager/Programs/NanoProgram/NanoProgram.cpp
    Core/ProgramManager/Programs/SnakeProgram/SnakeProgram.cpp
    Core/ProgramManager/Programs/ClockProgram/ClockProgram.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
)

target_include_directories(${PROJECT_NAME} PUBLIC .)