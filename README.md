# AVR-Dev-Environment

A small collection of build tooling, example firmware projects, and supporting files for AVR microcontroller development.

## Goals
- Provide a minimal, copyable template application for new AVR projects.
- Provide reproducible CMake and Make-based build tooling.
- Include a devcontainer so the same AVR toolchain is available in a consistent environment.
- Keep editor integrations simple by generating compile artifacts in a predictable location.

## Repository overview
This repository is organized around a reusable AVR development setup:

- `development/applications/` contains example or project-specific firmware applications.
- `development/applications/template/` is the starting point for a new application.
- `development/scripts/` contains the shared Make variables and the compile script used by app Makefiles.
- `development/cmake/toolchain-avr.cmake` configures the AVR GCC toolchain.
- `.devcontainer/` contains the Dockerfile and VS Code devcontainer configuration.

## Prerequisites / Requirements
The repository is designed around an AVR development toolchain and a serial programmer connection.

### Recommended setup: VS Code devcontainer
This is the easiest path and matches the repo configuration:

- VS Code
- Docker Desktop or Docker Engine
- VS Code Dev Containers extension
- A USB serial adapter or Arduino-compatible device connected to the host for flashing

The devcontainer build installs the required packages and downloads the AVR GNU toolchain automatically via the Dockerfile in `.devcontainer/`.

### Serial device requirements
For flashing, a device such as an Arduino Uno, Nano, or USB-to-TTL serial adapter must be available at a port such as:

- Linux: `/dev/ttyACM0`, `/dev/ttyUSB0`
- WSL: `/dev/ttyS*`, `/dev/ttyUSB*`, or `/dev/ttyACM*`
- macOS: `/dev/cu.*` or `/dev/tty.*`

The repo defaults to:

- MCU: `atmega328p`
- programmer: `arduino`
- baud: `115200`
- port: `/dev/ttyACM0`

You may need to add your user to the `dialout` group on Linux or create a udev rule for serial access.

## VS Code development workflow
The recommended workflow is to open VS Code in the `development/` directory instead of the workspace root. This matches the project layout and the generated compile database.

- `development/.vscode/c_cpp_properties.json` points IntelliSense at `development/artifacts/compile_commands.json`.
- The compile script copies the generated `compile_commands.json` into `development/artifacts/`.
- Keep the `development/artifacts/` folder available so VS Code can resolve correct include paths and compiler flags.

## Quick start
1. Open the repository in VS Code.
2. If using the devcontainer setup, reopen the workspace in the container.
3. In your terminal, change to a project directory, for example:

   cd /workspace/development/applications/template

4. Build the project:

   make build

5. Compile and generate the compile database:

   make compile

6. Flash to the target MCU:

   make flash

The project uses the common build script in `development/scripts/` to configure the AVR toolchain and output the firmware hex file.

## Template usage
1. Copy the contents of `development/applications/template/` into a new project folder.
2. Update the project-local `Makefile` and `CMakeLists.txt` to match the new application name or target.
3. Add or remove source files in the app-specific project as needed.
4. Adjust the build or flash settings in the local project files when your hardware or MCU differs from the defaults.

Tip: Keep project-specific edits inside the application directory so the shared template remains reusable.

## Build system details
The build system is lightweight:

- `development/scripts/common.mk` sets the default AVR toolchain, device, port, programmer, and flash arguments.
- `development/scripts/compile.sh` runs CMake and copies the resulting compile database to `development/artifacts/`.
- `development/cmake/toolchain-avr.cmake` sets the AVR compiler and target flags.
- `development/applications/template/CMakeLists.txt` builds an `firmware` executable and emits a `.hex` file from the compiled ELF output.

## Flashing
The default flash step uses `avrdude` with the Arduino profile and the target MCU set in the shared Makefile.

Example:

  make flash

To override defaults for a different board or port:

  make flash AVR_DEVICE=atmega328p PORT=/dev/ttyUSB0 BAUD=57600

If you are flashing from inside the devcontainer, make sure the serial device is visible inside the container. The repo mounts `/dev` into the container to support this pattern.

### Windows / WSL notes
On Windows/WSL, USB serial adapters may need to be attached to WSL first.

Example:

  usbipd wsl list
  usbipd wsl attach --busid <busid>

Replace `<busid>` with the bus ID reported by `usbipd wsl list`.

### macOS / Linux notes
- macOS devices are often under `/dev/cu.*` or `/dev/tty.*`.
- Linux devices are often under `/dev/ttyUSB*` or `/dev/ttyACM*`.
- Ensure the user has appropriate permissions for the serial device.

## Troubleshooting
- If the container cannot see the device, plug the adapter in before reopening the container or temporarily remove the device mount from the devcontainer configuration.
- If `/dev/ttyACM0` does not exist, check the actual port name with `ls /dev/tty*` or `ls /dev/serial/by-id`.
- If the serial port is not writable, check user permissions and add yourself to the `dialout` group if required.
- If IntelliSense is missing include paths, confirm that `development/artifacts/compile_commands.json` exists and that VS Code is opened in the `development/` folder.
- If all else fails, CONTACT ME: pattona@southern.edu.

## License and support
This project is intended for education and local firmware development. The repo is structured to be copied and adapted for general development.