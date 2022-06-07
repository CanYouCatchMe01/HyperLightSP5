#pragma once
#include <utility>
#include <functional>

#pragma warning(disable : 26819)
#pragma warning(disable : 4715)

#include "Hashing.h"

namespace CommonUtilities
{
	
	template <typename T>
	struct Hash
	{
		std::size_t operator()(const T& aKey) const noexcept
		{
			return MurmurHash2(&aKey, sizeof(T), 0);
		}
	};

	template <>
	struct Hash<std::string>
	{
		std::size_t operator()(const std::string& aKey) const noexcept
		{
			return std::hash<std::string>{}(aKey);
		}
	};

	template <typename Key, typename Value, typename Hash = Hash<Key>>
	class HashMap
	{
	public:
		HashMap(size_t aCapacity) : myHashes(nullptr), myStates(nullptr), myPairs(nullptr), myCapacity(aCapacity), mySize(0)
		{
			myStates = new State[aCapacity]{State::Empty};
			myHashes = new HashType[aCapacity];
			myPairs = new std::pair<Key, Value>[aCapacity];
		}

		HashMap() : myHashes(nullptr), myStates(nullptr), myPairs(nullptr), myCapacity(0), mySize(0)
		{}

		~HashMap()
		{
			delete[] myStates;
			delete[] myPairs;
			delete[] myHashes;
		}

		size_t Size()
		{
			return mySize;
		}

		bool Insert(const Key& aKey, const Value& aValue)
		{
			if (mySize + 1 > myCapacity/2)
			{
				Resize(myCapacity * 2 + 1);
				myCapacity = myCapacity * 2 + 1;
			}

			HashType hash = Hash{}(aKey);
			size_t index = hash % myCapacity;

			for (;index < myCapacity; ++index)
			{
				if (myStates[index] == State::Empty)
				{
					myStates[index] = State::InUse;
					myHashes[index] = hash;
					myPairs[index] = {aKey, aValue};
					++mySize;
					return true;
				}

				if (myStates[index] == State::InUse)
				{
					if (myHashes[index] == hash) // Remove if key is equal
					{
						myPairs[index] = { aKey, aValue };
						++mySize;
						return true;
					}
				}

				if (myStates[index] == State::Removed)
				{
					if (myHashes[index] == hash) // Only overwrite if the key is equal to removed element
					{
						myStates[index] = State::InUse;
						myPairs[index] = {aKey, aValue};
						++mySize;
						return true;
					}
				}

				if (index == myCapacity-1) // Loop back to beginning if no empty spot was found
					index = 0;

			}

		}

		bool Remove(const Key& aKey)
		{
			HashType hash = Hash{}(aKey);
			size_t index = hash % myCapacity;

			for (; myStates[index] != State::Empty; ++index)
			{
				if (myHashes[index] == hash)
				{
					if (myStates[index] == State::Removed)
						return false;

					myStates[index] = State::Removed;
					--mySize;
					return true;
				}

				if (index == myCapacity-1)
					index = 0;

			}
			return false;

		}

		const Value* Get(const Key& aKey) const
		{
			HashType hash = Hash{}(aKey);
			size_t index = hash % myCapacity;

			for (;index < myCapacity;++index)
			{
				if (myStates[index] == State::InUse)
				{
					if (myHashes[index] == hash)
					{
						return &myPairs[index].second;
					}
				}

				if (myStates[index] == State::Empty)
				{
					return nullptr;
				}

				if (myStates[index] == State::Removed)
				{
					if (myHashes[index] == hash)
					{
						return nullptr;
					}
				}

				if (index == myCapacity-1)
				{
					index = 0;
				}
			}
			return nullptr;

		}

		Value* Get(const Key& aKey)
		{
			HashType hash = Hash{}(aKey);
			size_t index = hash % myCapacity;

			size_t count = 0;
			for (; index < myCapacity; ++index, ++count)
			{
				if (myStates[index] == State::InUse)
				{
					if (myHashes[index] == hash)
					{
						return &myPairs[index].second;
					}
				}

				if (myStates[index] == State::Empty)
				{
					return nullptr;
				}

				if (myStates[index] == State::Removed)
				{
					if (myHashes[index] == hash)
					{
						return nullptr;
					}
				}

				if (index == myCapacity - 1)
				{
					index = 0;
				}
				if (count >= myCapacity)
				{
					return nullptr;
				}
			}
			return nullptr;
		}
		
	private:
		inline void Resize(size_t aSize)
		{
			State* tmpStates = new State[aSize]{State::Empty};
			HashType* tmpHashes = new HashType[aSize];
			std::pair<Key, Value>* tmpPairs = new std::pair<Key, Value>[aSize];
			
			for (size_t i = 0; i < myCapacity; i++)
			{
				if (myStates[i] == State::Empty)
					continue;

				if (myStates[i] == State::InUse)
				{
					size_t index = myHashes[i] % aSize;
					for (; index < aSize; ++index)
					{
						if (tmpStates[index] == State::Empty)
						{
							tmpStates[index] = State::InUse;
							tmpHashes[index] = myHashes[i];
							tmpPairs[index] = myPairs[i];
							break;
						}

						if (index == aSize-1)
							index = 0;
					}
				}

				if (myStates[i] == State::Removed)
				{
					size_t index = myHashes[i] % aSize;
					for (; index < aSize; ++index)
					{
						if (tmpStates[index] == State::Empty)
						{
							tmpStates[index] = State::Removed;
							tmpHashes[index] = myHashes[i];
						}
						if (index == aSize - 1)
							index = 0;
					}
				}
			}

			delete[] myPairs;
			myPairs = tmpPairs;
			delete[] myHashes;
			myHashes = tmpHashes;
			delete[] myStates;
			myStates = tmpStates;
		}
		enum class State : uint8_t
		{
			Empty,
			InUse,
			Removed
		};

		using HashType = size_t;
		size_t myCapacity;
		size_t mySize;

		State* myStates;
		HashType* myHashes;
		std::pair<Key, Value>* myPairs;
	};
}