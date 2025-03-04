<!-- Pull Request Template -->

## Description

<!-- Please include a summary of the change and which issue is fixed. Please also include relevant motivation and context. List any dependencies that are required for this change. -->


Fixes # (issue)

## Type of change

<!-- Please delete options that are not relevant. -->


- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)
- [ ] This change requires a documentation update
   * If you changed anything other than pinning submodules, then application teams will need to make the same change.
   * Examples of changes that may need documentation include CMake, link libraries, file moves/renames, common API call order changes, etc
   * Please document this change in "Application Integration Notes" section of the [upcoming Core release notes](https://github.deere.com/MachineAutomation/Ipm2CoreDocumentation/tree/master/Releases)
      * Updating APIs that are internal to common repositories and are not used directly by applications do not require application integration notes.
- [ ] This change requires new or updated DISDATT tests
- [ ] This change requires new or updated Test Cases

## How Has This Been Tested?

<!-- Please describe the tests that you ran to verify your changes. Provide instructions so we can reproduce. Please also list any relevant details for your test configuration -->


- [ ] Test A
- [ ] Test B

**Test Configuration**:

* Hardware: IPM2 Ultra/High/Standard or Nitro
* Camera configuration: Stereo/Mono
* Connected Cameras: #

## Checklist:

<!-- Please complete all items before merging or add a comment justifying why an item doesn't apply -->


- [ ] Author(s) have performed a self-review of the code
- [ ] The Verification Champion is aware of needed Test Case changes
- [ ] The Automation Engineer is aware of needed DISDATT test changes or simulation gaps preventing automated testing
- [ ] To my(our) knowledge, the code is easy to understand, with good names, and no misspellings
- [ ] Documentation has been updated
- [ ] These changes generate no new warnings
- [ ] Tests have been added that prove the fix is effective or that the feature works
- [ ] New and existing unit tests pass locally with these changes
- [ ] Any dependent changes have been merged and published in upstream modules
- [ ] Any hard-to-understand areas of the code have been commented
