aux_source_directory(. src)
ADD_EXECUTABLE(GBN ${src})
TARGET_LINK_LIBRARIES(GBN  ${NETSIM_LIB})