#ifndef VECTOR_H
#define VECTOR_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstddef>
#endif

template <typename T>
class Vector
{
public:
  /**
   * @brief Constructs an empty vector.
   */
  Vector();

  /**
   * @brief Destructor.
   */
  ~Vector();

  /**
   * @brief Copy constructor.
   * 
   * @param other The vector to be copied.
   */
  Vector(const Vector<T>& other);

  /**
   * @brief Assignment operator.
   * 
   * @param other The vector to be assigned.
   * @return Vector<T>& Reference to the assigned vector.
   */
  Vector<T>& operator=(const Vector<T>& other);

  /**
   * @brief Adds an element to the end of the vector.
   * 
   * @param value The value to be added.
   */
  void push_back(const T& value);

  /**
   * @brief Removes the last element from the vector.
   */
  void pop_back();

  /**
   * @brief Removes all elements from the vector.
   */
  void clear();

  /**
   * @brief Accesses the element at the specified index in the vector (read-only).
   * 
   * @param index The index of the element to access.
   * @return const T& Reference to the element at the specified index.
   */
  const T& operator[](size_t index) const;

  /**
   * @brief Accesses the element at the specified index in the vector.
   * 
   * @param index The index of the element to access.
   * @return T& Reference to the element at the specified index.
   */
  T& operator[](size_t index);

  /**
   * @brief Accesses the element at the specified index in the vector (read-only).
   * 
   * @param index The index of the element to access.
   * @return const T& Reference to the element at the specified index.
   */
  const T& at(size_t index) const;

  /**
   * @brief Accesses the element at the specified index in the vector.
   * 
   * @param index The index of the element to access.
   * @return T& Reference to the element at the specified index.
   */
  T& at(size_t index);

  /**
   * @brief Accesses the first element in the vector.
   * 
   * @return T& Reference to the first element.
   */
  T& front();

  /**
   * @brief Accesses the last element in the vector.
   * 
   * @return T& Reference to the last element.
   */
  T& back();

  /**
   * @brief Returns the number of elements in the vector.
   * 
   * @return size_t The number of elements.
   */
  size_t size() const;

  /**
   * @brief Checks if the vector is empty.
   * 
   * @return bool `true` if the vector is empty, `false` otherwise.
   */
  bool empty() const;

private:
  T* values_;      // Pointer to the dynamically allocated array of values
  size_t capacity_;  // Current capacity of the vector
  size_t size_;      // Number of elements in the vector

  /**
   * @brief Resizes the internal storage of the vector to accommodate a new capacity.
   * 
   * @param newCapacity The new capacity of the vector.
   */
  void resize(size_t newCapacity);
};

template <typename T>
Vector<T>::Vector()
  : values_(nullptr),
    capacity_(0),
    size_(0)
{
}

template <typename T>
Vector<T>::~Vector()
{
  clear();
}

template <typename T>
Vector<T>::Vector(const Vector<T>& other)
  : values_(nullptr),
    capacity_(0),
    size_(0)
{
  *this = other;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
  if (this != &other)
  {
    clear();
    resize(other.size_);

    for (size_t i = 0; i < other.size_; ++i)
    {
      values_[i] = other.values_[i];
    }

    size_ = other.size_;
  }
  return *this;
}

template <typename T>
void Vector<T>::push_back(const T& value)
{
  if (size_ == capacity_)
  {
    resize(capacity_ + 1);
  }

  values_[size_] = value;
  ++size_;
}

template <typename T>
void Vector<T>::pop_back()
{
  if (size_ > 0)
  {
    --size_;
  }
}

template <typename T>
void Vector<T>::clear()
{
  if (values_ != nullptr)
  {
    delete[] values_;
    values_ = nullptr;
  }

  capacity_ = 0;
  size_ = 0;
}

template <typename T>
const T& Vector<T>::operator[](size_t index) const
{
  return values_[index];
}

template <typename T>
T& Vector<T>::operator[](size_t index)
{
  return values_[index];
}

template <typename T>
const T& Vector<T>::at(size_t index) const
{
  if (index < size_)
  {
    return values_[index];
  }
  else
  {
    // Handle index out of bounds
    // For example, you can throw an exception or return a default value.
    // Here, we return the first element as a default value.
    return values_[0];
  }
}

template <typename T>
T& Vector<T>::at(size_t index)
{
  if (index < size_)
  {
    return values_[index];
  }
  else
  {
    // Handle index out of bounds
    // For example, you can throw an exception or return a default value.
    // Here, we return the first element as a default value.
    return values_[0];
  }
}

template <typename T>
T& Vector<T>::front()
{
  return values_[0];
}

template <typename T>
T& Vector<T>::back()
{
  return values_[size_ - 1];
}

template <typename T>
size_t Vector<T>::size() const
{
  return size_;
}

template <typename T>
bool Vector<T>::empty() const
{
  return size_ == 0;
}

template <typename T>
void Vector<T>::resize(size_t newCapacity)
{
  if (newCapacity <= capacity_)
  {
    return;
  }

  T* newValues = new T[newCapacity];

  if (values_ != nullptr)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      newValues[i] = values_[i];
    }

    delete[] values_;
  }

  values_ = newValues;
  capacity_ = newCapacity;
}

#endif