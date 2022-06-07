#include <cstdio>

template <typename T>
class SparseSet
{
public:
    using IdType = size_t;

    SparseSet() : dense(nullptr), sparse(nullptr), size(0), capacity(0), sparse_capacity(0), mirror(nullptr)
    {}

    ~SparseSet()
    {
        delete[] mirror;
        delete[] dense;
        delete[] sparse;
    }

    T& operator[](IdType index)
    {
        return mirror[index];
        //return dense[index].id;
    }

    void Insert(T val, IdType id)
    {
        Grow(id);

        mirror[size] = val;
        dense[size] = id;
        sparse[id] = size++;
    }

    void Remove(size_t id)
    {
        if (!Has(id)) return;

        size_t denseIndex = sparse[id];
        std::swap(dense[size - 1], dense[denseIndex]);
        sparse[dense[denseIndex]] = denseIndex;
        --size;
    }

    inline bool Has(IdType id) const
    {
        size_t denseIndex = sparse[id];
        return denseIndex < size&& dense[denseIndex] == id;
    }

    size_t Size() const
    {
        return size;
    }

private:
    inline void Grow(IdType id)
    {
        if (size >= capacity)
        {
            capacity = capacity * 2 + 1;
            IdType* tmp = new IdType[capacity];
            T* tmpmirror = new T[capacity];
            memcpy(tmp, dense, size * sizeof(IdType));
            std::move(mirror, mirror + size, tmpmirror);
            delete[] dense;
            delete[] mirror;
            dense = tmp;
            mirror = tmpmirror;
        }

        if (id >= sparse_capacity)
        {
            sparse_capacity = id * 2 + 1;
            IdType* tmp = new IdType[sparse_capacity];
            memcpy(tmp, sparse, id * sizeof(IdType));
            //std::copy(sparse, sparse + sparse_size, tmp);
            delete[] sparse;
            sparse = tmp;
        }
    }

    struct Container
    {
        T data;
        size_t id;
    };

    size_t size;
    size_t capacity;

    size_t sparse_capacity;

    T* mirror;
    IdType* dense;
    IdType* sparse;
};