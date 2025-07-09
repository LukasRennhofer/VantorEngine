# VTRG - Vantor Trigger Development Tool

VTRG (Vantor Trigger) is an enhanced command-line development tool for the Vantor Engine. It provides comprehensive functionality for building, formatting, configuring, and managing Vantor projects.

## Installation

VTRG is part of the Vantor Engine repository. No separate installation is required.

### Dependencies

- Python 3.7+
- CMake (for building)
- clang-format (optional, for code formatting)

## Usage

### Basic Commands

```bash
# Build the engine
python vtrg.py build

# Build a specific sample
python vtrg.py build --sample MyProject --platform Linux

# Build in debug mode and run immediately
python vtrg.py build --sample MyProject --debug --run

# Format source code
python vtrg.py format

# Check formatting without making changes
python vtrg.py format --check

# Clean build artifacts
python vtrg.py clean --all
```

### Global Options

- `-h, --help`: Show help information
- `-v, --verbose`: Enable verbose output
- `--version`: Show version information
- `--no-color`: Disable colored output

## Commands Reference

### build
Build internal modules, samples, or the sandbox.

**Options:**
- `--sample <name>`: Build specific sample project
- `--platform <platform>`: Target platform (Windows/Linux/Switch)
- `--debug`: Enable debug build
- `--release`: Enable release build
- `--clean <dir>`: Clean directory before building
- `--run`: Run executable after building
- `--check-deps`: Check build dependencies
- `--jobs <n>`: Number of parallel jobs
- `--target <target>`: Specific build target

### format
Format source code files.

**Options:**
- `--path <path>`: Specific path to format
- `--check`: Check formatting without making changes
- `--extensions <exts>`: File extensions to format
- `--exclude <patterns>`: Patterns to exclude

### clean
Clean build artifacts and temporary files.

**Options:**
- `--all`: Clean everything
- `--build`: Clean build directories
- `--cache`: Clean cache files
- `--path <path>`: Clean specific path
- `--dry-run`: Show what would be cleaned

## Examples

### Development Workflow

```bash
# Start development session
python vtrg.py -v --help

# Build and test a sample
python vtrg.py build --sample Sandbox --debug --run

# Format code before commit
python vtrg.py format --check
python vtrg.py format

# Clean up after development
python vtrg.py clean --build
```

### CI/CD Integration

```bash
# Check dependencies
python vtrg.py build --check-deps

# Build for multiple platforms
python vtrg.py build --platform Linux --release
python vtrg.py build --platform Windows --release

# Verify code formatting
python vtrg.py format --check
```

## Contributing

When contributing to VTRG:

1. Follow the existing code structure
2. Add appropriate error handling
3. Include logging for user feedback
4. Update this documentation for new features
5. Maintain backwards compatibility where possible

## License

VTRG is part of the Vantor Engine and is licensed under the GNU General Public License, Version 3.
