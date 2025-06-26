# TODO: Be a part of Vantor

## Integrate dev environment

- Requirements:
    - Python 3.13
    - A C/C++ Compiler
    - Make
    - CMake

### Linux
```
chmod +x Tools/scripts/Linux/setupEnvironment.sh
./Tools/scripts/Linux/setupEnvironment.sh
```

### Windows

```
./Tools/scripts/Windows/setupEnvironment.bat
```

## Commiting

- Your commit message should be structured like this:
    ```
    "<Module>: <Message> (<CoreVersion>) [<LibraryBuildStatus>]"
    ```
- For example:
    ```
    "Graphics: Added RenderDevice Context (0.15.21) [STABLE]"
    ```