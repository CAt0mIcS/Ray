add_custom_command(TARGET "ZealGUILayer" POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/res/Cube.obj" "${ZealGUILayer_OUTPUTPATH}"
)


add_custom_command(TARGET "ZealGUILayer" POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/res/Cube.mtl" "${ZealGUILayer_OUTPUTPATH}"
)