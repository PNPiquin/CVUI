//
// Created by p-np on 12/04/2022.
//

#ifndef CAMERAAPP_MATRIX_H
#define CAMERAAPP_MATRIX_H

template<typename T>
class Matrix
{
private:
  T* mat;
  unsigned rows;
  unsigned cols;

public:
  Matrix();
  Matrix(unsigned _rows, unsigned _cols);
  Matrix(unsigned _rows, unsigned _cols, T* buffer);
  Matrix(const Matrix<T>& rhs);
  ~Matrix();

  // Operator overloading, for "standard" mathematical matrix operations
  Matrix<T>& operator=(const Matrix<T>& rhs);

  // Matrix mathematical operations
  Matrix<T>& operator+(const Matrix<T>& rhs);
  Matrix<T>& operator+=(const Matrix<T>& rhs);
  Matrix<T>& operator-(const Matrix<T>& rhs);
  Matrix<T>& operator-=(const Matrix<T>& rhs);
  Matrix<T>& operator*(const Matrix<T>& rhs);
  Matrix<T>& operator*=(const Matrix<T>& rhs);
  Matrix<T> transpose();

  // Matrix/scalar operations
  Matrix<T> operator+(const T& rhs);
  Matrix<T> operator-(const T& rhs);
  Matrix<T> operator*(const T& rhs);
  Matrix<T> operator/(const T& rhs);

  // Access the individual elements
  T& operator()(const unsigned& row, const unsigned& col);
  const T& operator()(const unsigned& row, const unsigned& col) const;
  bool is_inside(const int& row, const int& col)
  {
    if (row < 0 || col < 0) {
      return false;
    }
    if (row >= int(this->rows) || col >= int(this->cols)) {
      return false;
    }
    return true;
  };

  // Access the row and column sizes
  unsigned get_rows() const;
  unsigned get_cols() const;
};

template<typename T>
Matrix<T>::Matrix()
{
  mat = nullptr;
  rows = 0;
  cols = 0;
}

template<typename T>
Matrix<T>::Matrix(unsigned int _rows, unsigned int _cols)
{
  mat = new T[_rows * _cols];
  rows = _rows;
  cols = _cols;
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& rhs)
{
  rows = rhs.rows;
  cols = rhs.cols;
  mat = new T[rows * cols];
  memcpy(mat, rhs.mat, sizeof(T) * rows * cols);
}

template<typename T>
Matrix<T>::Matrix(unsigned int _rows, unsigned int _cols, T* buffer)
{
  rows = _rows;
  cols = _cols;
  mat = new T[rows * cols];
  memcpy(mat, buffer, sizeof(T) * rows * cols);
}

template<typename T>
Matrix<T>::~Matrix()
{
  if (mat) {
    delete[] mat;
  }
};

// Assignment Operator
template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& rhs)
{
  if (&rhs == this)
    return *this;

  if (mat) {
    delete[] mat;
  }

  rows = rhs.get_rows();
  cols = rhs.get_cols();
  mat = new T[rows * cols];
  memcpy(mat, rhs.mat, sizeof(T) * rows * cols);

  return *this;
}

// Matrix mathematical Operator
template<typename T>
Matrix<T>& Matrix<T>::operator+(const Matrix<T>& rhs)
{
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++) {
    for (unsigned j = 0; j < cols; j++) {
      result(i, j) = this->operator()(i, j) + rhs(i, j);
    }
  }

  return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator-(const Matrix<T>& rhs)
{
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++) {
    for (unsigned j = 0; j < cols; j++) {
      result(i, j) = this->operator()(i, j) - rhs(i, j);
    }
  }

  return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator*(const Matrix<T>& rhs)
{
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++) {
    for (unsigned j = 0; j < cols; j++) {
      result(i, j) = this->operator()(i, j) * rhs(i, j);
    }
  }

  return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& rhs)
{
  assert(rows == rhs.get_rows());
  assert(cols == rhs.get_cols());

  for (unsigned i = 0; i < rows; i++) {
    for (unsigned j = 0; j < cols; j++) {
      this->operator()(i, j) += rhs(i, j);
    }
  }

  return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& rhs)
{
  assert(rows == rhs.get_rows());
  assert(cols == rhs.get_cols());

  for (unsigned i = 0; i < rows; i++) {
    for (unsigned j = 0; j < cols; j++) {
      this->operator()(i, j) -= rhs(i, j);
    }
  }

  return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& rhs)
{
  assert(rows == rhs.get_rows());
  assert(cols == rhs.get_cols());

  for (unsigned i = 0; i < rows; i++) {
    for (unsigned j = 0; j < cols; j++) {
      this->operator()(i, j) *= rhs(i, j);
    }
  }

  return *this;
}

// Access the individual elements
template<typename T>
T& Matrix<T>::operator()(const unsigned& row, const unsigned& col)
{
  return this->mat[row * cols + col];
}

// Access the individual elements (const)
template<typename T>
const T& Matrix<T>::operator()(const unsigned& row, const unsigned& col) const
{
  return this->mat[row * cols + col];
}

// Get the number of rows of the matrix
template<typename T>
unsigned Matrix<T>::get_rows() const
{
  return this->rows;
}

// Get the number of columns of the matrix
template<typename T>
unsigned Matrix<T>::get_cols() const
{
  return this->cols;
}

template<typename T>
Matrix<T> Matrix<T>::transpose()
{
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++) {
    for (unsigned j = 0; j < cols; j++) {
      result(i, j) = this->mat[j][i];
    }
  }

  return result;
}

// Matrix/scalar addition
template<typename T>
Matrix<T> Matrix<T>::operator+(const T& rhs)
{
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++) {
    for (unsigned j = 0; j < cols; j++) {
      result(i, j) = this->operator()(i, j) + rhs;
    }
  }

  return result;
}

// Matrix/scalar subtraction
template<typename T>
Matrix<T> Matrix<T>::operator-(const T& rhs)
{
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++) {
    for (unsigned j = 0; j < cols; j++) {
      result(i, j) = this->operator()(i, j) - rhs;
    }
  }

  return result;
}

// Matrix/scalar multiplication
template<typename T>
Matrix<T> Matrix<T>::operator*(const T& rhs)
{
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++) {
    for (unsigned j = 0; j < cols; j++) {
      result(i, j) = this->operator()(i, j) * rhs;
    }
  }

  return result;
}

// Matrix/scalar division
template<typename T>
Matrix<T> Matrix<T>::operator/(const T& rhs)
{
  Matrix result(rows, cols, 0.0);

  for (unsigned i = 0; i < rows; i++) {
    for (unsigned j = 0; j < cols; j++) {
      result(i, j) = this->operator()(i, j) / rhs;
    }
  }

  return result;
}

#endif // CAMERAAPP_MATRIX_H
