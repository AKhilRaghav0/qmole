# This script drives creation of a Subversion repository and checks
# that CTest can update from it.

#-----------------------------------------------------------------------------
# Test in a directory next to this script.
get_filename_component(TOP "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(TOP "${TOP}/CTest UpdateSVN")
set(UPDATE_GLOBAL_ELEMENTS SVNPath)

# Include code common to all update tests.
include("/home/chris/cmake-2.8.10.2/Tests/CTestUpdateCommon.cmake")

#-----------------------------------------------------------------------------
# Report subversion tools in use.
message("Using subversion tools:")
set(SVN "/usr/bin/svn")
set(SVNADMIN "/usr/bin/svnadmin")
message(" svn      = ${SVN}")
message(" svnadmin = ${SVNADMIN}")

# Isolate svn test operations from the user configuration.
file(MAKE_DIRECTORY ${TOP}/config)
set(SVNCMD ${SVN} --config-dir ${TOP}/config)
set(SVNUSER --username "test author" --non-interactive)

# Configure for this svn version.
execute_process(
  COMMAND ${SVN} help add OUTPUT_VARIABLE help_add ERROR_VARIABLE help_add
  )
if("${help_add}" MATCHES "--depth")
  set(depth_empty "--depth=empty")
else()
  set(depth_empty "")
endif()

#-----------------------------------------------------------------------------
# Initialize the testing directory.
message("Creating test directory...")
init_testing()

#-----------------------------------------------------------------------------
# Create the repository.
message("Creating repository...")
run_child(
  COMMAND ${SVNADMIN} create --config-dir ${TOP}/config ${TOP}/repo
  )
set(REPO file:///${TOP}/repo/trunk)

#-----------------------------------------------------------------------------
# Import initial content into the repository.
message("Importing content...")
create_content(import)

# Import the content into the repository.
run_child(
  WORKING_DIRECTORY ${TOP}/import
  COMMAND ${SVNCMD} import ${SVNUSER} -m "Initial content" . "${REPO}"
  )

#-----------------------------------------------------------------------------
# Create a working tree.
message("Checking out revision 1...")
run_child(
  WORKING_DIRECTORY ${TOP}
  COMMAND ${SVNCMD} co ${SVNUSER} ${REPO} user-source
  )

#-----------------------------------------------------------------------------
# Make changes in the working tree.
message("Changing content...")
update_content(user-source files_added files_removed dirs_added)
if(dirs_added)
  run_child(
    WORKING_DIRECTORY ${TOP}/user-source
    COMMAND ${SVNCMD} add ${depth_empty} ${dirs_added}
    )
endif()
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${SVNCMD} add ${files_added}
  )
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${SVNCMD} rm ${files_removed}
  )

#-----------------------------------------------------------------------------
# Commit the changes to the repository.
message("Committing revision 2...")
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${SVNCMD} commit -m "Changed content"
  )

#-----------------------------------------------------------------------------
# Make changes in the working tree.
message("Changing content again...")
change_content(user-source)

#-----------------------------------------------------------------------------
# Commit the changes to the repository.
message("Committing revision 3...")
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${SVNCMD} commit -m "Changed content again"
  )

#-----------------------------------------------------------------------------
# Go back to before the changes so we can test updating.
message("Backing up to revision 1...")
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${SVNCMD} up -r1
  )

# Create a modified file.
message("Modifying locally...")
modify_content(user-source)

#-----------------------------------------------------------------------------
# Test updating the user work directory with the command-line interface.
message("Running CTest Dashboard Command Line...")

# Create the user build tree.
create_build_tree(user-source user-binary)
file(APPEND ${TOP}/user-binary/CTestConfiguration.ini
  "# SVN command configuration
SVNCommand: ${SVN}
SVNUpdateOptions: --config-dir \"${TOP}/config\"
")

# Run the dashboard command line interface.
run_dashboard_command_line(user-binary)

#-----------------------------------------------------------------------------
# Test initial checkout and update with a dashboard script.
message("Running CTest Dashboard Script...")

create_dashboard_script(dash-binary
  "# Subversion command configuration
set(CTEST_SVN_COMMAND \"${SVN}\")
set(CTEST_SVN_UPDATE_OPTIONS
  \"--config-dir \\\"\${CTEST_DASHBOARD_ROOT}/config\\\"\")
set(CTEST_CHECKOUT_COMMAND
  \"\\\"\${CTEST_SVN_COMMAND}\\\" co -r1 \\\"${REPO}\\\" dash-source\")
")

# Run the dashboard script with CTest.
run_dashboard_script(dash-binary)
