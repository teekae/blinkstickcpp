
if(BUILD_CLI)
    # CLI TOOL
    set(cli_tool_name blinkstick-cli)
    add_executable(${cli_tool_name} blinkstick_test.cpp)
    add_dependencies(${cli_tool_name} blinkstickcpp)
    target_link_libraries(${cli_tool_name}
        PUBLIC
            blinkstickcpp
            $<$<PLATFORM_ID:Linux>:pthread>
    )
    set_target_properties(${cli_tool_name} PROPERTIES OUPUT_NAME blinkstick)

    set_property(TARGET ${cli_tool_name} PROPERTY CXX_STANDARD 17)
endif(BUILD_CLI)