native-test-suite-launchers
===========================

Native test suite launchers for Gradle. The native launchers are used for better coupling between a test framework and Gradle internal event system.

The project is divided into 4 components:
 - Main: The core api
 - Channel: The communication channel between the compiled native test suite and Gradle. The default implementation use socket. Third party can use implement there own if socket is not available on the platform.
 - EntryPoint: The generic entry point definition so the user doesn't need to create one. The plan support is for main, dllmain and kernel driver main.
 - CUnit & Google Test: Integration of the commands with each test framework. Any new test framework support should just need to implement this part.

Status:
 - Socket communication unit test are finished.
 - Threading code has been started but will be dump as it was reveal useless.
 
Planned Feature for the launcher:
 - Integration with CUnit
 - Integration with Google Test
 - Forward stdout and stderr to channel (mainly useful for shared library and kernel driver test)
 - Commands initiated by Gradle:
   - List all test suites and test cases
   - Execute a specific test case

These features are the minimum required for maximum flexibility. Everything else (the actual features) will be implemented in Gradle and most likely shared between the other language.
