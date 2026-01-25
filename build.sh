#!/bin/bash
 
BUILD_DIR="build"
BIN_DIR="bin"
PROJECT_NAME="sandbox" # Name of your executable
BUILD_TYPE="Debug"     # Default build type (Debug or Release)

GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

print_step() {
    echo -e "${BLUE}[BUILD]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

if [[ "$OSTYPE" == "darwin"* ]]; then
    CORES=$(sysctl -n hw.ncpu)
    PLATFORM="macOS"
else
    CORES=$(nproc)
    PLATFORM="Linux"
fi

DO_CLEAN=false
DO_RUN=true

for arg in "$@"
do
    case $arg in
        clean)
            DO_CLEAN=true
            ;;
        release)
            BUILD_TYPE="Release"
            ;;
        no-run)
            DO_RUN=false
            ;;
        help)
            echo "Usage: ./build.sh [clean] [release] [no-run]"
            exit 0
            ;;
    esac
done


echo -e "${YELLOW}----------------------------------------------"
echo -e " Building Vektor Engine ($PLATFORM - $BUILD_TYPE)"
echo -e "----------------------------------------------${NC}"

if [ "$DO_CLEAN" = true ]; then
    print_step "Cleaning build artifacts..."
    rm -rf "$BUILD_DIR"
    rm -rf "$BIN_DIR"
    print_success "Clean complete."
fi

if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

print_step "Configuring CMake..."
cd "$BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
if [ $? -ne 0 ]; then
    print_error "CMake configuration failed."
    exit 1
fi

print_step "Compiling with $CORES cores..."
cmake --build . --config $BUILD_TYPE --parallel $CORES
if [ $? -ne 0 ]; then
    print_error "Compilation failed."
    exit 1
fi

print_success "Build finished successfully."

if [ "$DO_RUN" = true ]; then
    cd ..  
    
    if [ -d "$BIN_DIR/$BUILD_TYPE" ]; then
        EXE_PATH="$BIN_DIR/$BUILD_TYPE/$PROJECT_NAME"
        LIB_PATH="$BIN_DIR/$BUILD_TYPE"
    elif [ -d "$BIN_DIR" ]; then
        EXE_PATH="$BIN_DIR/$PROJECT_NAME"
        LIB_PATH="$BIN_DIR"
    else
        print_error "Could not locate executable in $BIN_DIR"
        exit 1
    fi

    echo -e "${YELLOW}----------------------------------------------"
    echo -e " Running Application..."
    echo -e "----------------------------------------------${NC}"

    if [[ "$PLATFORM" == "macOS" ]]; then
        export DYLD_LIBRARY_PATH=$LIB_PATH:$DYLD_LIBRARY_PATH
    else
        export LD_LIBRARY_PATH=$LIB_PATH:$LD_LIBRARY_PATH
    fi

    # Execute
    if [ -f "$EXE_PATH" ]; then
        "$EXE_PATH"
    else
        print_error "Executable not found at: $EXE_PATH"
        exit 1
    fi
fi