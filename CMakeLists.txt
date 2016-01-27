cmake_minimum_required(VERSION 2.8)
project(jnp_zadanie7)

add_custom_target(example1 COMMAND make example1 -C ${jnp_zadanie7_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})

add_custom_target(example2 COMMAND make example3 -C ${jnp_zadanie7_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})

add_custom_target(example3 COMMAND make example3 -C ${jnp_zadanie7_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})
