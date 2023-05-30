#pragma once
#include <iostream>
#include "Defines.h"


class Chain {
public:
	std::pair<int, int> start;
	std::pair<int, int> end;
	Chain() {
		start = { EMPTY,EMPTY };
		end = { EMPTY,EMPTY };
	}
	Chain(std::pair<int, int>& start, std::pair<int, int>& end) {
		this->start = start;
		this->end = end;
	}
	Chain(const std::pair<int, int>& start, const std::pair<int, int>& end) {
		this->start = start;
		this->end = end;
	}
	Chain(std::pair<int, int>&& start, std::pair<int, int>&& end) {
		this->start = std::move(start);
		this->end = std::move(end);
	}
	Chain(Chain&& other) {
		this->start = std::move(other.start);
		this->end = std::move(other.end);
	}
	Chain(const Chain& other) {
		this->start = other.start;
		this->end = other.end;
	}
	Chain(Chain& other) {
		this->start = other.start;
		this->end = other.end;
	}
	Chain(int x1, int y1, int x2, int y2) {
		start = std::make_pair(x1, y1);
		end = std::make_pair(x2, y2);
	}
	Chain(std::initializer_list<int> values) {
		auto it = values.begin();
		start = std::make_pair(*it++, *it++);
		end = std::make_pair(*it++, *it);
	}
	Chain& operator=(Chain&& other) {
		this->start = std::move(other.start);
		this->end = std::move(other.end);
		return *this;
	}
	Chain& operator=(Chain& other) {
		this->start = other.start;
		this->end = other.end;
		return *this;
	}
	Chain& operator=(const Chain& other) {
		this->start = other.start;
		this->end = other.end;
		return *this;
	}

	bool empty() {
		return start.first == EMPTY;
	}

	bool operator<(const Chain& other) const {
		if (start != other.start) {
			return start < other.start;
		}
		return end < other.end;
	}

	bool operator==(const Chain& other) const {
		return start == other.start && end == other.end;
	}
};
 
 
//struct Chain {
//	int x1, y1;
//	int x2, y2;
//	Chain() {
//		x1 = EMPTY;
//		y1 = EMPTY;
//		x2 = EMPTY;
//		y2 = EMPTY;
//	}
//	Chain(Chain&& other) {
//		x1 = other.x1;
//		y1 = other.y1;
//		x2 = other.x2;
//		y2 = other.y2;
//	}
//	Chain(const Chain& other) {
//		x1 = other.x1;
//		y1 = other.y1;
//		x2 = other.x2;
//		y2 = other.y2;
//	}
//	Chain(Chain& other) {
//		x1 = other.x1;
//		y1 = other.y1;
//		x2 = other.x2;
//		y2 = other.y2;
//	}
//	Chain(int x1, int y1, int x2, int y2) {
//		this->x1 = x1;
//		this->y1 = y1;
//		this->x2 = x2;
//		this->y2 = y2;
//	}
//	Chain& operator=(Chain&& other) {
//		x1 = other.x1;
//		y1 = other.y1;
//		x2 = other.x2;
//		y2 = other.y2;
//		return *this;
//	}
//	Chain& operator=(Chain& other) {
//		x1 = other.x1;
//		y1 = other.y1;
//		x2 = other.x2;
//		y2 = other.y2;
//		return *this;
//	}
//	Chain& operator=(const Chain& other) {
//		x1 = other.x1;
//		y1 = other.y1;
//		x2 = other.x2;
//		y2 = other.y2;
//		return *this;
//	}
//
//	bool empty() {
//		return x1 == EMPTY;
//	}
//
//	bool operator<(const Chain& other) const {
//		if (x1 != other.x1) {
//			return x1 < other.x1;
//		}
//		return x2 < other.x2;
//	}
//
//	bool operator==(const Chain& other) const {
//		return x1 == other.x1 && x2 == other.x2 && y1 == other.y1 && y2 == other.y2;
//	}
//
//	std::pair<int, int> first() const {
//		return std::make_pair(x1, y1);
//	}
//
//	std::pair<int, int> second() const {
//		return std::make_pair(x2, y2);
//	}
//};