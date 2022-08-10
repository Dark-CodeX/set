/**
 * This header file is written to create sets in C++.
 * Author: Tushar Chaurasia (https://github.com/Dark-CodeX/)
 * Commit to this repository at https://github.com/Dark-CodeX/set.git
 * You can use this header file. Do not modify it locally, instead commit it on https://www.github.com
 * File: "set.hh" under "set" directory
 * set version: 1.0.0
 * MIT License
 *
 * Copyright (c) 2022 Tushar Chaurasia
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifdef __cplusplus

#ifndef SET_T
#define SET_T

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#include <utility>
#else
#include <bits/functional_hash.h>
#include <bits/move.h>
#endif

#include <functional>
#include <initializer_list>
#include <cstdio>

#define set_t_version "1.0.0"

namespace openutils
{
#ifndef EXIT_HEAP_FAIL
#define EXIT_HEAP_FAIL
	static inline void exit_heap_fail(const void *ptr)
	{
		if (!ptr)
		{
			std::fprintf(stderr, "err: can't allocate heap memory.\n");
			std::exit(EXIT_FAILURE);
		}
	}
#endif

#ifndef RESIZE_ARRAY_OPENUTILS
#define RESIZE_ARRAY_OPENUTILS
	/**
	 * @brief Resizes an array with new space `new_size`, and copies previous elements to the new one.
	 * NOTE: If your array is allocated using `malloc` or `calloc` then, do not use this function instead use `realloc` function defined in `stdlib.h`
	 * @tparam T data type of `arr`
	 * @param arr array to be resized
	 * @param prev_size previous length of `arr`
	 * @param new_size new length of `arr`. NOTE: `prev_size` >= `new_size`
	 * @return T* return new allocated and copied data
	 */
	template <typename T>
	T *resize_array(T *arr, std::size_t prev_size, std::size_t new_size)
	{
		if (prev_size >= new_size)
			return (T *)nullptr;
		T *temp = new T[new_size]();
		exit_heap_fail(temp);
		for (std::size_t i = 0; i < prev_size; i++)
			temp[i] = arr[i];
		delete[] arr;
		return temp;
	}

