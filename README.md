* # MAP: The Safe C/C++ Set Library ![license-status](https://img.shields.io/github/license/Dark-CodeX/set)
	* #### **Safe set in C/C++, with huge amount of in-built functions.**
	* #### **This library has 18 pre-defined functions.**
	* #### **This library can be used in both C and C++.**
	* #### This header file is *cross-platform*, but tested only on Windows 10, Windows 11, Arch Linux, Debian Linux, WSL 2 only. (But will work on macOS also). *And checked memory leak using **`valgrind`** in which (18/18) functions passed.*

* # Functions Defined v:1.0.0
```
add(key)
add(node<key> *)
add(node<key> &)
remove
contains
get_node
get_index
erase
sort_keys
empty
length
capacity
error_rate
iterator
hash
compare
comapre_hash
max_depth
```

* # BUILD
	* #### **For testing use:** *`g++ -g -W -Wall -Wextra -std=c++20 ./samples/test.cc -o test` **and then** `valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./test "Hello" "World" "123" "set"`*
* # Installation
	* #### To install on *arch linux* use **`PKGBUILD`**.
	* #### To install on **any Operating System** use:
		* #### [**GUI** Download Jar File (Cross-Platform)](https://github.com/Dark-CodeX/InstallRepos/releases/download/v1.0.0/InstallReposGUI.jar)

		* #### [**CLI** Download Jar File (Cross-Platform)](https://github.com/Dark-CodeX/InstallRepos/releases/download/v1.0.0/InstallReposCLI.jar)
