
set (protocon_exe "$<TARGET_FILE:protocon>")

add_test (NAME Verif4_Sync_OrientRing_test COMMAND ${Fildesh_EXECUTABLE}
  -alias "protocon=${protocon_exe}"
  -f "${CMAKE_CURRENT_SOURCE_DIR}/Verif4_Sync_OrientRing_test.fildesh"
  "--input_file=${PROJECT_SOURCE_DIR}/examplesoln/OrientRing.prot"
)

add_test (NAME Verif4_OrientRingOdd_failure_test COMMAND ${Fildesh_EXECUTABLE}
  -alias "protocon=${protocon_exe}"
  -f "${CMAKE_CURRENT_SOURCE_DIR}/Verif4_OrientRingOdd_failure_test.fildesh"
  "--input_file=${PROJECT_SOURCE_DIR}/examplesoln/OrientRingOdd.prot"
)

## PLA file output.
add_test (NAME PLA_MatchRingThreeState_test COMMAND ${Fildesh_EXECUTABLE}
  -alias "protocon=${protocon_exe}"
  -f "${CMAKE_CURRENT_SOURCE_DIR}/PLA_MatchRingThreeState_test.fildesh"
  "--expect_file=${CMAKE_CURRENT_SOURCE_DIR}/MatchRingThreeState.pla"
  "--input_file=${PROJECT_SOURCE_DIR}/examplesoln/MatchRingThreeState.prot"
)

## Protocon file output minimized by Espresso (which speaks in PLA format).
add_executable(Espresso_MatchRingThreeState
  "Espresso_MatchRingThreeState.cc"
)
target_link_libraries(Espresso_MatchRingThreeState ${Fildesh_LIBRARIES})
add_test(NAME Espresso_MatchRingThreeState_test COMMAND ${Fildesh_EXECUTABLE}
  -alias "protocon=${protocon_exe}"
  -f "${CMAKE_CURRENT_SOURCE_DIR}/Espresso_MatchRingThreeState_test.fildesh"
  "--espresso=$<TARGET_FILE:Espresso_MatchRingThreeState>"
  "--input_file=${PROJECT_SOURCE_DIR}/examplesoln/MatchRingThreeState.prot"
)

## Promela file output.
add_test (NAME Promela_OrientDaisy_test COMMAND ${Fildesh_EXECUTABLE}
  -alias "protocon=${protocon_exe}"
  -f "${CMAKE_CURRENT_SOURCE_DIR}/Promela_OrientDaisy_test.fildesh"
  "--expect_file=${CMAKE_CURRENT_SOURCE_DIR}/OrientDaisy.pml"
  "--input_file=${PROJECT_SOURCE_DIR}/examplesoln/OrientDaisy.prot"
)

## UDP file output.
add_test (NAME UDP_TokenRingRand_test COMMAND ${Fildesh_EXECUTABLE}
  -alias "protocon=${protocon_exe}"
  -f "${CMAKE_CURRENT_SOURCE_DIR}/UDP_TokenRingRand_test.fildesh"
  "--expect_file=${CMAKE_CURRENT_SOURCE_DIR}/TokenRingRand.h"
  "--input_file=${PROJECT_SOURCE_DIR}/examplesoln/TokenRingRand.prot"
)
