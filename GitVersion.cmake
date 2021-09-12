#
#
# Generate a source file containing version information generated
# from git tags & commits.
#
#

execute_process(COMMAND git.exe describe --always --tags --long --dirty OUTPUT_VARIABLE GVERSION)
string(REPLACE "\r\n" "" GVERSION ${GVERSION})
string(REPLACE "\n" "" GVERSION ${GVERSION})

string(FIND ${GVERSION} - index)
math(EXPR index "${index}-1" OUTPUT_FORMAT DECIMAL)
string(SUBSTRING ${GVERSION} 1 ${index} VERSION)
math(EXPR index "${index}+2" OUTPUT_FORMAT DECIMAL)
string(SUBSTRING ${GVERSION} ${index} -1 GVERSION)

string(FIND ${GVERSION} - index)
string(SUBSTRING ${GVERSION} 0 ${index} COMMITS)
math(EXPR index "${index}+1" OUTPUT_FORMAT DECIMAL)
string(SUBSTRING ${GVERSION} ${index} -1 DESC)


string(FIND ${VERSION} . index)
string(SUBSTRING ${VERSION} 0 ${index} VERSION_MAJOR)
math(EXPR index "${index}+1" OUTPUT_FORMAT DECIMAL)
string(SUBSTRING ${VERSION} ${index} -1 VERSION)
string(FIND ${VERSION} . index)
string(SUBSTRING ${VERSION} 0 ${index} VERSION_MINOR)
math(EXPR index "${index}+1" OUTPUT_FORMAT DECIMAL)
string(SUBSTRING ${VERSION} ${index} -1 VERSION_PATCH)

string(JOIN "" VERSION_OUT 
 "extern const char LIBRARY_VERSION_STR[]\;\n"
 "const char LIBRARY_VERSION_STR[] = \n"
 "\"VERSION_MAJOR: " ${VERSION_MAJOR} "\\n\"\n"
 "\"VERSION_MINOR: " ${VERSION_MINOR} "\\n\"\n"
 "\"VERSION_PATCH: " ${VERSION_PATCH} "\\n\"\n"
 "\"VERSION_COMMITS: " ${COMMITS} "\\n\"\n"
 "\"VERSION_GIT_HASH: " ${DESC} "\\n\"\;\n" )

math(EXPR index "${CMAKE_ARGC} - 1" OUTPUT_FORMAT DECIMAL)

file(WRITE  "${CMAKE_ARGV${index}}" ${VERSION_OUT})
