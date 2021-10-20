# Cross Compiler Installation on Linux
---
## Binutils Installation
```bash
export PREFIX="/usr/local/i386elfgcc"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"

mkdir /tmp/src
cd /tmp/src
curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.24.tar.gz
tar xf binutils-2.24.tar.gz
mkdir binutils-build
cd binutils-build
../binutils-2.24/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
sudo make all install 2>&1 | tee make.log
```

add this line to the end of bashrc (`nano $HOME/.bashrc`):
```bash
export PATH=$PATH:/usr/local/i386elfgcc/bin # add to the end of .bashrc
```

---
## GCC Installation
`Needed even if GCC already installed.`

```bash
cd /tmp/src
curl -O https://ftp.gnu.org/gnu/gcc/gcc-11.2.0/gcc-11.2.0.tar.gz
tar xf gcc-11.2.0.tar.gz
mkdir gcc-build
cd gcc-build
../gcc-11.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-languages=c --without-headers
sudo make all-gcc 
sudo make all-target-libgcc 
sudo make install-gcc 
sudo make install-target-libgcc
```