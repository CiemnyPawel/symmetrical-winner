#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Vector
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
    size_type vectorSize;
    size_type reservedSize;
    pointer vectorArray;
public:
  Vector()
  {
    vectorSize = 0;
    reservedSize = 2;
    vectorArray = new value_type[reservedSize];
  }
  Vector(std::initializer_list<Type> l)
  {
    vectorSize = 0;
    reservedSize = l.size() + 1;
    vectorArray = new value_type[reservedSize];
    for(auto i = l.begin(); i != l.end(); i++)
    {
      append(*i);
    }
  }

  Vector(const Vector& other)
  {
      vectorSize = other.vectorSize;
      reservedSize = other.reservedSize;
      vectorArray = new value_type[reservedSize];
      for(auto i = 0; i != vectorSize; i++)
      {
          vectorArray[i] = other.vectorArray[i];
      }
  }

  Vector(Vector&& other)
  {
      vectorSize = other.vectorSize;
      reservedSize = other.reservedSize;
      vectorArray = other.vectorArray;
      other.vectorSize = 0;
      other.reservedSize = 0;
      other.vectorArray = nullptr;
  }

  ~Vector()
  {
    if(vectorArray == nullptr)
      return;
    delete [] vectorArray;
  }

  Vector& operator=(const Vector& other)
  {
      if(this != &other) {
          delete [] vectorArray;
          vectorSize = other.vectorSize;
          reservedSize = other.reservedSize;
          vectorArray = new value_type[reservedSize];
          for (auto i = 0; i != vectorSize; i++)
          {
              vectorArray[i] = other.vectorArray[i];
          }
      }
      return *this;
  }

  Vector& operator=(Vector&& other)
  {
      if(this != &other)
      {
          delete [] vectorArray;
          vectorSize = other.vectorSize;
          reservedSize = other.reservedSize;
          vectorArray = other.vectorArray;
          other.vectorSize = 0;
          other.reservedSize = 0;
          other.vectorArray = nullptr;
      }
      return *this;
  }

  bool isEmpty() const
  {
    if(vectorSize==0)
      return 1;
    else
      return 0;
  }

  size_type getSize() const
  {
    return vectorSize;
  }

  void append(const Type& item)
  {
    if(vectorSize==reservedSize)
      reallocate();
    vectorArray[(int)vectorSize]=item;
    vectorSize++;
  }

  void prepend(const Type& item)
  {
    if(vectorSize==reservedSize)
      reallocate();
    for(auto i=vectorSize; i!=0; i--)
    {
      vectorArray[i]=vectorArray[i-1];
    }
    vectorArray[0]=item;
    vectorSize++;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if(insertPosition==cbegin())
    {
      prepend(item);
      return;
    }
    if(insertPosition==cend())
    {
      append(item);
      return;
    }
    if(vectorSize==reservedSize)
      reallocate();
    size_type insertPlace=insertPosition.index-cbegin().index;
    for(auto i=vectorSize; i!=insertPlace; i--)
    {
      vectorArray[i]=vectorArray[i-1];
    }
    vectorArray[insertPlace]=item;
    vectorSize++;
  }

  Type popFirst()
  {
    if(isEmpty() == 1)
        throw std::out_of_range("Can't delete first element in empty vector");
    else
    {
        value_type temporary = *begin();
        for(auto i = 0; i != vectorSize - 1; i++)
        {
            vectorArray[i] = vectorArray[i+1];
        }
        vectorSize--;
        return temporary;
    }
  }

  Type popLast()
  {
      if(isEmpty() == 1)
          throw std::out_of_range("Can't delete last element in empty vector");
      else
      {
          value_type temporary = vectorArray[vectorSize - 1];
          vectorSize--;
          return temporary;
      }
  }

  void erase(const const_iterator& possition)
  {
      if (isEmpty() == 1)
          throw std::out_of_range("Can't erase element from empty vector");
      if (possition.index >= vectorSize or possition.index < 0)
          throw std::out_of_range("Can't erase object out of vector");
      for(auto i = possition.index; i != vectorSize -1; i++)
      {
          vectorArray[i] = vectorArray[i+1];
      }
      vectorSize--;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
      if (isEmpty() == 1)
          throw std::out_of_range("Can't erase element from empty vector");
      if (firstIncluded.index >= cend().index and firstIncluded.index < cbegin().index)
          throw std::out_of_range("Can't erase object out of vector");
      if (lastExcluded.index >= cend().index and lastExcluded.index < cbegin().index)
          throw std::out_of_range("Can't erase object out of vector");
      for(auto i = lastExcluded.index; i != vectorSize; i++)
      {
          vectorArray[i - lastExcluded.index + firstIncluded.index] = vectorArray[i-1];
      }
      vectorSize -= lastExcluded.index - firstIncluded.index;
  }

  iterator begin()
  {
      iterator i = Iterator(this,0);
      return i;
  }

  iterator end()
  {
      iterator i = Iterator(this,vectorSize);
      return i;
  }

  const_iterator cbegin() const
  {
      const_iterator i = ConstIterator(this,0);
      return i;
  }

  const_iterator cend() const
  {
      const_iterator i = ConstIterator(this,vectorSize);
      return i;
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
  void reallocate()
  {
      reservedSize*=2;
      auto newArray = new value_type[reservedSize];
      for (auto i = 0; i != vectorSize; i++)
      {
          newArray[i] = vectorArray[i];
      }
      delete [] vectorArray;
      vectorArray = newArray;
  }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;

    const Vector* pointerToVector;
    size_t index;


  explicit ConstIterator()
  {
      pointerToVector = nullptr;
      index = 0;
  }
  ConstIterator(const Vector* other, size_t other2)
  {
      pointerToVector = other;
      index = other2;
  }

  reference operator*() const
  {
      if(pointerToVector == nullptr or pointerToVector->vectorSize <= index)
          throw std::out_of_range("Iterator is pointing to non-existing place");
      return pointerToVector -> vectorArray[index];
  }

  ConstIterator& operator++()
  {
      if(index + 1 == pointerToVector -> vectorSize + 1)
         throw std::out_of_range("Can't increase iterator");
      index++;
      return *this;
  }

  ConstIterator operator++(int)
  {
      if(index + 1 == pointerToVector -> vectorSize + 1)
        throw std::out_of_range("Can't increase iterator");
      ConstIterator result(*this);
      ++(*this);
      return result;
  }

  ConstIterator& operator--()
  {
      if((int)index - 1 <0)
          throw std::out_of_range("Can't decrease iterator");
      index--;
      return *this;
  }

  ConstIterator operator--(int)
  {
      if((int)index - 1 <0)
        throw std::out_of_range("Can't decrease iterator");
      ConstIterator result(*this);
      --(*this);
      return result;
  }

  ConstIterator operator+(difference_type d) const
  {
      if (index + d > pointerToVector->vectorSize or index + d < 0)
          throw std::out_of_range("Can't increase/decrease iterator");
      ConstIterator i(pointerToVector, index + d);
      return i;
  }

  ConstIterator operator-(difference_type d) const
  {
      return operator+(-d);
  }

  bool operator==(const ConstIterator& other) const
  {
      if(other.pointerToVector == pointerToVector and other.index == index)
          return 1;
      else
          return 0;
  }

  bool operator!=(const ConstIterator& other) const
  {
      if(other.pointerToVector == pointerToVector and other.index == index)
          return 0;
      else
          return 1;
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator (const Vector* other, size_t other2)
            : ConstIterator(other, other2) {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_VECTOR_H
