set(TARGET chapter_02)

file(GLOB SOURCES *.cpp *.h *.txt)

include_directories(
    ${CMAKE_SOURCE_DIR}
)

add_executable(${TARGET} ${SOURCES})

if (NOT DEFINED ENV{SYSTEMC_HOME})
    target_link_libraries(${TARGET} systemc-cci::systemc-cci systemc-scv::systemc-scv SystemC::systemc)
else()
    target_link_libraries(${TARGET} PUBLIC ${SCV_LIBRARIES} ${SystemC_LIBRARIES})
endif()

set_property(TARGET ${TARGET} PROPERTY FOLDER "SystemC_Intro")
