# Just these things:

sudo pacman -S mingw-w64-gcc mingw-w64-headers mingw-w64-crt mingw-w64-binutils
sudo pacman -S wine

# In Build folder:

cp /usr/x86_64-w64-mingw32/bin/libgcc_s_seh-1.dll .
cp /usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll .
cp /usr/x86_64-w64-mingw32/bin/libstdc++-6.dll .

