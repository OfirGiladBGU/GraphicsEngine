project(glad)

add_library(glad src/glad.c)
target_include_directories(glad PUBLIC include)

if(NOT WIN32)
  target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})
endif()

# Generate position independent code
set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)
