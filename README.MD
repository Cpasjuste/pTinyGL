Portable TinyGL, a limited software OpenGL wrapper for multiple platforms

Compilation (SDL1)
- mkdir build && cd build
- cmake -DBUILD_SDL1=ON -DCMAKE_BUILD_TYPE=Release ..
- make install

Compilation (SDL2)
- mkdir build && cd build
- cmake -DBUILD_SDL2=ON -DCMAKE_BUILD_TYPE=Release ..
- make install

Compilation (PS Vita)
- mkdir build && cd build
- cmake -DBUILD_PSP2=ON -DCMAKE_BUILD_TYPE=Release ..
- make install
