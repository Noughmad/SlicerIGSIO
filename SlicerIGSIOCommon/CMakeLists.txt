project(vtkSlicerIGSIOCommon)

# --------------------------------------------------------------------------
# Sources
# --------------------------------------------------------------------------

find_package(IGSIO REQUIRED)

set(SlicerIGSIOCommon_SRCS
  vtkSlicerIGSIOCommon.cxx
  vtkSlicerIGSIOCommon.h
  vtkSlicerIGSIOLogger.cxx
  vtkSlicerIGSIOLogger.h
  )

set(SlicerIGSIOCommon_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Slicer_Libs_INCLUDE_DIRS}
  ${Slicer_Base_INCLUDE_DIRS}
  ${VTKIGSIOCOMMON_INCLUDE_DIRS}
  ${VTKSEQUENCEIO_INCLUDE_DIRS}
  ${vtkSlicerSequenceBrowserModuleMRML_INCLUDE_DIRS}
  ${vtkSlicerSequencesModuleMRML_INCLUDE_DIRS}
  ${vtkSlicerVolumesModuleLogic_INCLUDE_DIRS}
  CACHE INTERNAL "" FORCE)

# --------------------------------------------------------------------------
# Build the library
# --------------------------------------------------------------------------

set(lib_name ${PROJECT_NAME})
set(SlicerIGSIOCommon_LIBS ${SlicerIGSIOCommon_LIBS}
  ${VTK_LIBRARIES}
  MRMLCore
  vtkIGSIOCommon
  vtkSequenceIO
  vtkSlicerSequencesModuleMRML
  vtkSlicerSequenceBrowserModuleMRML
  vtkSlicerVolumesModuleLogic
  )

include_directories( ${SlicerIGSIOCommon_INCLUDE_DIRS} )
add_library(${lib_name} ${SlicerIGSIOCommon_SRCS})
target_link_libraries( ${lib_name} ${SlicerIGSIOCommon_LIBS} )

# Set loadable modules output
set_target_properties(${lib_name} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_BIN_DIR}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_LIB_DIR}"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_LIB_DIR}"
  )

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" AND NOT WIN32)
  set_target_properties(${lib_name} PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

set_property(GLOBAL APPEND PROPERTY Slicer_TARGETS ${lib_name})

# --------------------------------------------------------------------------
# Install library
# --------------------------------------------------------------------------
install(TARGETS ${lib_name}
  RUNTIME DESTINATION ${Slicer_INSTALL_QTLOADABLEMODULES_BIN_DIR} COMPONENT RuntimeLibraries
  LIBRARY DESTINATION ${Slicer_INSTALL_QTLOADABLEMODULES_LIB_DIR} COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION ${Slicer_INSTALL_QTLOADABLEMODULES_LIB_DIR} COMPONENT Development
  )

# --------------------------------------------------------------------------
# Python wrapping
# --------------------------------------------------------------------------
if(VTK_WRAP_PYTHON AND BUILD_SHARED_LIBS)

  set(Slicer_Wrapped_LIBRARIES
    )

  foreach(library ${lib_name})
    if(TARGET ${library}PythonD)
      list(APPEND Slicer_Wrapped_LIBRARIES ${library}PythonD)
    endif()
  endforeach()

  SlicerMacroPythonWrapModuleVTKLibrary(
    NAME ${PROJECT_NAME}
    SRCS ${SlicerIGSIOCommon_SRCS}
    WRAPPED_TARGET_LIBRARIES ${Slicer_Wrapped_LIBRARIES}
    RELATIVE_PYTHON_DIR "."
    )

  # Set python module logic output
  set_target_properties(${PROJECT_NAME}Python ${PROJECT_NAME}PythonD PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_BIN_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_LIB_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${Slicer_QTLOADABLEMODULES_LIB_DIR}"
    )

  # Export target
  set_property(GLOBAL APPEND PROPERTY Slicer_TARGETS ${PROJECT_NAME}Python ${PROJECT_NAME}PythonD)
endif()