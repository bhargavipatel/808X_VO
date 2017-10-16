add_executable(shell-app main.cpp vo.cpp allmod.cpp getdata.cpp)
# Link the OpenCV_LIBS with binary
target_link_libraries( shell-app ${OpenCV_LIBS} )

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)
