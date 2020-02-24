include_directories(../lib)

enable_testing()

# fo_apf
add_executable(fo_apf_test fo_apf_test.cpp)
target_link_libraries(fo_apf_test gtest_main)
add_test(NAME fo_apf_test COMMAND fo_apf_test)

# fo_hpf
add_executable(fo_hpf_test fo_hpf_test.cpp)
target_link_libraries(fo_hpf_test gtest_main)
add_test(NAME fo_hpf_test COMMAND fo_hpf_test)

# fo_lpf
add_executable(fo_lpf_test fo_lpf_test.cpp)
target_link_libraries(fo_lpf_test gtest_main)
add_test(NAME fo_lpf_test COMMAND fo_lpf_test)

# fo_shelving_high
add_executable(fo_shelving_high_test fo_shelving_high_test.cpp)
target_link_libraries(fo_shelving_high_test gtest_main)
add_test(NAME fo_shelving_high_test COMMAND fo_shelving_high_test)

# fo_shelving_low
add_executable(fo_shelving_low_test fo_shelving_low_test.cpp)
target_link_libraries(fo_shelving_low_test gtest_main)
add_test(NAME fo_shelving_low_test COMMAND fo_shelving_low_test)

# so_apf
add_executable(so_apf_test so_apf_test.cpp)
target_link_libraries(so_apf_test gtest_main)
add_test(NAME so_apf_test COMMAND so_apf_test)

# so_hpf
add_executable(so_hpf_test so_hpf_test.cpp)
target_link_libraries(so_hpf_test gtest_main)
add_test(NAME so_hpf_test COMMAND so_hpf_test)

# so_lpf
add_executable(so_lpf_test so_lpf_test.cpp)
target_link_libraries(so_lpf_test gtest_main)
add_test(NAME so_lpf_test COMMAND so_lpf_test)

# so_bpf
add_executable(so_bpf_test so_bpf_test.cpp)
target_link_libraries(so_bpf_test gtest_main)
add_test(NAME so_bpf_test COMMAND so_bpf_test)

# so_bsf
add_executable(so_bsf_test so_bsf_test.cpp)
target_link_libraries(so_bsf_test gtest_main)
add_test(NAME so_bsf_test COMMAND so_bsf_test)

# so_butterworth_bpf
add_executable(so_butterworth_bpf_test so_butterworth_bpf_test.cpp)
target_link_libraries(so_butterworth_bpf_test gtest_main)
add_test(NAME so_butterworth_bpf_test COMMAND so_butterworth_bpf_test)

# so_butterworth_bsf
add_executable(so_butterworth_bsf_test so_butterworth_bsf_test.cpp)
target_link_libraries(so_butterworth_bsf_test gtest_main)
add_test(NAME so_butterworth_bsf_test COMMAND so_butterworth_bsf_test)

# so_butterworth_hpf
add_executable(so_butterworth_hpf_test so_butterworth_hpf_test.cpp)
target_link_libraries(so_butterworth_hpf_test gtest_main)
add_test(NAME so_butterworth_hpf_test COMMAND so_butterworth_hpf_test)

# so_butterworth_lpf
add_executable(so_butterworth_lpf_test so_butterworth_lpf_test.cpp)
target_link_libraries(so_butterworth_lpf_test gtest_main)
add_test(NAME so_butterworth_lpf_test COMMAND so_butterworth_lpf_test)

# so_linkwitz_riley_hpf
add_executable(so_linkwitz_riley_hpf_test so_linkwitz_riley_hpf_test.cpp)
target_link_libraries(so_linkwitz_riley_hpf_test gtest_main)
add_test(NAME so_linkwitz_riley_hpf_test COMMAND so_linkwitz_riley_hpf_test)

# so_linkwitz_riley_lpf
add_executable(so_linkwitz_riley_lpf_test so_linkwitz_riley_lpf_test.cpp)
target_link_libraries(so_linkwitz_riley_lpf_test gtest_main)
add_test(NAME so_linkwitz_riley_lpf_test COMMAND so_linkwitz_riley_lpf_test)

# so_parametric_cq_boost
add_executable(so_parametric_cq_boost_test so_parametric_cq_boost_test.cpp)
target_link_libraries(so_parametric_cq_boost_test gtest_main)
add_test(NAME so_parametric_cq_boost_test COMMAND so_parametric_cq_boost_test)

# so_parametric_cq_cut
add_executable(so_parametric_cq_cut_test so_parametric_cq_cut_test.cpp)
target_link_libraries(so_parametric_cq_cut_test gtest_main)
add_test(NAME so_parametric_cq_cut_test COMMAND so_parametric_cq_cut_test)

# so_parametric_ncq
add_executable(so_parametric_ncq_test so_parametric_ncq_test.cpp)
target_link_libraries(so_parametric_ncq_test gtest_main)
add_test(NAME so_parametric_ncq_test COMMAND so_parametric_ncq_test)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS
                  fo_apf_test fo_hpf_test fo_lpf_test
                  fo_shelving_high_test fo_shelving_low_test
                  so_apf_test so_hpf_test so_lpf_test
                  so_bpf_test so_bsf_test
                  so_butterworth_bpf_test so_butterworth_bsf_test
                  so_butterworth_hpf_test so_butterworth_lpf_test
                  so_linkwitz_riley_hpf_test so_linkwitz_riley_lpf_test
                  so_parametric_cq_boost_test so_parametric_cq_cut_test
                  so_parametric_ncq_test)