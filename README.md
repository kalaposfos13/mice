# Mechanical Interface for Cargo Equipment

(name pending)
ps4 homebrew game with funny controls

## Building

Non-Linux: Follow the Linux building tutorial, and it will either work or not. ¯\\\_(ツ)\_/¯

Linux:
- Get some build tools, you'll figure it out hopefully. The default compiler is clang, but in theory other compilers can work too.
- Get the toolchain-llvm-18 (older llvm versions probably work too) artifact from the latest release/action from https://github.com/ps4emulation/OpenOrbis-PS4-Toolchain/
- Unzip it, and set the OO_PS4_TOOLCHAIN environment variable to the project root
- Run `make`
