#include <cstdint>


template <typename T = uint64_t>
class CUIDProvider
{
public:
	T Provide()
	{
		return ++nextID;
	}

public:
	CUIDProvider() : nextID(0)
	{}

private:
	T nextID;
};