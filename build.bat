@echo off

rmdir /s /q build\

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

cmake ^
  -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake" ^
  -DCMAKE_CXX_COMPILER="cl.exe" ^
  -DCMAKE_C_COMPILER="cl.exe" ^
  -DCMAKE_LINKER="link.exe" ^
  -G "Ninja" ^
  -B "build" ^
  -S .

pushd build
ninja
popd