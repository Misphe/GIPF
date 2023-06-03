#pragma once
#include <iostream>
#include "Defines.h"


class Chain {
public:
	std::pair<int, int> start;
	std::pair<int, int> end;
	char color;
	Chain() {
		start = { EMPTY,EMPTY };
		end = { EMPTY,EMPTY };
		color = EMPTY;
	}
	Chain(const std::pair<int, int>& start, const std::pair<int, int>& end, char set_color) {
		this->start = start;
		this->end = end;
		this->color = set_color;
	}
	Chain(std::pair<int, int>&& start, std::pair<int, int>&& end, char&& set_color) {
		this->start = std::move(start);
		this->end = std::move(end);
		this->color = set_color;
	}
	Chain(Chain&& other) {
		this->start = std::move(other.start);
		this->end = std::move(other.end);
		this->color = other.color;
	}
	Chain(const Chain& other) {
		this->start = other.start;
		this->end = other.end;
		this->color = other.color;
	}
	Chain(int x1, int y1, int x2, int y2, char set_color) {
		start = std::make_pair(x1, y1);
		end = std::make_pair(x2, y2);
		this->color = set_color;
	}
	Chain& operator=(Chain&& other) {
		this->start = std::move(other.start);
		this->end = std::move(other.end);
		this->color = other.color;
		return *this;
	}
	Chain& operator=(const Chain& other) {
		this->start = other.start;
		this->end = other.end;
		this->color = other.color;
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

	bool operator<(Chain&& other) const {
		if (start != other.start) {
			return start < other.start;
		}
		return end < other.end;
	}

	bool operator==(const Chain& other) const {
		return start == other.start && end == other.end;
	}

	bool operator==(Chain&& other) const {
		return start == other.start && end == other.end;
	}
};