
#pragma once

#include <vector>

template<class T>
class Matrix {
public:
  Matrix() = default;
  Matrix(std::size_t n, std::size_t m) : table_(n, std::vector<T>(m)), height(n), width(m) {};
  ~Matrix() = default;

  Matrix(const Matrix& rhs) {
    if (this != &rhs) {
      table_ = rhs.table_;
      height = rhs.height;
      width = rhs.width;
    }
  };
  Matrix(Matrix&& rhs) {
    if (this != &rhs) {
      std::swap(table_, rhs.table_);
      std::swap(height, rhs.height);
      std::swap(width, rhs.width);
    }
  };

  Matrix& operator=(const Matrix& rhs) {
    if (this != &rhs) {
      table_ = rhs.table_;
      height = rhs.height;
      width = rhs.width;
    }
    return *this;
  };
  Matrix& operator=(Matrix&& rhs) {
    if (this != &rhs) {
      std::swap(table_, rhs.table_);
      std::swap(height, rhs.height);
      std::swap(width, rhs.width);
    }
    return *this;
  };

  std::vector<T>& operator[](std::size_t n) {
    return table_[n];
  };

  const std::vector<T>& operator[](std::size_t n) const {
    return table_[n];
  };

  std::size_t getWidth() const noexcept { return width; };
  std::size_t getHeight() const noexcept { return height; };
private:
  std::size_t height = 0;
  std::size_t width = 0;
  std::vector<std::vector<T>> table_;
};