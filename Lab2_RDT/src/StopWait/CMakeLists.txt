aux_source_directory(. src)
ADD_EXECUTABLE(StopWait ${src})
TARGET_LINK_LIBRARIES(StopWait  ${NETSIM_LIB})