
#include <iostream>
#include <functional>
#include "../threadPool.hpp"
#include "../matrix.hpp"
template<class T>
void calc(const Matrix<T>& a, const Matrix<T>& b, int row, int col, Matrix<T>& result) {
  if (a.getWidth() != b.getHeight()) return;
  int k = a.getWidth();
  for (int i = 0; i < k; ++i) {
    result[row][col] += a[row][i] * b[i][col];
  }
}

template<class T>
void print(const Matrix<T> rhs) {
  for (int i = 0; i < rhs.getHeight(); ++i) {
    for (int j = 0; j < rhs.getWidth(); ++j) {
      std::cout << rhs[i][j] << ' ';
    }
    std::cout << '\n';
  }
}

int main() {

  int n = 1000;

  Matrix<long long> a(n, n);
  Matrix<long long> b(n, n);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      a[i][j] = std::max(i, j);
      b[i][j] = std::min(i, j);
    }
  }

  Matrix<long long> c(n, n);
  auto start1 = std::chrono::high_resolution_clock::now();
  {
    TaskQueue<std::function<void()>> mainQueue;
    ThreadPool pool(6, mainQueue);
    start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        mainQueue.push([&a, &b, &c, i, j]() {calc(a, b, i, j, c); });
      }
    }
  }

  auto end1 = std::chrono::high_resolution_clock::now();
  auto res1 = std::chrono::duration<float>{ end1 - start1 };
  std::cout << res1 << '\n';

  auto start2 = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      calc(a, b, i, j, c);
    }
  }
  auto end2 = std::chrono::high_resolution_clock::now();
  auto res2 = std::chrono::duration<float>{ end2 - start2 };

  std::cout << res2;
}