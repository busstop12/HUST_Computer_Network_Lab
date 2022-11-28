aux_source_directory(. src)
ADD_EXECUTABLE(TCP ${src})
TARGET_LINK_LIBRARIES(TCP  ${NETSIM_LIB})