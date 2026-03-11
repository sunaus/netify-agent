export JOBS=17
export MAKEOPTS=-j${JOBS} MAKEFLAGS=-j${JOBS}
#export ENABLE_SANITIZER=address
export OPTION_LIBTCMALLOC=disable
export COMPILER=clang
export CPPFLAGS="-pipe -gdwarf-4 -O0 -fexceptions -Wall"
export DESTDIR=/tmp/netify-agent
