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
    this->vectorSize = 0;
    this->reservedSize = 2;
    this->vectorArray = new value_type[this->reservedSize];
  }
  Vector(std::initializer_list<Type> l)
  {
    this->vectorSize = l.size();
    this->reservedSize = this->vectorSize;
    this->vectorArray = new value_type[this->reservedSize];
    for(auto i = l.begin(); i != l.end(); i++)
    {
      this->append(*i);
    }
  }

  Vector(const Vector& other)
  {
      this->vectorSize = other.vectorSize;
      this->reservedSize = other.reservedSize;
      this->vectorArray = new value_type[this->reservedSize];
      for(auto i = 0; i != this->vectorSize; i++)
      {
          this->vectorArray[i] = other.vectorArray[i];
      }
  }

  Vector(Vector&& other)
  {
      this->vectorSize = other.vectorSize;
      this->reservedSize = other.reservedSize;
      this->vectorArray = other.vectorArray;
      other.vectorSize = 0;
      other.reservedSize = 0;
      other.vectorArray = nullptr;
  }

  ~Vector()
  {
    delete [] this->vectorArray;
  }

  Vector& operator=(const Vector& other)
  {
      if(this != &other) {
          delete [] this->vectorArray;
          this->vectorSize = other.vectorSize;
          this->reservedSize = other.reservedSize;
          this->vectorArray = new value_type[this->reservedSize];
          for (auto i = 0; i != this->vectorSize; i++)
          {
              this->vectorArray[i] = other.vectorArray[i];
          }
      }
      return *this;
  }

  Vector& operator=(Vector&& other)
  {
      if(this != &other)
      {
          delete [] this->vectorArray;
          this->vectorSize = other.vectorSize;
          this->reservedSize = other.reservedSize;
          this->vectorArray = other.vectorArray;
          other.vectorSize = 0;
          other.reservedSize = 0;
          other.vectorArray = nullptr;
      }
      return *this;
  }

  bool isEmpty() const
  {
    if(this->vectorSize==0)
      return 1;
    else
      return 0;
  }

  size_type getSize() const
  {
    return this->vectorSize;
  }

  void append(const Type& item)
  {
    if(this->vectorSize==this->reservedSize)
      this->reallocate();
    this->vectorArray[(int)this->vectorSize]=item;
    vectorSize++;
  }

  void prepend(const Type& item)
  {
    if(this->vectorSize==this->reservedSize)
      this->reallocate();
    for(auto i=vectorSize; i!=0; i--)
    {
      this->vectorArray[i]=this->vectorArray[i-1];
    }
    this->vectorArray[0]=item;
    this->vectorSize++;
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
    if(this->vectorSize==this->reservedSize)
      this->reallocate();
    size_type insertPlace=insertPosition.index-cbegin().index;
    for(auto i=vectorSize; i!=insertPlace; i--)
    {
      this->vectorArray[i]=this->vectorArray[i-1];
    }
    this->vectorArray[insertPlace]=item;
    this->vectorSize++;
  }

  Type popFirst()
  {
    if(this->isEmpty() == 1)
        throw std::out_of_range("Can't delete first element in empty vector");
    else
    {
        value_type temporary = *begin();
        for(auto i = 0; i != this->vectorSize - 2; i++)
        {
            this->vectorArray[i] = this->vectorArray[i+1];
        }
        this->vectorSize--;
        return temporary;
    }
  }

  Type popLast()
  {
      if(this->isEmpty() == 1)
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
      if (this->isEmpty() == 1)
          throw std::out_of_range("Can't erase element from empty vector");
      if (possition.index >= this->cend().index and possition.index < this->cbegin().index)
          throw std::out_of_range("Can't erase object out of vector");
      for(auto i = possition.index; i != this->vectorSize -1; i++)
      {
          this->vectorArray[i] = this->vectorArray[i+1];
      }
      this->vectorSize--;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
      if (this->isEmpty() == 1)
          throw std::out_of_range("Can't erase element from empty vector");
      if (firstIncluded.index >= this->cend().index and firstIncluded.index < this->cbegin().index)
          throw std::out_of_range("Can't erase object out of vector");
      if (lastExcluded.index >= this->cend().index and lastExcluded.index < this->cbegin().index)
          throw std::out_of_range("Can't erase object out of vector");
      for(auto i = lastExcluded.index; i != this->vectorSize; i++)
      {
          this->vectorArray[i - lastExcluded.index + firstIncluded.index] = this->vectorArray[i-1];
      }
      this->vectorSize -= lastExcluded.index - firstIncluded.index;
  }

  iterator begin()
  {
      iterator i = Iterator(this,0);
      return i;
  }

  iterator end()
  {
      iterator i = Iterator(this,this->vectorSize);
      return i;
  }

  const_iterator cbegin() const
  {
      const_iterator i = ConstIterator(this,0);
      return i;
  }

  const_iterator cend() const
  {
      const_iterator i = ConstIterator(this,this->vectorSize);
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
      this->reservedSize<<=2;
      auto newArray = new value_type[this->reservedSize];
      for (auto i = 0; i != this->vectorSize; i++)
      {
          newArray[i] = this->vectorArray[i];
      }
      delete [] this->vectorArray;
      this->vectorArray = newArray;
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
      this->pointerToVector = nullptr;
      this->index = 0;
  }
  ConstIterator(const Vector* other, size_t other2)
  {
      this->pointerToVector = other;
      this->index = other2;
  }

  reference operator*() const
  {
      if(this->pointerToVector == nullptr or this->pointerToVector->vectorSize <= this->index)
          throw std::out_of_range("Iterator is pointing to non-existing place");
      return this->pointerToVector->vectorArray[this->index];
  }

  ConstIterator& operator++()
  {
      if(this->index+1 == this->pointerToVector->vectorSize)
         throw std::out_of_range("Can't increase iterator");
      this->index++;
      return *this;
  }

  ConstIterator operator++(int)
  {
      ConstIterator i(this->pointerToVector, this->index);
      this->operator++();
      return i;
  }

  ConstIterator& operator--()
  {
      if(this->index - 1 <0)
          throw std::out_of_range("Can't decrease iterator");
      this->index--;
      return *this;
  }

  ConstIterator operator--(int)
  {
      ConstIterator i(this->pointerToVector, this->index);
      this->operator--();
      return i;
  }

  ConstIterator operator+(difference_type d) const
  {
      if (this->index + d > this->pointerToVector->vectorSize or this->index + d < 0)
          throw std::out_of_range("Can't increase/decrease iterator");
      ConstIterator i(this->pointerToVector, this->index + d);
      return i;
  }

  ConstIterator operator-(difference_type d) const
  {
      return this->operator+(-d);
  }

  bool operator==(const ConstIterator& other) const
  {
      if(other.pointerToVector == this->pointerToVector and other.index == this->index)
          return 1;
      else
          return 0;
  }

  bool operator!=(const ConstIterator& other) const
  {
      if(other.pointerToVector == this->pointerToVector and other.index == this->index)
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