#endif

	template <typename KEY>
	struct set_node_t
	{
		KEY key;
		set_node_t *next;
		set_node_t() : next(nullptr) {}
		set_node_t(const KEY &key) : key(key), next(nullptr) {}
	};

	template <typename KEY>
	class iter_set_t;

	template <typename KEY>
	class set_t
	{
	private:
		set_node_t<KEY> **keys;
		KEY *order_key;
		std::size_t order_key_len, order_key_cap, len, cap;
		float load_factor;

		std::size_t get_hash(const KEY &key, std::size_t c) const;
		bool equal(const KEY &key1, const KEY &key2) const;
		void rehash();
		friend class iter_set_t<KEY>;

	public:
		set_t(std::size_t capacity = 16, float load_factor = 0.75f);
		set_t(const set_t &other);
		set_t(set_t &&other) noexcept;
		set_t(std::initializer_list<KEY> list);
		set_t(float load_factor);
		bool add(KEY &&key);
		bool add(const KEY &key);
		bool add(const set_node_t<KEY> *node);
		bool add(const set_node_t<KEY> &node);
		bool remove(KEY &&key);
		bool remove(const KEY &key);
		bool contains(KEY &&key) const;
		bool contains(const KEY &key) const;
		const set_node_t<KEY> *get_node(KEY &&key) const;
		const set_node_t<KEY> *get_node(const KEY &key) const;
		std::size_t get_index(KEY &&key) const;
		std::size_t get_index(const KEY &key) const;
		void erase();

		template <typename compt>
		void sort_keys(compt c)
		{
			std::sort(this->order_key, this->order_key + this->order_key_len, c);
		}

		bool empty() const;
		std::size_t length() const;
		std::size_t capacity() const;
		long double error_rate(const std::size_t &expected_size) const;
		typedef iter_set_t<KEY> iter;
		iter iterator() const;
		std::size_t hash() const;
		bool compare(const set_t &m) const;
		bool compare_hash(const set_t &m) const;
		std::size_t max_depth() const;
		std::size_t nerr = (std::size_t)-1;
		void operator=(const set_t &other);
		set_t &operator=(set_t &&other) noexcept;
		bool operator==(const set_t &other) const;
		bool operator!=(const set_t &other) const;
		bool operator+=(KEY k);
		void operator+=(std::initializer_list<KEY> list);
		void operator+=(const set_t &other);
		bool operator-=(KEY &&key);
		bool operator-=(const KEY &key);
		~set_t();
	};

	template <typename KEY>
	set_t<KEY>::set_t(std::size_t capacity, float load_factor)
	{
		this->cap = capacity;
		this->load_factor = load_factor;
		this->len = 0, this->order_key_cap = 10, this->order_key_len = 0;

		this->keys = new set_node_t<KEY> *[this->cap]();
		exit_heap_fail(this->keys);
		this->order_key = new KEY[this->order_key_cap]();
		exit_heap_fail(this->order_key);
		for (std::size_t i = 0; i < this->cap; i++)
			this->keys[i] = nullptr;
	}

	template <typename KEY>
	set_t<KEY>::set_t(const set_t &other)
	{
		this->cap = 16, this->len = 0, this->order_key_len = 0, this->order_key_cap = 10;
		this->keys = new set_node_t<KEY> *[this->cap]();
		exit_heap_fail(this->keys);
		this->order_key = new KEY[this->order_key_cap]();
		exit_heap_fail(this->order_key);
		for (std::size_t i = 0; i < this->cap; i++)
			this->keys[i] = nullptr;
		this->load_factor = other.load_factor;
		for (set_t<KEY>::iter i = other.iterator(); i.c_loop() != false; i.next())
			this->add(i->key);
	}

	template <typename KEY>
	set_t<KEY>::set_t(set_t &&other) noexcept : cap(0), len(0), load_factor(0), keys(nullptr), order_key(nullptr), order_key_cap(0), order_key_len(0)
	{
		this->cap = other.cap;
		this->keys = other.keys;
		this->len = other.len;
		this->order_key = other.order_key;
		this->order_key_cap = other.order_key_cap;
		this->order_key_len = other.order_key_len;
		this->load_factor = other.load_factor;

		other.cap = 0;
		other.keys = nullptr;
		other.len = 0;
		other.load_factor = 0;
		other.order_key = nullptr;
		other.order_key_cap = 0;
		other.order_key_len = 0;
	}

	template <typename KEY>
	set_t<KEY>::set_t(std::initializer_list<KEY> list)
	{
		this->cap = list.size();
		this->len = 0, this->order_key_len = 0, this->order_key_cap = 10;
		this->keys = new set_node_t<KEY> *[this->cap]();
		exit_heap_fail(this->keys);
		this->order_key = new KEY[this->order_key_cap]();
		exit_heap_fail(this->order_key);
		for (std::size_t i = 0; i < this->cap; i++)
			this->keys[i] = nullptr;
		this->load_factor = 0.75f;
		for (auto &k : list)
			this->add(k);
	}

	template <typename KEY>
	set_t<KEY>::set_t(float load_factor)
	{
		this->cap = 16, this->order_key_cap = 10;
		this->keys = new set_node_t<KEY> *[this->cap]();
		exit_heap_fail(this->keys);
		this->order_key = new KEY[this->order_key_cap]();
		exit_heap_fail(this->order_key);
		for (std::size_t i = 0; i < this->cap; i++)
			this->keys[i] = nullptr;
		this->len = 0, this->order_key_len = 0;
		this->load_factor = load_factor;
	}

	template <typename KEY>
	std::size_t set_t<KEY>::get_hash(const KEY &key, std::size_t c) const
	{
		return std::hash<KEY>()(key) % c;
	}

	template <typename KEY>
	bool set_t<KEY>::equal(const KEY &key1, const KEY &key2) const
	{
		return key1 == key2;
	}

	template <typename KEY>
	void set_t<KEY>::rehash()
	{
		std::size_t new_cap = this->cap * 2;
		set_node_t<KEY> **new_table = new set_node_t<KEY> *[new_cap]();
		exit_heap_fail(new_table);
		for (std::size_t i = 0; i < new_cap; i++)
			new_table[i] = nullptr;
		for (std::size_t i = 0; i < this->cap; i++)
		{
			set_node_t<KEY> *cur = this->keys[i];
			while (cur != nullptr)
			{
				std::size_t hash = this->get_hash(cur->key, new_cap);
				set_node_t<KEY> *next = cur->next;
				cur->next = new_table[hash];
				new_table[hash] = cur;
				cur = next;
			}
		}
		delete[] this->keys;
		this->keys = new_table;
		this->cap = new_cap;
	}

	template <typename KEY>
	bool set_t<KEY>::add(KEY &&key)
	{
		std::size_t hash = this->get_hash(key, this->cap);
		set_node_t<KEY> *cur = this->keys[hash];
		while (cur != nullptr)
		{
			if (this->equal(cur->key, key))
				return false;
			cur = cur->next;
		}
		this->order_key[this->order_key_len++] = key;
		set_node_t<KEY> *new_node = new set_node_t<KEY>();
		exit_heap_fail(new_node);
		new_node->key = key;
		new_node->next = this->keys[hash];
		this->keys[hash] = new_node;
		this->len++;
		if (this->len >= this->cap * this->load_factor)
			this->rehash();
		if (this->order_key_len >= this->order_key_cap)
			this->order_key = resize_array<KEY>(this->order_key, this->order_key_len, this->order_key_cap *= 2);
		return true;
	}

	template <typename KEY>
	bool set_t<KEY>::add(const KEY &key)
	{
		return this->add((KEY &&) key);
	}

	template <typename KEY>
	bool set_t<KEY>::add(const set_node_t<KEY> *node)
	{
		if (!node)
			return false;
		return this->add(node->key);
	}

	template <typename KEY>
	bool set_t<KEY>::add(const set_node_t<KEY> &node)
	{
		return this->add(node.key);
	}

	template <typename KEY>
	bool set_t<KEY>::remove(KEY &&key)
	{
		std::size_t hash = this->get_hash(key, this->cap);
		set_node_t<KEY> *cur = this->keys[hash];
		set_node_t<KEY> *prev = nullptr;
		while (cur != nullptr)
		{
			if (this->equal(cur->key, key))
			{
				if (prev == nullptr)
					this->keys[hash] = cur->next;
				else
					prev->next = cur->next;
				delete cur;
				this->len--;
				return true;
			}
			prev = cur;
			cur = cur->next;
		}
		return false;
	}

	template <typename KEY>
	bool set_t<KEY>::remove(const KEY &key)
	{
		return this->remove((KEY &&) key);
	}

	template <typename KEY>
	bool set_t<KEY>::contains(KEY &&key) const
	{
		std::size_t hash = this->get_hash(key, this->cap);
		set_node_t<KEY> *cur = this->keys[hash];
		while (cur != nullptr)
		{
			if (this->equal(cur->key, key))
				return true;
			cur = cur->next;
		}
		return false;
	}

	template <typename KEY>
	bool set_t<KEY>::contains(const KEY &key) const
	{
		return this->contains((KEY &&) key);
	}

	template <typename KEY>
	const set_node_t<KEY> *set_t<KEY>::get_node(KEY &&key) const
	{
		std::size_t hash = this->get_hash(key, this->cap);
		set_node_t<KEY> *cur = this->keys[hash];
		while (cur != nullptr)
		{
			if (this->equal(cur->key, key))
				return cur;
			cur = cur->next;
		}
		return nullptr;
	}

	template <typename KEY>
	const set_node_t<KEY> *set_t<KEY>::get_node(const KEY &key) const
	{
		return this->get_node((KEY &&) key);
	}

	template <typename KEY>
	std::size_t set_t<KEY>::get_index(KEY &&key) const
	{
		std::size_t hash = this->get_hash(key, this->cap);
		set_node_t<KEY> *cur = this->keys[hash];
		if (!cur)
			return (std::size_t)-1;
		return hash;
	}

	template <typename KEY>
	std::size_t set_t<KEY>::get_index(const KEY &key) const
	{
		return this->get_index((KEY &&) key);
	}

	template <typename KEY>
	void set_t<KEY>::erase()
	{
		for (std::size_t i = 0; i < this->cap; i++)
		{
			set_node_t<KEY> *cur = this->keys[i];
			while (cur != nullptr)
			{
				set_node_t<KEY> *next = cur->next;
				delete cur;
				cur = next;
			}
			this->keys[i] = nullptr;
		}
		delete[] this->order_key;
		this->keys = new KEY[this->order_key_cap]();
		exit_heap_fail(this->order_key);
		this->len = 0, this->order_key_len = 0;
	}

	template <typename KEY>
	bool set_t<KEY>::empty() const
	{
		return this->len == 0;
	}

	template <typename KEY>
	std::size_t set_t<KEY>::length() const
	{
		return this->len;
	}

	template <typename KEY>
	std::size_t set_t<KEY>::capacity() const
	{
		return this->cap;
	}

	template <typename KEY>
	long double set_t<KEY>::error_rate(const std::size_t &expected_size) const
	{
		return (expected_size - this->len) * 100.0L / this->len;
	}

	template <typename KEY>
	typename set_t<KEY>::iter set_t<KEY>::iterator() const
	{
		return set_t<KEY>::iter(this);
	}

	template <typename KEY>
	inline void hash_combine(std::size_t &seed, const KEY &k)
	{
		seed ^= std::hash<KEY>()(k) + static_cast<std::size_t>(0xc70f6907UL) + (seed << 7) + (seed >> 3);
	}

	template <typename KEY>
	std::size_t set_t<KEY>::hash() const
	{
		std::size_t h = 0;
		for (set_t<KEY>::iter i = this->iterator(); i.c_loop(); i.next())
			hash_combine(h, i->key);
		return h;
	}

	template <typename KEY>
	bool set_t<KEY>::compare(const set_t &m) const
	{
		if (this->len != m.len)
			return false;
		for (set_t<KEY>::iter i = this->iterator(), j = m.iterator(); i.c_loop() && j.c_loop(); i.next(), j.next())
			if (!this->equal(i->key, j->key))
				return false;
		return true;
	}

	template <typename KEY>
	bool set_t<KEY>::compare_hash(const set_t &m) const
	{
		return this->hash() == m.hash();
	}

	template <typename KEY>
	std::size_t set_t<KEY>::max_depth() const
	{
		std::size_t max = 0, x = 0;
		for (std::size_t i = 0; i < this->cap; i++)
		{
			set_node_t<KEY> *temp = this->keys[i];
			while (temp)
				x++, temp = temp->next;
			if (x > max)
				max = x;
			x = 0;
		}
		return max;
	}

	template <typename KEY>
	void set_t<KEY>::operator=(const set_t &other)
	{
		for (std::size_t i = 0; i < this->cap; i++)
		{
			set_node_t<KEY> *cur = this->keys[i];
			while (cur != nullptr)
			{
				set_node_t<KEY> *next = cur->next;
				delete cur;
				cur = next;
			}
			this->keys[i] = nullptr;
		}
		delete[] this->keys;
		delete[] this->keys;
		this->len = 0, this->cap = 16, this->order_key_len = 0, this->order_key_cap = 10;
		this->load_factor = other.load_factor;
		this->keys = new set_node_t<KEY> *[this->cap]();
		exit_heap_fail(this->keys);
		for (std::size_t i = 0; i < this->cap; i++)
			this->keys[i] = nullptr;
		this->order_key = new KEY[this->order_key_cap]();
		exit_heap_fail(this->order_key);

		for (set_t<KEY>::iter i = other.iterator(); i.c_loop() != false; i.next())
			this->add(i->key);
	}

	template <typename KEY>
	set_t<KEY> &set_t<KEY>::operator=(set_t &&other) noexcept
	{
		if (this != &other)
		{
			for (std::size_t i = 0; i < this->cap; i++)
			{
				set_node_t<KEY> *cur = this->keys[i];
				while (cur != nullptr)
				{
					set_node_t<KEY> *next = cur->next;
					delete cur;
					cur = next;
				}
			}
			delete[] this->keys;
			delete[] this->order_key;

			this->keys = other.keys;
			this->cap = other.cap;
			this->len = other.len;
			this->keys = other.keys;
			this->order_key_cap = other.order_key_cap;
			this->order_key_len = other.order_key_len;
			this->load_factor = other.load_factor;

			other.keys = nullptr;
			other.cap = 0;
			other.len = 0;
			other.order_key = nullptr;
			other.order_key_cap = 0;
			other.order_key_len = 0;
			other.load_factor = 0;
		}
		return *this;
	}

	template <typename KEY>
	bool set_t<KEY>::operator==(const set_t &other) const
	{
		return this->compare(other);
	}

	template <typename KEY>
	bool set_t<KEY>::operator!=(const set_t &other) const
	{
		return !this->compare(other);
	}

	template <typename KEY>
	bool set_t<KEY>::operator+=(KEY k)
	{
		return this->add(k);
	}

	template <typename KEY>
	void set_t<KEY>::operator+=(std::initializer_list<KEY> list)
	{
		for (auto &l : list)
			this->add(l);
	}

	template <typename KEY>
	void set_t<KEY>::operator+=(const set_t &other)
	{
		for (std::size_t i = 0; i < other.cap; i++)
		{
			set_node_t<KEY> *cur = other.keys[i];
			while (cur != nullptr)
			{
				this->add(cur->key);
				cur = cur->next;
			}
		}
	}

	template <typename KEY>
	bool set_t<KEY>::operator-=(KEY &&key)
	{
		return this->remove(key);
	}

	template <typename KEY>
	bool set_t<KEY>::operator-=(const KEY &key)
	{
		return this->remove((KEY &&) key);
	}

	template <typename KEY>
	set_t<KEY>::~set_t()
	{
		for (std::size_t i = 0; i < this->cap; i++)
		{
			set_node_t<KEY> *cur = this->keys[i];
			while (cur != nullptr)
			{
				set_node_t<KEY> *next = cur->next;
				delete cur;
				cur = next;
			}
		}
		delete[] this->keys;
		delete[] this->order_key;
		this->len = 0, this->cap = 0, this->order_key_len = 0, this->order_key_cap = 0;
	}

	template <typename KEY>
	class iter_set_t
	{
	private:
		set_t<KEY> *m;
		set_node_t<KEY> *cur;
		std::size_t i;

	public:
		iter_set_t(const set_t<KEY> *m);
		bool c_loop() const;
		set_node_t<KEY> *&operator->();
		const set_node_t<KEY> *&operator*() const;
		void next();
	};

	template <typename KEY>
	iter_set_t<KEY>::iter_set_t(const set_t<KEY> *m)
	{
		this->m = (set_t<KEY> *)m;
		this->i = 0;
		while (this->i < this->m->order_key_len && this->m->get_node(this->m->order_key[this->i]) == nullptr)
			this->i++;
		this->cur = (set_node_t<KEY> *)this->m->get_node(this->m->order_key[this->i]);
	}

	template <typename KEY>
	bool iter_set_t<KEY>::c_loop() const
	{
		return this->cur != nullptr;
	}

	template <typename KEY>
	set_node_t<KEY> *&iter_set_t<KEY>::operator->()
	{
		return (set_node_t<KEY> *&)this->cur;
	}

	template <typename KEY>
	const set_node_t<KEY> *&iter_set_t<KEY>::operator*() const
	{
		return (const set_node_t<KEY> *&)this->cur;
	}

	template <typename KEY>
	void iter_set_t<KEY>::next()
	{
		this->i++;
		while (this->i < this->m->order_key_len && this->m->get_node(this->m->order_key[this->i]) == nullptr)
			this->i++;
		this->cur = (set_node_t<KEY> *)this->m->get_node(this->m->order_key[this->i]);
	}
};

namespace std
{
	template <typename KEY>
	struct hash<openutils::set_t<KEY>>
	{
		std::size_t operator()(const openutils::set_t<KEY> &m) const
		{
			return m.hash();
		}
	};
};

#endif
#endif