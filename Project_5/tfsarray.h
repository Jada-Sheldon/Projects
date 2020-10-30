#pragma once

// Jada Sheldon
// 10 - 27 - 2020
// tfsarray.h

#ifndef TFSARRAY_H
#define TFSARRAY_H

#include <cstddef> // for std::size_t
#include <algorithm> // for std::max
#include <stdexcept> // for std::out_of_range



// class TFSArray
// frightfully smart array of value_type (not as good as a vector)
// Resizeable and exception safe
// Invariants:
//		0<= _size <= _capacity
//		_data points to an array of value_type, allocated with new [],
//      owned by *this, holding _capacity value_type values -- UNLESS
//      _capacity == 0, in which case _data may be nullptr.

template <class T>
class TFSArray {

// ********* TFSArray : Types **********
public:

	// value_type : data item type
	using value_type = T;

	// size_type : type of sizes & indices
	using size_type = std::size_t;

	
	// iterator, const_iterator : random_access iterator types
	using iterator = value_type*;

	using const_iterator = const value_type*;




// ******** TFSArray : internal-use constants ********
private:

	// Capacity of default-constructed object
	enum {DEFAULT_CAP = 16 };




	// ******* TFSArray : ctors, op=, dctor ********
public:

	// Default ctor & ctor from szie
	// Strong Guarantee
	explicit TFSArray(size_type size = 0)
		:_capacity(std::max(size, size_type(DEFAULT_CAP))),
		_size(size),
		_data(_capacity == 0 ? nullptr :new value_type[_capacity])
	{}

	// Copy ctor
	// Strong Guarantee
	TFSArray(const TFSArray& other) 
		:_capacity(other._capacity),
		_size(other._size),
		_data(other._capacity == 0 ? nullptr
									: new value_type[other._capacity])
	{
		try {
			std::copy(other.begin(), other.end(), begin());
		}
		catch (...) {
			delete[] _data;
			throw;
		}
	}

	// Move ctor
	// No-Throw Guarantee
	TFSArray(TFSArray&& other) noexcept 
		:_capacity(other._capacity),
		_size(other._size),
		_data(other._data)
	{
		other._capacity = 0;
		other._size = 0;
		other._data = nullptr;
	}

	// copy Assignment
	// Strong Guarantee
	TFSArray& operator=(const TFSArray& other) {
		TFSArray copy_of_rhs(other);
		swap(copy_of_rhs);
		return *this;
	}

	// Move Assignment
	// No-throw Guarantee
	TFSArray& operator=(TFSArray&& other) noexcept{
		swap(other);
		return *this;
	}

	// dctor
	// No-Throw Guarantee
	~TFSArray() {
		delete[] _data;
	}




// ******** TFSArray : general public operators *********
public:

	// operator[] - non-const & const
	// pre:
	//		0 <= index  <= _size
	// No-Throw Guarantee
	value_type& operator[](size_type index) noexcept {
		return _data[index];
	}

	const value_type& operator[](size_type index) const noexcept {
		return _data[index];
	}




// ********* TFSArray : general public functions **********
public:

	//size
	// No-throw Guarantee
	size_type size() const noexcept {
		return _size;
	}


	// empty
	// No-Throw Guarantee
	bool empty() const noexcept {
		return size() == 0;
	}


	// begin - non-const & const
	// No-Throw Guarantee
	iterator begin() noexcept {
		return _data;
	}

	const_iterator begin() const noexcept {
		return _data;
	}


	// end - non-const & const
	// No-Throw Gaurantee
	iterator end() noexcept
	{
		return begin() + size();
	}

	const_iterator end() const noexcept
	{
		return begin() + size();
	}


	// resize
	// Strong Guarantee
	void resize(size_type newsize) {
		if (newsize > _capacity) {
			size_type newCapacity = std::max(newsize, 5*_capacity/3);
			value_type* newData = new value_type[newCapacity];
			
			try {
				std::copy(begin(), end(), newData);
			}
			catch (...) {
				delete[] newData;
				throw;
			}

			std::swap(_size, newsize);
			std::swap(_capacity, newCapacity);
			std::swap(_data, newData);
			delete[] newData;
		} else {
			_size = newsize;
		}
	}


	// insert
	// Strong Guarantee
	iterator insert(iterator pos, const value_type& item) {
		if (pos<begin() || pos>end()) 
			throw std::out_of_range("iterator out of range.\n");
		
		size_type index = pos - begin();
		resize(size() +1);
		_data[_size-1] = item;
		try {
			std::rotate(begin() + index, end()-1, end());
		}
		catch (...) {
			throw;
		}

		return index + begin();
	}


	// erase
	// Strong Guarantee
	iterator erase(iterator pos) {
		if (pos<begin() || pos>end()) 
			throw std::out_of_range("iterator out of range. \n");

		size_type index = pos - begin();
		try {
			std::rotate(pos, pos + 1, end());
		}
		catch (...) {
			throw;
		}

		resize(size() - 1);
		return index + begin();
	}


	// push_back
	// Strong Guarantee
	void push_back(const value_type & item) {
		insert(end(), item);
	}


	// pop_back
	// Strong Guarantee
	void pop_back() {
		erase(end() - 1);
	}


	//swap
	// No-Throw Guarantee
	void swap(TFSArray& other) noexcept {
		std::swap(_capacity, other._capacity);
		std::swap(_size, other._size);
		std::swap(_data, other._data);
	}




// ******** TFSArray : Data members *********
private:
	size_type _capacity;
	size_type _size;
	value_type* _data;
};
// End Class TFSArray

#endif  //#ifndef TFSARRAY_H
