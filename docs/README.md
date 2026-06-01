# Documentation

This directory contains the Doxygen configuration for generating API documentation for Cosmic Horizons.

## Prerequisites

- [Doxygen](https://www.doxygen.nl/) installed on your system

### Installation

**Ubuntu/Debian:**
```bash
sudo apt-get install doxygen
```

**Fedora:**
```bash
sudo dnf install doxygen
```

**macOS:**
```bash
brew install doxygen
```

**Windows:**
Download from [doxygen.nl](https://www.doxygen.nl/download.html)

## Building Documentation

From the project root directory:

```bash
# Generate documentation
doxygen docs/Doxyfile
```

The HTML documentation will be generated in `docs/output/html/`.

## Viewing Documentation

Open the generated documentation in your browser:

```bash
# Linux/macOS
xdg-open docs/output/html/index.html

# macOS alternative
open docs/output/html/index.html

# Windows
start docs/output/html/index.html
```
