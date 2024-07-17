@echo ON

call "%VS2022INSTALLDIR%/VC/Auxiliary/Build/vcvars64.bat"

set buildType=%1
if %buildType% == Debug (
    goto build
) else if %buildType% == Release (
    goto build
) else if %buildType% == RelWithDebInfo (
    goto build
) else if %buildType% == Configure (
    goto cmake
) else if %buildType% == ReConfigure (
    rd /s /q .build
    goto cmake
) else (
    echo Error %buildType% InValid
    goto end
)

:build
REM 编译
cmake.exe --build .build/%buildType% --config %buildType% -j 16 
goto end

:cmake
REM 生成cmake缓存
cmake.exe -S . -B .build/Debug -GNinja -DCMAKE_BUILD_TYPE=Debug
cmake.exe -S . -B .build/Release -GNinja -DCMAKE_BUILD_TYPE=Release
cmake.exe -S . -B .build/RelWithDebInfo -GNinja -DCMAKE_BUILD_TYPE=RelWithDebInfo
:end