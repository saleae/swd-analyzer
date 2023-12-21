# Get repo clean status from git
execute_process(
    COMMAND git diff --quiet --exit-code
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE GIT_REPO_CHANGED
)
#message("Info: Git repo changed is ${GIT_REPO_CHANGED} (0 - Not changed, 1 - Changed)")

if(${GIT_REPO_CHANGED} EQUAL 0)
    # Get hash of last commit
    execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    # Since there are changes in repo detected hash will be zeroed
    set(GIT_COMMIT_HASH "0000000")
endif()
#message("Info: Git hash is ${GIT_COMMIT_HASH}")

execute_process(
    COMMAND git rev-list --all --count
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_REV_COUNT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
#message("Info: Git revisions count is ${GIT_REV_COUNT}")
math(EXPR GIT_BUILD "${GIT_REV_COUNT}+${GIT_REPO_CHANGED}")
#message("Info: Build #${GIT_BUILD}")

# Hardcode version parts
set(PROJECT_VERSION_MAJOR 2)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
math(EXPR PROJECT_VERSION_BUILD "${GIT_REV_COUNT}+${GIT_REPO_CHANGED}")
message("Info: Version ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_BUILD} ... ${GIT_COMMIT_HASH}")

message("Info: Updating ${CMAKE_SOURCE_DIR}/../src/VersionInfo.cpp")
file(WRITE ${CMAKE_SOURCE_DIR}/../src/VersionInfo.cpp "#include \"VersionInfo.h\"\n\nconst struct VersionInfo VersionInfo =\n{\n  ${PROJECT_VERSION_MAJOR}u,\n  ${PROJECT_VERSION_MINOR}u,\n  ${PROJECT_VERSION_PATCH}u,\n  ${PROJECT_VERSION_BUILD}u,\n  __DATE__,\n  __TIME__,\n  \"${GIT_COMMIT_HASH}\"\n};\n")
