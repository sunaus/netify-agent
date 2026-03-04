export ENABLE_SANITIZER=address OPTION_LIBTCMALLOC=disable
export JOBS=17
export MAKEOPTS=-j${JOBS} MAKEFLAGS=-j${JOBS}
export DESTDIR=/tmp/netify-agent
