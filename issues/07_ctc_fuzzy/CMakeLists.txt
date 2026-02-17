cmake_minimum_required(VERSION 3.5.20)

project(tests_CtcFuzzy)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(CODAC REQUIRED)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ./codac2_${PROJECT_NAME}.cpp)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
target_compile_options(${PROJECT_NAME} PUBLIC ${CODAC_CXX_FLAGS})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CODAC_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${CODAC_LIBRARIES})
# target_compile_options(tests_CtcRelaxed PUBLIC ${CODAC_CXX_FLAGS})
# target_include_directories(tests_CtcRelaxed SYSTEM PUBLIC ${CODAC_INCLUDE_DIRS})
# target_link_libraries(tests_CtcRelaxed PUBLIC ${CODAC_LIBRARIES})
