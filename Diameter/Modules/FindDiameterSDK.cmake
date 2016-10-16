
macro(subdirlist result curdir)
  file(GLOB children ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach(child ${children})
  set(${result} ${dirlist})
endmacro(subdirlist result curdir)

function(diameter_sdk_generate SRCS INCLUDES INCLUDE_DIR dictionaryFile)
    
    set(options GENERATE_BASE_AVPS)
    
    cmake_parse_arguments(diameter_sdk_generate "${options}" "" "" ${ARGN})
    
    set(GEN_ARG_EXT "-n")
    
    if (${diameter_sdk_generate_GENERATE_BASE_AVPS})
        set(GEN_ARG_EXT "")
    endif (${diameter_sdk_generate_GENERATE_BASE_AVPS})


    set(${SRCS})
    set(${INCLUDES})

    MESSAGE(STATUS "DiameterSDK: Generating from ${dictionaryFile}")
    get_filename_component(ABS_dictionaryFile ${dictionaryFile} ABSOLUTE)
    get_filename_component(WE_dictionaryFile ${dictionaryFile} NAME_WE)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${WE_dictionaryFile})
    set(GEN_ARGS -f ${ABS_dictionaryFile}
                -o ${CMAKE_CURRENT_BINARY_DIR}/${WE_dictionaryFile}
                -t ${DIAMETER_SDK_TEMPLATE_DIR} ${GEN_ARG_EXT})

    set(INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${WE_dictionaryFile})

    execute_process(COMMAND ${DIAMETER_SDK_GEN_EXE} ${GEN_ARGS}
            RESULT_VARIABLE gen_result
            OUTPUT_VARIABLE gen_out
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    MESSAGE(STATUS "Executing ${DIAMETER_SDK_GEN_EXE} ${GEN_ARGS}")

    file(GLOB dictSrcs ${CMAKE_CURRENT_BINARY_DIR}/${WE_dictionaryFile}/*.cpp)
    foreach(dictSrc ${dictSrcs})
      list(APPEND ${SRCS} ${dictSrc})
    endforeach(dictSrc ${dictSrcs})

    file(GLOB dictHdrs ${CMAKE_CURRENT_BINARY_DIR}/${WE_dictionaryFile}/*.h)
    foreach(dictHdr ${dictHdrs})
      list(APPEND ${INCLUDES} ${dictHdr})
    endforeach(dictHdr ${dictHdrs})
    
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/${WE_dictionaryFile})

    set(${SRCS} ${${SRCS}} PARENT_SCOPE)
    set(${INCLUDES} ${${INCLUDES}} PARENT_SCOPE)
    set(${INCLUDE_DIR} ${${INCLUDE_DIR}} PARENT_SCOPE)
    
endfunction(diameter_sdk_generate SRCS INCLUDES)

function(find_diameter_dictionary dictionary_name)
    find_path(${dictionary_name}_INCLUDES ${dictionary_name}_Dictionary.h
                PATHS ${DIAMETER_DICTIONARIES_ROOT}/${dictionary_name}Dictionary/include)
    find_library(${dictionary_name}_LIBRARIES ${dictionary_name}Dictionary
                lib${dictionary_name}Dictionary
                PATHS ${DIAMETER_DICTIONARIES_ROOT}/${dictionary_name}Dictionary/lib)
endfunction(find_diameter_dictionary dictionary_name include_dir lib)

if(NOT DEFINED DIAMETER_SDK_ROOT)
    message(SEND_ERROR "You must specify a DIAMETER_SDK_ROOT")
endif(NOT DEFINED DIAMETER_SDK_ROOT)

find_program(DIAMETER_SDK_GEN_EXE
            NAMES DiameterGenerator
            DOC "Diameter Dictionary Generator Executable"
            PATHS ${DIAMETER_SDK_ROOT}/bin)

find_path(DIAMETER_SDK_ROOT DiameterEvents/DiameterEvent.h
            PATHS ${DIAMETER_SDK_ROOT}/sdk)

find_library(DIAMETER_EVENTS_LIB NAMES DiameterEvents libDiameterEvents
            PATHS ${DIAMETER_SDK_ROOT}/lib)

find_library(DIAMETER_DICTIONARY_LIB NAMES DiameterDictionary libDiameterDictionary
            PATHS ${DIAMETER_SDK_ROOT}/lib)

find_library(DIAMETER_SERIALISER_LIB NAMES DiameterSerialiser libDiameterSerialiser
            PATHS ${DIAMETER_SDK_ROOT}/lib)

subdirlist(DIAMETER_SDK_INCLUDE_DIRS ${DIAMETER_SDK_ROOT}/includes)

set(DIAMETER_SDK_LIBRARIES ${DIAMETER_EVENTS_LIB} ${DIAMETER_DICTIONARY_LIB} ${DIAMETER_SERIALISER_LIB})

set(DIAMETER_SDK_TEMPLATE_DIR ${DIAMETER_SDK_ROOT}/bin)

