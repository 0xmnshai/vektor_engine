#!/bin/bash

BUILD_DIR="build"
BIN_DIR="$BUILD_DIR/bin"
PROJECT_NAME="sandbox"
BUILD_TYPE="Debug"

GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

print_step()    { echo -e "${BLUE}[BUILD]${NC} $1"; }
print_success() { echo -e "${GREEN}[SUCCESS]${NC} $1"; }
print_error()   { echo -e "${RED}[ERROR]${NC} $1"; }

if [[ "$OSTYPE" == "darwin"* ]]; then
    CORES=$(sysctl -n hw.ncpu)
    PLATFORM="macOS"
else
    CORES=$(nproc)
    PLATFORM="Linux"
fi

DO_CLEAN=false
DO_RUN=true

for arg in "$@"; do
    case $arg in
        clean)   DO_CLEAN=true ;;
        release) BUILD_TYPE="Release" ;;
        no-run)  DO_RUN=false ;;
        help)
            echo "Usage: ./build.sh [clean] [release] [no-run]"
            exit 0 ;;
    esac
done

echo -e "${YELLOW}----------------------------------------------"
echo -e " Building Vektor Engine ($PLATFORM - $BUILD_TYPE)"
echo -e "----------------------------------------------${NC}"

if [ "$DO_CLEAN" = true ]; then
    print_step "Cleaning build artifacts..."
    rm -rf "$BUILD_DIR"
    print_success "Clean complete."
fi

print_step "Configuring CMake..."
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=$BUILD_TYPE
if [ $? -ne 0 ]; then
    print_error "CMake configuration failed."
    exit 1
fi

print_step "Compiling with $CORES cores..."
cmake --build "$BUILD_DIR" --parallel $CORES
if [ $? -ne 0 ]; then
    print_error "Compilation failed."
    exit 1
fi

print_success "Build finished successfully."

if [ "$DO_RUN" = true ]; then
    EXE_PATH="$BIN_DIR/$PROJECT_NAME"
    LIB_PATH="$BIN_DIR"

    echo -e "${YELLOW}----------------------------------------------"
    echo -e " Running Application..."
    echo -e "----------------------------------------------${NC}"

    if [[ "$PLATFORM" == "macOS" ]]; then
        export DYLD_LIBRARY_PATH="$LIB_PATH:$DYLD_LIBRARY_PATH"
    else
        export LD_LIBRARY_PATH="$LIB_PATH:$LD_LIBRARY_PATH"
    fi

    if [ -f "$EXE_PATH" ]; then
        "$EXE_PATH"
    else
        print_error "Executable not found at: $EXE_PATH"
        exit 1
    fi
fi
