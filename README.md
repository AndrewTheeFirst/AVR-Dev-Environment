# AVR-Dev-Environment

A small collection of build tooling, example firmware projects, and supporting files for AVR microcontroller development.

## Goals
- Provide a copyable template application for new AVR projects.
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

### Setup

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

## Quick start 
1. Open the repository in VS Code.

2. In your terminal, outside of the container, run:

   bash tools/setup.sh

3. Reopen the workspace in the container via Reopen in Container command from the Command Palette (Ctrl+Shift+P)

4. Change to a project directory. For example, in your terminal run:

   ```cd /workspace/development/applications/template```

5. Build the project:

   ```make build```

6. Compile and generate the binaries:

   ```make compile```

7. Flash to the target MCU:

   ```make flash```

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

  ```make flash```

To override defaults for a different board or port:

  ```make flash AVR_DEVICE=atmega328p PORT=/dev/ttyUSB0 BAUD=57600```

If you are flashing from inside the devcontainer, make sure the serial device is visible inside the container. The repo mounts `/dev` into the container to support this pattern.

### Windows / WSL notes
On Windows/WSL, USB serial adapters may need to be attached to WSL first. 

1. Open Powershell / Terminal

2. Install usbipd with:

   ```winget install usbipd```

3. Search for AVR device and. Note the busid.

   ```usbipd wsl list```

4. Attach the port to WSL. Run the command below. Be sure to replace `<busid>` with the bus ID reported by `usbipd wsl list`.

  ```usbipd attach --wsl --busid <busid> --auto-attach```

### macOS notes
- macOS devices are often under `/dev/cu.*` or `/dev/tty.*`.
- On macOS, this command `make flash` must be run OUTSIDE of the container (but still within the correct application directory).
1. To close the container, open the command palette (Cmd+Shift+P) and search ```Dev Containers: Reopen Folder Locally```
2. Change to a project directory, ie.:

   ```cd /workspace/development/applications/template```
3. Then flash to device:

   ```make flash```

### Linux notes
- Ensure the user has appropriate permissions for the serial device.
- Linux devices are often under `/dev/ttyUSB*` or `/dev/ttyACM*`.

## Troubleshooting
### I am not getting syntax highlighting!
* Confirm that `development/artifacts/compile_commands.json` exists, if not, try running:

```make compile```

* Otherwise try reloading the window with Ctrl + R.

### I am getting this error: avrdude ser_open() OS error: cannot open port <...>: No such file or directory!
* Attempt to plug the adapter in before reopening the container or temporarily remove the device mount from the devcontainer configuration.

* Additionally, you can check the actual port name in the container using `ls /dev/tty*` or `ls /dev/serial/by-id`.

### If all else fails, CONTACT ME: pattona@southern.edu.