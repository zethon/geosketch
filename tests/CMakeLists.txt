function(create_test testname SOURCEFILES LINKEDLIBRARIES)

    add_executable(test_${testname}
        main.cpp
        test_${testname}.cpp
        ${SOURCEFILES}
    )

    target_link_libraries(test_${testname}
        PUBLIC
            ${CONAN_LIBS}
            ${CONAN_LIBS_BOOST}
            ${LINKEDLIBRARIES}
    )

    add_test(NAME test_${testname}
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_${testname}
    )

endfunction(create_test)

set(REGION_SOURCE_FILES
    ../src/Data/RegionDatabase.h
    ../src/Data/RegionDatabase.cpp
    ../src/Data/Regions.h
    ../src/Data/Regions.cpp
)

create_test("Regions" "${REGION_SOURCE_FILES}" "${REGION_LIB_FILES}")