# This script drives creation of a git repository and checks
# that CTest can update from it.

#-----------------------------------------------------------------------------
# Test in a directory next to this script.
get_filename_component(TOP "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(TOP "${TOP}/CTest UpdateGIT")
set(UPDATE_EXTRA Updated{module})

# Include code common to all update tests.
include("/home/chris/cmake-2.8.10.2/Tests/CTestUpdateCommon.cmake")

#-----------------------------------------------------------------------------
# Report git tools in use.
message("Using GIT tools:")
set(GIT "/usr/bin/git")
message(" git = ${GIT}")

set(AUTHOR_CONFIG "[user]
\tname = Test Author
\temail = testauthor@cmake.org
")

#-----------------------------------------------------------------------------
# Initialize the testing directory.
message("Creating test directory...")
init_testing()

if(UNIX)
  set(src "/home/chris/cmake-2.8.10.2/Tests")
  configure_file(${src}/CTestUpdateGIT.sh.in ${TOP}/git.sh @ONLY)
  set(GIT ${TOP}/git.sh)
endif()

#-----------------------------------------------------------------------------
# Create the repository.
message("Creating repository...")
file(MAKE_DIRECTORY ${TOP}/repo.git)
run_child(
  WORKING_DIRECTORY ${TOP}/repo.git
  COMMAND ${GIT} --bare init
  )
file(REMOVE_RECURSE ${TOP}/repo.git/hooks)
set(REPO file://${TOP}/repo.git)

# Create submodule repository.
message("Creating submodule...")
file(MAKE_DIRECTORY ${TOP}/module.git)
run_child(
  WORKING_DIRECTORY ${TOP}/module.git
  COMMAND ${GIT} --bare init
  )
file(REMOVE_RECURSE ${TOP}/module.git/hooks)
set(MOD_REPO file://${TOP}/module.git)
create_content(module)
run_child(WORKING_DIRECTORY ${TOP}/module
  COMMAND ${GIT} init
  )
file(REMOVE_RECURSE ${TOP}/module/.git/hooks)
file(APPEND ${TOP}/module/.git/config "
[remote \"origin\"]
\turl = ${MOD_REPO}
\tfetch = +refs/heads/*:refs/remotes/origin/*
${AUTHOR_CONFIG}")
run_child(WORKING_DIRECTORY ${TOP}/module
  COMMAND ${GIT} add .
  )
run_child(WORKING_DIRECTORY ${TOP}/module
  COMMAND ${GIT} commit -m "Initial content"
  )
run_child(WORKING_DIRECTORY ${TOP}/module
  COMMAND ${GIT} push origin master:refs/heads/master
  )

#-----------------------------------------------------------------------------
# Import initial content into the repository.
message("Importing content...")
create_content(import)

# Import the content into the repository.
run_child(WORKING_DIRECTORY ${TOP}/import
  COMMAND ${GIT} init
  )
file(REMOVE_RECURSE ${TOP}/import/.git/hooks)
file(APPEND ${TOP}/import/.git/config "
[remote \"origin\"]
\turl = ${REPO}
\tfetch = +refs/heads/*:refs/remotes/origin/*
${AUTHOR_CONFIG}")
run_child(WORKING_DIRECTORY ${TOP}/import
  COMMAND ${GIT} add .
  )
run_child(WORKING_DIRECTORY ${TOP}/import
  COMMAND ${GIT} config core.safecrlf false
  )
run_child(WORKING_DIRECTORY ${TOP}/import
  COMMAND ${GIT} submodule add ${MOD_REPO} module
  )
run_child(WORKING_DIRECTORY ${TOP}/import
  COMMAND ${GIT} commit -m "Initial content"
  )
run_child(WORKING_DIRECTORY ${TOP}/import
  COMMAND ${GIT} push origin master:refs/heads/master
  )

#-----------------------------------------------------------------------------
# Modify the submodule.
change_content(module)
run_child(WORKING_DIRECTORY ${TOP}/module
  COMMAND ${GIT} add -u
  )
run_child(WORKING_DIRECTORY ${TOP}/module
  COMMAND ${GIT} commit -m "Changed content"
  )
run_child(WORKING_DIRECTORY ${TOP}/module
  COMMAND ${GIT} push origin master:refs/heads/master
  )

#-----------------------------------------------------------------------------
# Create a working tree.
message("Checking out revision 1...")
run_child(
  WORKING_DIRECTORY ${TOP}
  COMMAND ${GIT} clone ${REPO} user-source
  )
file(REMOVE_RECURSE ${TOP}/user-source/.git/hooks)
file(APPEND ${TOP}/user-source/.git/config "${AUTHOR_CONFIG}")
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} submodule init
  )
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} submodule update
  )

# Save the first revision name.
execute_process(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} rev-parse HEAD
  OUTPUT_VARIABLE revision1
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

#-----------------------------------------------------------------------------
# Create an empty commit.
message("Creating empty commit...")
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} commit --allow-empty -m "Empty commit"
  )

#-----------------------------------------------------------------------------
# Make changes in the working tree.
message("Changing content...")
update_content(user-source files_added files_removed dirs_added)
if(dirs_added)
  run_child(
    WORKING_DIRECTORY ${TOP}/user-source
    COMMAND ${GIT} add ${dirs_added}
    )
endif()
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} add ${files_added}
  )
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} rm ${files_removed}
  )
run_child(WORKING_DIRECTORY ${TOP}/user-source/module
  COMMAND ${GIT} checkout master
  )
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} add -u
  )

#-----------------------------------------------------------------------------
# Commit the changes to the repository.
message("Committing revision 2...")
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} commit -m "Changed content"
  )
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} push origin
  )

#-----------------------------------------------------------------------------
# Make changes in the working tree.
message("Changing content again...")
change_content(user-source)
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} add -u
  )

#-----------------------------------------------------------------------------
# Commit the changes to the repository.
message("Committing revision 3...")
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} commit -m "Changed content again"
  )
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} push origin
  )

#-----------------------------------------------------------------------------
# Go back to before the changes so we can test updating.
macro(rewind_source src_dir)
  message("Backing up to revision 1...")
  run_child(
    WORKING_DIRECTORY ${TOP}/${src_dir}
    COMMAND ${GIT} reset --hard ${revision1}
    )
  run_child(
    WORKING_DIRECTORY ${TOP}/${src_dir}
    COMMAND ${GIT} submodule update
    )
endmacro()
rewind_source(user-source)

# Make sure pull does not try to rebase (which does not work with
# modified files) even if ~/.gitconfig sets "branch.master.rebase".
run_child(
  WORKING_DIRECTORY ${TOP}/user-source
  COMMAND ${GIT} config branch.master.rebase false
  )

# Create a modified file.
modify_content(user-source)

#-----------------------------------------------------------------------------
# Test updating the user work directory with the command-line interface.
message("Running CTest Dashboard Command Line...")

# Create the user build tree.
create_build_tree(user-source user-binary)
file(APPEND ${TOP}/user-binary/CTestConfiguration.ini
  "# GIT command configuration
UpdateCommand: ${GIT}
")

# Run the dashboard command line interface.
set(UPDATE_NO_MODIFIED 1)
run_dashboard_command_line(user-binary)
set(UPDATE_NO_MODIFIED 0)

rewind_source(user-source)
modify_content(user-source)

message("Running CTest Dashboard Command Line (custom update)...")

# Create the user build tree.
create_build_tree(user-source user-binary-custom)
file(APPEND ${TOP}/user-binary-custom/CTestConfiguration.ini
  "# GIT command configuration
UpdateCommand: ${GIT}
GITUpdateCustom: ${GIT};pull;origin;master
")

# Run the dashboard command line interface.
run_dashboard_command_line(user-binary-custom)

#-----------------------------------------------------------------------------
# Test initial checkout and update with a dashboard script.
message("Running CTest Dashboard Script...")

create_dashboard_script(dash-binary
  "# git command configuration
set(CTEST_GIT_COMMAND \"${GIT}\")
set(CTEST_GIT_UPDATE_OPTIONS)
execute_process(
  WORKING_DIRECTORY \"${TOP}\"
  COMMAND \"${GIT}\" clone \"${REPO}\" dash-source
  )

# Test .git file.
file(RENAME \"${TOP}/dash-source/.git\" \"${TOP}/dash-source/repo.git\")
file(WRITE \"${TOP}/dash-source/.git\" \"gitdir: repo.git\n\")

execute_process(
  WORKING_DIRECTORY \"${TOP}/dash-source\"
  COMMAND \"${GIT}\" reset --hard ${revision1}
  )
execute_process(
  WORKING_DIRECTORY \"${TOP}/dash-source\"
  COMMAND \"${GIT}\" submodule init
  )
execute_process(
  WORKING_DIRECTORY \"${TOP}/dash-source\"
  COMMAND \"${GIT}\" submodule update
  )
")

# Run the dashboard script with CTest.
run_dashboard_script(dash-binary)

rewind_source(dash-source)

#-----------------------------------------------------------------------------
# Test custom update with a dashboard script.
message("Running CTest Dashboard Script (custom update)...")

create_dashboard_script(dash-binary-custom
  "# git command configuration
set(CTEST_GIT_COMMAND \"${GIT}\")
set(CTEST_GIT_UPDATE_OPTIONS)
set(CTEST_GIT_UPDATE_CUSTOM \${CTEST_GIT_COMMAND} pull origin master)
")

# Run the dashboard script with CTest.
run_dashboard_script(dash-binary-custom)
