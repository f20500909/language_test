set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build/lib)

aux_source_directory(. tobelib)
add_library(luastatic STATIC ${tobelib})

set_target_properties(luastatic PROPERTIES OUTPUT_NAME "lua")