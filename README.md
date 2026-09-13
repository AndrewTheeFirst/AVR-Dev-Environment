
# AVR-Dev-Environment

### A small collection of build tooling for building firmware for AVR microcontrollers.

### Contains template application and the repository toolchain to help with AVR firmware development.
# AVR-Dev-Environment

A small collection of build tooling, a template application, and supporting files to get you started building firmware for AVR microcontrollers.

## Goals
- Provide a minimal, copyable template application so new projects can be created quickly.  
- Provide reproducible build tooling and a development container for consistent environments.  
- Keep editor integrations (VS Code) simple by outputting compile artifacts in a predictable place.

## Quick overview / Features
- Template application you can copy into a new repo or project folder.  
- Makefile and CMake support for building and flashing.  
- Compile script that places compile commands / artifacts into an artifacts/ folder so VS Code (c_cpp_properties.json) can pick them up.  
- Dockerfile and devcontainer support for development inside a reproducible container.

## VS Code development workflow
- You can develop in the workspace root, but the recommended workflow for full syntax highlighting and IntelliSense is to open VS Code in the `development/` directory (a subdirectory of the workspace). This matches how c_cpp_properties.json and the project layout are set up so VS Code can discover include paths and the compile database in artifacts/.  
- The compile script writes compile_commands.json and other outputs into artifacts/ — keep that folder present so IntelliSense can pick up correct flags and include paths.

## Template usage
1. Copy the template application into your new project directory (for example, copy contents of `template/` into your project root).  
2. Edit the project-local Makefile and `CMakeLists.txt` and update the application name or target variable to match your project.
3. If you need to add/compile additional source files or change flashing settings, edit the Makefile and/or `CMakeLists.txt` in your project — build system changes are intended to be made there.

Tip: Keep project-specific changes inside the project directory so the reusable template can remain generic.

## Build (examples)

Make:
  cd /workspace/development/applications/template
  make build
  make compile
  make flash

The included compile script moves artifacts and the compile_commands.json into an artifacts/ folder. VS Code's c_cpp_properties.json is configured to reference that folder so IntelliSense and browse/indexing work without extra setup.

## Flashing
Example (using avrdude — update programmer/port/MCU to match your hardware)

  make flash

If flashing inside the development container (Dev Container / Docker), be aware:
- The container needs access to the host serial device at container start. If a device/port is referenced in the container configuration but not present, "Reopen in Container" may fail.
- Workarounds:
  - Plug the device (Arduino/USB-serial) into the host *before* opening the container so the port exists at container launch.
  - Temporarily comment out or remove the device-forwarding line from the Dockerfile or devcontainer.json, reopen the container, then re-add the device-forwarding once the container is running and the device is attached.
  - Use VS Code's devcontainer.json "runArgs": ["--device=/dev/ttyUSB0"] or the equivalent to pass the device into the container.

Windows (WSL) specific note:
- On Windows/WSL you may need to attach the USB device to WSL. On Windows 10/11 with usbipd:

  usbipd wsl list
  usbipd wsl attach --busid <busid>

Replace `<busid>` with the bus ID shown by `usbipd wsl list` for your device. After attaching, the device is visible inside WSL as /dev/ttyS*, /dev/ttyUSB* or /dev/ttyACM* depending on the adapter.

macOS / Linux notes:
- On macOS the device is usually /dev/cu.* or /dev/tty.* — plug it in and check `ls /dev/cu.*`.  
- On Linux the device is typically /dev/ttyUSB* or /dev/ttyACM*. If you cannot access the serial device from the container or host, ensure your user is in the `dialout` group (or add a udev rule).



## Troubleshooting
- "Reopen in Container" fails with missing port: plug the device in before reopening, or temporarily remove the device-forwarding line from the Dockerfile/devcontainer.json and reopen, then restore it.  
- On Windows, if ports don't appear in WSL, use usbipd to attach the device.
- If all else fails, contact me: pattona@southern.edu