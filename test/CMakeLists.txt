# for each "test/x.cpp", generate target "x"
file(GLOB_RECURSE all_tests *.cc)
foreach(v ${all_tests})
    string(REGEX MATCH "test/.*" relative_path ${v})
    # message(${relative_path})
    string(REGEX REPLACE "test/" "" target_name ${relative_path})
    string(REGEX REPLACE ".cc" "" target_name ${target_name})

    add_executable(${target_name} ${v})
    message(${PROJECT_SOURCE_DIR}/ejsonlib)
    target_link_libraries(${target_name} PUBLIC ejson)
    target_link_directories(${target_name} PUBLIC ${PROJECT_SOURCE_DIR}/ejsonlib)
endforeach()
