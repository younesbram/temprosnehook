# TemprosneHook
**TemprosneHook** is a hooking library designed for use with Team Fortress 2. It allows for intercepting and modifying game functions to create custom behaviors such as bot automation and cheats. This project is for educational purposes only and demonstrates how hooking can be implemented in C++.
## Features
- **Function Hooking:** Intercept and modify game functions.
- **Custom Configurations:** Easily adjust parameters via configuration files.
- **Automated Setup:** Use the provided script to set up the environment quickly.
## Getting Started
### Prerequisites
- A working installation of Team Fortress 2.
- C++ development environment (GCC/Clang/MSVC).
- CMake for building the project.
- Git for cloning the repository.
### Installation
1. **Clone the repository:**
```git clone https://github.com/RosneBurgerworks/temprosnehook.git
cd temprosnehook
```
2. **Build the project:**
```
mkdir build
cd build
cmake ..
make
```
3.**Run the installation script:**
```
../scripts/install.sh
```
4.**Download Rosnehook:**
Open a terminal window and enter the following command:
```
bash <(wget -qO- https://raw.githubusercontent.com/RosneBurgerworks/One-in-all-rosnehook-install/master/install-all)
```
### Configuration
Modify the configuration files in the `config_data` directory to adjust the behavior of the hooks. These settings control various aspects of how the hooks interact with the game.
#### Example Configuration
An example configuration might look like this:
```
{
"aimbot": {
"enabled": true,
"smoothness": 0.8
},
"esp": {
"enabled": true,
"box": true,
"health": true
}
}
```
## Detailed File Descriptions
### `.circleci/config.yml`
- **Purpose:** CI configuration for CircleCI.
- **Functionality:** Defines build and test jobs to ensure code quality.
- **Contents:**
- Specifies jobs for building and testing.
- Defines the Docker image to use for the build environment.
- Sets up caching for dependencies to speed up builds.
### `class_dumping/`
- **Purpose:** Contains scripts and tools for dumping class information.
- **Functionality:** Used to extract and analyze class structures from the game.
- **Files:**
- **`class_dump.py`**: Script for dumping class information.
- **`README.md`**: Documentation for class dumping tools.
### `config_data/`
- **Purpose:** Holds configuration files.
- **Functionality:** Allows customization of various features like aimbots and ESP.
- **Files:**
- **`aimbot.json`**: Configuration for aimbot settings.
- **`esp.json`**: Configuration for ESP settings.
- **`general.json`**: General configuration settings.
- **`README.md`**: Documentation for configuration files.
### `data/`
- **Purpose:** Contains data files used by the hooks.
- **Functionality:** Stores necessary data for hook operations.
- **Files:**
- **`offsets.txt`**: Memory offsets for game functions.
- **`signatures.txt`**: Signatures used to locate game functions.
### `external/`
- **Purpose:** External dependencies.
- **Functionality:** Includes third-party libraries and tools required by the project.
- **Files:**
- **`libhook.so`**: Precompiled hooking library.
- **`libjsoncpp.a`**: JSON parsing library.
- **`README.md`**: Documentation for external dependencies.
### `include/`
- **Purpose:** Header files.
- **Functionality:** Defines interfaces and structures used across the project.
- **Subdirectories and Files:**
- **`classinfo/`**:
- **`player_classinfo.h`**: Header for player class information.
- **`core/`**:
- **`hook_manager.h`**: Manages the lifecycle of hooks.
- **`utils.h`**: Utility functions used across the project.
- **`hacks/`**:
- **`aimbot.h`**: Implements aimbot functionality.
- **`esp.h`**: Implements ESP features.
- **`hooks/`**:
- **`hook.h`**: Generic hook interface.
- **`vmt_hook.h`**: Virtual method table hooking.
- **`reclasses/`**:
- **`c_baseentity.h`**: Base entity class.
- **`sdk/`**:
- **`game_sdk.h`**: Game-specific SDK functions.
- **`settings/`**:
- **`config_manager.h`**: Manages configuration settings.
- **`visual/`**:
- **`draw_utils.h`**: Drawing utilities for ESP.
### `lib/`
- **Purpose:** Compiled libraries.
- **Functionality:** Contains static and dynamic libraries used by the project.
- **Files:**
- **`libtemprosne.a`**: Static library for TemprosneHook.
- **`libtemprosne.so`**: Dynamic library for TemprosneHook.
### `scripts/`
- **Purpose:** Automation scripts.
- **Functionality:** Includes scripts for installation, setup, and other utilities.
- **Files:**
- **`install.sh`**: Installation script.
- **`update.sh`**: Update script.
- **`README.md`**: Documentation for scripts.
### `src/`
- **Purpose:** Source code.
- **Functionality:** Core implementation of the hooking mechanism.
- **Files:**
- **`main.cpp`**: Initializes and manages hooks.
- **`hook_manager.cpp`**: Manages the lifecycle of hooks.
- **`aimbot.cpp`**: Implements aimbot functionality.
- **`esp.cpp`**: Implements ESP features.
- **`utils.cpp`**: Utility functions used across the project.
- **`memory.cpp`**: Memory manipulation functions.
### `.autotools/`
- **Purpose:** Autotools configuration.
- **Functionality:** Automates the build process with Autotools.
- **Files:**
- **`configure.ac`**: Autotools configure script.
- **`Makefile.am`**: Automake file for building the project.
### `.clang-format`
- **Purpose:** Clang format configuration.
- **Functionality:** Defines coding style for the project.
- **Contents:**
- Specifies formatting rules such as indentation, brace style, and spacing.
### `.gitignore`
- **Purpose:** Git ignore file.
- **Functionality:** Specifies files and directories to be ignored by Git.
- **Contents:**
- Lists build artifacts, temporary files, and other untracked files to ignore.
### `.gitlab-ci.yml`
- **Purpose:** CI configuration for GitLab CI.
- **Functionality:** Defines build and test jobs for GitLab CI.
- **Contents:**
- Specifies jobs for building, testing, and deploying the project.
### `.gitmodules`
- **Purpose:** Git submodules configuration.
- **Functionality:** Manages external Git repositories as submodules.
- **Contents:**
- Lists submodule repositories and their paths.
### `CMakeLists.txt`
- **Purpose:** CMake configuration.
- **Functionality:** Defines the build process with CMake.
- **Contents:**
- Specifies project name, version, and required C++ standard.
- Lists source files and include directories.
- Configures build targets and dependencies.
### `README.md`
- **Purpose:** Project documentation.
- **Functionality:** Provides an overview, installation instructions, and usage details.
### `TODO`
- **Purpose:** Task list.
- **Functionality:** Contains a list of tasks and features to be implemented.
### `Toolchain-cross-m32.cmake`
- **Purpose:** Cross-compilation configuration.
- **Functionality:** Defines settings for cross-compiling the project.
### `attach`
- **Purpose:** Debugging script.
- **Functionality:** Attaches a debugger to the game process.
- **Contents:**
- Contains commands to attach to the game process for debugging.
### `attach-gdb`
- **Purpose:** GDB debugging script.
- **Functionality:** Attaches GDB to the game process for debugging.
- **Contents:**
- Contains GDB commands to attach to the game process for debugging.
### `build_names`
- **Purpose:** Build configuration.
- **Functionality:** Specifies names for different build configurations.
### `config`
- **Purpose:** Runtime configuration.
- **Functionality:** Configures runtime settings for hooks.
- **Contents:**
- Specifies runtime parameters for the hooking library.
### `install-all`
- **Purpose:** Master installation script.
- **Functionality:** Automates the entire setup process.
- **Contents:**
- Contains commands to install all necessary components for the project.
### `install-data`
- **Purpose:** Data installation script.
- **Functionality:** Installs necessary data files.
- **Contents:**
- Contains commands to download and install data files.
### `preload`
- **Purpose:** Preload script.
- **Functionality:** Preloads the hooking library before game execution.
- **Contents:**
- Contains commands to preload the hooking library.
### `preload-gdb`
- **Purpose:** GDB preload script.
- **Functionality:** Preloads the hooking library with GDB for debugging.
- **Contents:**
- Contains commands to preload the hooking library with GDB for debugging.
### `update`
- **Purpose:** Update script.
- **Functionality:** Updates the hooking library to the latest version.
- **Contents:**
- Contains commands to update the hooking library.
## Usage
To use TemprosneHook, run the compiled executable. Ensure that Team Fortress 2 is running, and the hooks will automatically be applied based on your configuration.
## Support
For support, join the [Discord Server](https://discord.gg/XqBRXE5QVy).
## Contributing
Contributions are welcome! Please fork the repository and submit pull requests. Ensure that your code follows the existing style and includes appropriate comments and documentation.
### Issues
If you encounter any issues, please report them on the [issues page](https://github.com/RosneBurgerworks/temprosnehook/issues).
## License
This project is licensed under the GPL-3.0 License - see the [LICENSE](LICENSE) file for details.
## Disclaimer
This software is for educational purposes only. Using it to cheat in online games is against the terms of service of most games, including Team Fortress 2. 
---
For more detailed information, please refer to the source code and configuration files in the repository. If you have any questions or need further assistance, feel free to open an issue or contact the repository maintainers.
