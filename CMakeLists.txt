cmake_minimum_required(VERSION 3.1)

project(Algorithm)

function(AllFilesLoad CurPath CurGroup)
file(GLOB Cur_Files ${CurPath}/*.*)
source_group(${CurGroup}  FILES ${Cur_Files})
file(GLOB files ${CurPath}/*)	
foreach(var ${files})
if(IS_DIRECTORY ${var})
file(RELATIVE_PATH RelativePath ${CurPath} ${var})
set(childGroup ${CurGroup}\\${RelativePath})
AllFilesLoad(${var} ${childGroup})
endif()
endforeach()	
endfunction(AllFilesLoad)

file(GLOB_RECURSE AllFile *.*)

file(RELATIVE_PATH RelativePath ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR})
	
add_executable(${PROJECT_NAME} ${AllFile})

AllFilesLoad(${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_NAME})