#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <algorithm>
#include <vector>

namespace CircularBuffer {

/// This class represents a circular buffer data structure.

template <typename T>
class CircularBuffer {
 private:
  struct Iterator {
    Iterator(CircularBuffer* buffer, size_t index) : m_buffer(buffer), m_index(index) {}

    T& operator*() const { return m_buffer->m_data[m_buffer->circular_index(m_index)]; }
    T* operator->() { return &(m_buffer->m_data[m_buffer->circular_index(m_index)]); }

    Iterator& operator++() {
      m_index++;
      return *this;
    }

    Iterator operator++(int) const {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_index == b.m_index; }
    friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_index != b.m_index; }

   private:
    CircularBuffer* m_buffer;
    size_t m_index;
  };

  std::vector<T> m_data;
  size_t m_window_size;
  size_t m_index = 0;

  size_t circular_index(size_t index) const noexcept { return (m_index + index) % m_window_size; }

 public:
  explicit CircularBuffer(size_t window_size) : m_window_size(window_size) { m_data.resize(window_size); }

  /**
   * @brief Get the window size of the circular buffer
   *
   * @return size_t buffer window size
   */
  size_t get_window_size() const noexcept { return m_window_size; }

  /**
   * @brief push back a value to the circular buffer
   *
   * @param value value to push back
   */
  void push_back(T value) {
    m_data[m_index] = value;
    m_index = circular_index(1);
  }

  /**
   * @brief Reset the data in the circular buffer (resets to 0)
   *
   */
  void clear() {
    std::fill(m_data.begin(), m_data.end(), 0);
    m_index = 0;
  }

  /**
   * @brief index operator overload
   *
   * @param index index
   * @return T& reference to index in buffer
   */
  T& operator[](size_t index) {
    if (index >= m_window_size) throw std::out_of_range("Index out of bounds");
    return m_data[circular_index(index)];
  }

  /**
   * @brief index operator overload
   *
   * @param index index
   * @return const T& reference to index in buffer
   */
  const T& operator[](size_t index) const {
    if (index >= m_window_size) throw std::out_of_range("Index out of bounds");
    return m_data[circular_index(index)];
  }

  Iterator begin() { return Iterator(this, 0); }
  Iterator end() { return Iterator(this, m_window_size); }
};

}  // namespace CircularBuffer

#endif  // CIRCULARBUFFER_H_

