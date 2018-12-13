#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
	class LinkedList
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

		struct Node
		{
		public:
			value_type value;
			Node* next;
			Node* prev;
			Node()
			{
				prev = this;
				next = this;
			}
			Node(value_type val, Node* prev1, Node* next1)
			{
				value=val;
				prev=prev1;
				next=next1;
				prev->next = this;
				next->prev = this;
			}
			~Node()
			{
				prev->next = next;
				next->prev = prev;
			}
		};

	public:
		Node* sentinel;
		size_type size;

		LinkedList()
		{
			sentinel = new Node();
			size = 0;
		}

		LinkedList(std::initializer_list<Type> l)
		{
			sentinel = new Node();
			size = 0;
			for(auto i = l.begin(); i != l.end(); i++)
			{
				append(*i);
			}
		}

		LinkedList(const LinkedList& other)
		{
			sentinel = new Node();
			size = 0;
			for(const_iterator i = other.begin(); i != other.end(); i++)
			{
				append(*i);
			}
		}

		LinkedList(LinkedList&& other)
		{
			size = other.size;
			sentinel = other.sentinel;
			other.size = 0;
			other.sentinel = nullptr;
		}

		~LinkedList()
		{
      if(sentinel == nullptr)
        return;
			Node* next;
			for(Node* loop = sentinel->next; loop != sentinel; loop = next)
			{
				next = loop->next;
				delete loop;
			}
		}

		LinkedList& operator=(const LinkedList& other)
		{

			if(this == &other)
				return *this;
			Node* next;
			for(Node* loop = sentinel->next; loop != sentinel; loop = next)
			{
				next = loop->next;
				delete loop;
			}
			size = 0;
			for(const_iterator i = other.begin();i!=other.end();i++)
			{
				append(*i);
			}
			return *this;
		}

		LinkedList& operator=(LinkedList&& other)
		{
			if(this == &other)
				return *this;

			Node* next;
			for(Node* loop = sentinel->next; loop != sentinel; loop = next)
			{
				next = loop->next;
				delete loop;
			}
			sentinel = other.sentinel;
			size = other.size;
			other.size = 0;
			other.sentinel= nullptr;
			return *this;
		}
/*
		void swap(LinkedList& other) noexcept
		{
			using std::swap;
			swap(sentinel->next->prev, other.sentinel->next->prev);
			swap(sentinel->prev->next, other.sentinel->prev->next);
			swap(sentinel->next, other.sentinel->next);
			swap(sentinel->prev, other.sentinel->prev);
			swap(size, other.size);
		}
*/
		bool isEmpty() const
		{
			if(size == 0)
				return 1;
			else
				return 0;
		}

		size_type getSize() const
		{
			return size;
		}

		void append(const Type& item)
		{
			new Node(item, sentinel->prev, sentinel);
			size++;
		}

		void prepend(const Type& item)
		{
			new Node(item, sentinel, sentinel->next);
			size++;
		}

		void insert(const const_iterator& insertPosition, const Type& item)
		{
			if(insertPosition.node == sentinel)
				throw std::out_of_range ("Can't go there");
			new Node(item, insertPosition.node - 1, insertPosition.node);
			size++;
		}

		Type popFirst()
		{
			if(isEmpty() == 1)
				throw std::out_of_range ("List is empty, can't pop anything");
			value_type temporary = sentinel->next->value;
			delete sentinel->next;
			size--;
			return temporary;
		}

		Type popLast()
		{
			if(isEmpty() == 1)
				throw std::out_of_range ("List is empty, can't pop anything");
			value_type temporary = sentinel->next->value;
			delete sentinel->next;
			size--;
			return temporary;
		}

		void erase(const const_iterator& possition)
		{
			if(isEmpty() == 1)
				throw std::out_of_range ("List is empty, can't pop anything");
			if(possition.node == sentinel)
				throw std::out_of_range ("Can't go there");
			delete possition.node;
		}

		void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
		{
			if(isEmpty() == 1)
				throw std::out_of_range ("List is empty, can't pop anything");
			if(firstIncluded.node == sentinel or lastExcluded.node == sentinel)
				throw std::out_of_range ("Can't go there");
			for(auto i = firstIncluded + 1; i != lastExcluded + 1; i++)
			{
				delete i.node;
				size--;
			}
		}

		iterator begin()
		{
			return Iterator(sentinel->next,sentinel);
		}

		iterator end()
		{
			return Iterator(sentinel,sentinel);
		}

		const_iterator cbegin() const
		{
			return ConstIterator(sentinel->next,sentinel);
		}

		const_iterator cend() const
		{
			return ConstIterator(sentinel,sentinel);
		}

		const_iterator begin() const
		{
			return cbegin();
		}

		const_iterator end() const
		{
			return cend();
		}
	};

template <typename Type>
	class LinkedList<Type>::ConstIterator
	{
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = typename LinkedList::value_type;
		using difference_type = typename LinkedList::difference_type;
		using pointer = typename LinkedList::const_pointer;
		using reference = typename LinkedList::const_reference;

		Node* sentinelIterator;
		Node* node;

		explicit ConstIterator()
		{
			node = nullptr;
			sentinelIterator = nullptr;

		}

		explicit ConstIterator(Node *other, Node* other2)
		{
			node = other;
			sentinelIterator = other2;
		}

		reference operator*() const
		{
			if(node == sentinelIterator)
				throw std::out_of_range("Sentinel don't have value");
			return node->value;
		}

		ConstIterator& operator++()
		{
			if(node == sentinelIterator)
				throw std::out_of_range("Can't increase iterator - operator++()");
			node = node->next;
			return *this;
		}

		ConstIterator operator++(int)
		{
			if(node->next == sentinelIterator)
				throw std::out_of_range("Can't increase iterator - operator++(int)");
			ConstIterator result(*this);
			++(*this);
			return result;
		}

		ConstIterator& operator--()
		{
			if(node->prev == sentinelIterator)
				throw std::out_of_range("Can't decrease iterator");
			node = node->prev;
			return *this;
		}

		ConstIterator operator--(int)
		{
			if(node->prev == sentinelIterator)
				throw std::out_of_range("Can't decrease iterator");
			ConstIterator result(*this);
			--(*this);
			return result;
		}

		ConstIterator operator+(difference_type d) const
		{
			Node* temporary = node;
			if(d>0)
			{
				while(d>0)
				{
					temporary = temporary->next;
					d--;
				}
			}
			if(d<0)
			{
				while(d<0)
				{
					temporary = temporary->prev;
					d++;
				}
			}
			return ConstIterator(temporary,sentinelIterator);
		}

		ConstIterator operator-(difference_type d) const
		{
			Node* temporary = node;
			if(d>0)
				while(d>0)
				{
					temporary = temporary->prev;
					d--;
				}
				if(d<0)
					while(d<0)
					{
						temporary = temporary->next;
						d++;
					}
					return ConstIterator(temporary,sentinelIterator);
				}

				bool operator==(const ConstIterator& other) const
				{
					return (node == other.node);
				}

				bool operator!=(const ConstIterator& other) const
				{
					return !(*this == other);
				}
			};

template <typename Type>
			class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
			{
			public:
				using pointer = typename LinkedList::pointer;
				using reference = typename LinkedList::reference;

				explicit Iterator(Node* node= nullptr, Node* sentinelIterator = nullptr) :
				ConstIterator(node, sentinelIterator)
				{}

				Iterator(const ConstIterator& other)
				: ConstIterator(other)
				{}

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

#endif // AISDI_LINEAR_LINKEDLIST_H
