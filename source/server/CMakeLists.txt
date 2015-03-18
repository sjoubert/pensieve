find_package(microhttpd REQUIRED)

file(GLOB source_files "*.?pp")
add_executable(pensieved ${source_files})
