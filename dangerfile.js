/*
 Copyright Deere & Company. For more information,
 please see COPYRIGHT file in root of source repository.
*/
const danger = require('@deere-embedded/ado-danger-rules')

danger.validate_pr_size()
danger.check_for_gitignore()
danger.check_for_empty_pr_description()
danger.icd_structs_were_modified()
danger.ini_file_was_modified()
danger.check_if_submodules_are_pinned_to_master()
danger.all_checkboxes_finished()
danger.check_for_git_issue()