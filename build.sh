#!/bin/bash

BUILD_DIR="build"
BIN_DIR="$BUILD_DIR/bin"
BUILD_TYPE="Debug"

# Default target
PROJECT_NAME="sandbox"

GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

print_step()    { echo -e "${BLUE}[BUILD]${NC} $1"; }
print_success() { echo -e "${GREEN}[SUCCESS]${NC} $1"; }
print_error()   { echo -e "${RED}[ERROR]${NC} $1"; }

# Detect platform
if [[ "$OSTYPE" == "darwin"* ]]; then
    CORES=$(sysctl -n hw.ncpu)
    PLATFORM="macOS"
else
    CORES=$(nproc)
    PLATFORM="Linux"
fi

DO_CLEAN=false
DO_RUN=true

# -----------------------------
# Parse arguments
# -----------------------------
for arg in "$@"; do
    case $arg in
        clean)      DO_CLEAN=true ;;
        release)    BUILD_TYPE="Release" ;;
        no-run)     DO_RUN=false ;;
        -s)         PROJECT_NAME="sandbox" ;;
        -ve)        PROJECT_NAME="vektor_editor" ;;
        help|-h|--help)
            echo "Usage: ./build.sh [options]"
            echo ""
            echo "Targets:"
            echo "  -s       Run Sandbox"
            echo "  -ve      Run Vektor Editor"
            echo ""
            echo "Build options:"
            echo "  clean    Delete build directory"
            echo "  release  Build in Release mode"
            echo "  no-run   Do not run after build"
            exit 0 ;;
    esac
done

echo -e "${YELLOW}----------------------------------------------"
echo -e " Building Vektor Engine ($PLATFORM - $BUILD_TYPE)"
echo -e " Target: $PROJECT_NAME"
echo -e "----------------------------------------------${NC}"

# -----------------------------
# Clean
# -----------------------------
if [ "$DO_CLEAN" = true ]; then
    print_step "Cleaning build artifacts..."
    rm -rf "$BUILD_DIR"
    print_success "Clean complete."
fi

# -----------------------------
# Configure
# -----------------------------
print_step "Configuring CMake..."
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=$BUILD_TYPE
if [ $? -ne 0 ]; then
    print_error "CMake configuration failed."
    exit 1
fi

# -----------------------------
# Build
# -----------------------------
print_step "Compiling with $CORES cores..."
cmake --build "$BUILD_DIR" --parallel $CORES
if [ $? -ne 0 ]; then
    print_error "Compilation failed."
    exit 1
fi

print_success "Build finished successfully."

# -----------------------------
# Run selected target
# -----------------------------
if [ "$DO_RUN" = true ]; then
    EXE_PATH="$BIN_DIR/$PROJECT_NAME"
    LIB_PATH="$BIN_DIR"

    echo -e "${YELLOW}----------------------------------------------"
    echo -e " Running: $PROJECT_NAME"
    echo -e "----------------------------------------------${NC}"

    if [[ "$PLATFORM" == "macOS" ]]; then
        export DYLD_LIBRARY_PATH="$LIB_PATH:$DYLD_LIBRARY_PATH"
    else
        export LD_LIBRARY_PATH="$LIB_PATH:$LD_LIBRARY_PATH"
    fi

    if [ -f "$EXE_PATH" ]; then
        "$EXE_PATH"
    else
        print_error "Executable not found: $EXE_PATH"
        print_error "Make sure CMake target name matches this."
        exit 1
    fi
fi
