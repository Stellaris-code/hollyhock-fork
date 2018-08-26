#pragma once
#include <stdint.h>

// No point in documenting these macros. If people are trying to use them,
// they're doing something weird and I don't want to help them :)
// If you need to know what they do, look at how they're used.
/// @cond INTERNAL
/**
 * Create @p n fake vtable function entries.
 * 
 * The parameter @p x is required as the fake entry needs a name. It is
 * recommended to simply increment from 0 as fake entries are required.
 * 
 * @param n The number of fake vtable entries to create.
 * @param x An integer, unique within the vtable.
 */
#define VTABLE_FAKE_ENTRY(n, x) uint32_t fakeentry##x[n * 3]

/**
 * Represents an entry in a vtable.
 * 
 * The function can be called with the () operator. The first argument must be
 * the object to call the function on, followed by the arguments to the
 * function.
 * 
 * @tparam TReturn The return type of the function.
 * @tparam ...TArgs The types of the function's arguments.
 */
template<typename TReturn, typename ...TArgs>
struct VTableFunction {
	int32_t offset;
	uint32_t unused;
	TReturn (*func)(void *self, TArgs...);

	TReturn operator()(void *self, TArgs... args) {
		void *self2 = reinterpret_cast<void *>(
			reinterpret_cast<uint8_t *>(self) + offset
		);

		return func(self2, args...);
	}
};
/// @endcond

/**
 * Wraps an internal class from the fx-CP400.
 */
class Wrapped {
public:
	/**
	 * Returns a pointer to the internal class.
	 * 
	 * @tparam T The type of the pointer to return. Can be @c void.
	 * @return A pointer to the internal class.
	 */
	template <typename T>
	T *GetWrapped() {
		return static_cast<T *>(m_wrapped);
	}

protected:
	Wrapped() = default;
	~Wrapped() = default;

	// Since we use the trick with the vtable to overide functions (@c .me), we
	// can't let the object's address change. Prevent that by deleting the copy
	// constructor and assignment operator.
	Wrapped(Wrapped const &) = delete;
	void operator=(Wrapped const &) = delete;

	/// A pointer to the wrapped class.
	void *m_wrapped;
};

/**
 * Utility class which is the parent of all GUI element classes.
 */
class GUIElement : public Wrapped {};
