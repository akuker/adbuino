# Supplementary Files

## Debugging
`launch.json` - an example file to go into `.vscode/` if wanting to debug with Visual Studio Code.

`991-local.rules` - a example file to allow user access to the picoprobe with openocd

Visual Studio Code extensions need to be installed for debugging
 - CMake Tools
 - C/C++
 - Cortex-Debug
 - WSL - if you want use WSLv2 for compiling under linux but running Windows

 Other tools needed for using WSLv2:
- usbipd https://github.com/dorssel/usbipd-win - to attach the picoprobe to the WSL instance