pkgname="openutils-set"
pkgver="1.0.0"
pkgrel="1"
pkgdesc="Memory Safe Set Library in C/C++ language."
arch=("x86_64" "i686")
conflicts=('openutils-set')
depends=(gcc)
license=("MIT")
source=("https://raw.githubusercontent.com/Dark-CodeX/set/master/set/set.hh" "https://raw.githubusercontent.com/Dark-CodeX/set/master/set/set.h" "https://raw.githubusercontent.com/Dark-CodeX/set/master/set/prototype_err.h")

sha512sums=("914d27ac6fa35b4e9649e0fddb971732be299c795c79ec2ce0127d552cfc4ecd4e2b42ba3341e5ae82df325659c99ae1cf06625c204a1b82ffebad07e528c0ee" "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e" "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e")

package() {
	echo "Memory Safe Set Library in C/C++ language."
	mkdir -p "${pkgdir}/usr/include/openutils/set"
	cp "${srcdir}/set.hh" "${pkgdir}/usr/include/openutils/set/set.hh"
	cp "${srcdir}/set.h" "${pkgdir}/usr/include/openutils/set/set.h"
	cp "${srcdir}/prototype_err.h" "${pkgdir}/usr/include/openutils/set/prototype_err.h"
	echo "Done!"
}
