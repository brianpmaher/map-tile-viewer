# Map Tile Viewer

![Screenshot](screenshot.png)

A tool for visualizing map tiles.

## Installation

Only Windows is supported at the moment.

1. Clone or download this repository

2. Navigate to the project directory

3. Clone submodules:

    ```batch
    git submodule update --init --recursive
    ```

4. Build and run the project:

    ```batch
    .\build.bat -r
    ```

## Faster VSCode Build

To speed up the build in VSCode, run the following command in the terminal prior to launching VSCode:

```batch
vcvarsall.bat x64
```

Then launch VSCode from the same terminal.

```batch
code .
```

This seems to only work from a `cmd` terminal, not PowerShell.

Also building in debug mode with the `-d` flag is faster:

```batch
.\build.bat -dr
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Resources

- [Raylib Official Website](https://www.raylib.com/)
