aux_source_directory(. src)
ADD_EXECUTABLE(SR ${src})
TARGET_LINK_LIBRARIES(SR  ${NETSIM_LIB})