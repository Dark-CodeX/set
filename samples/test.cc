#include "../set/set.hh"
#include <iostream>
#include <openutils/sstring/sstring.hh>

int main(int argc, char const **argv)
{
	using namespace openutils;
	set_t<sstring> set;
	for (std::size_t i = 1; i < (std::size_t)argc; ++i)
		set.add(argv[i]);
	for (set_t<sstring>::iter i = set.iterator(); i.c_loop() != false; i.next())
		std::cout << i->key << std::endl;
	std::cout << "------------------------------------------" << std::endl;
	std::cout << "Length = " << set.length() << std::endl;
	std::cout << "Capacity = " << set.capacity() << std::endl;
	std::cout << "Max Depth = " << set.max_depth() << std::endl;
	std::cout << "HASH = " << std::hash<set_t<sstring>>()(set) << std::endl;
	std::cout << "Error Rate = " << set.error_rate(argc - 1) << "%" << std::endl;
	return 0;
}
