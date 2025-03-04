
add_subdirectory(c)
add_subdirectory(cpp)
if(${CMAKE_GENERATOR} MATCHES "Visual Studio.*")
	add_subdirectory(csharp)
endif()
add_subdirectory(python)
