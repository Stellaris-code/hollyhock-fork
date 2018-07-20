#pragma once
#include <stdint.h>

// No point in documenting these macros. If people are trying to use them,
// they're doing something weird and I don't want to help them :)
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
 * Create a vtable function entry.
 * 
 * Makes entries for the offset (suffix @c _Offset)
 * (added to @c self when the function is called), an unused padding entry, and
 * an entry for the function pointer.
 * 
 * @param return_type The return type of the function.
 * @param name The name of the function.
 * @param ... The arguments to the function (type and name).
 */
#define VTABLE_ENTRY(return_type, name, ...) int32_t name##_Offset; \
                                             uint32_t name##_Unused; \
                                             return_type (*name)(__VA_ARGS__)

/**
 * Call a function within an external vtable.
 * 
 * @param vtable The vtable containing the function.
 * @param name The name of the function to call.
 * @param self The pointer to the object to call the function on.
 * @param ... The arguments to the function.
 */
#define VTABLE_CALL_EXT(self, vtable, name, ...) (vtable)->name( \
	reinterpret_cast<decltype(self)>( \
		reinterpret_cast<uint8_t *>(self) + (vtable)->name##_Offset \
	), \
	## __VA_ARGS__ \
)

/**
 * Call a function within a vtable.
 * 
 * @param vtable The name of the vtable inside @p self.
 * @param name The name of the function to call.
 * @param self The pointer to the object to call the function on.
 * @param ... The arguments to the function.
 */
#define VTABLE_CALL(self, vtable, name, ...) VTABLE_CALL_EXT( \
	self, self->vtable, name, ## __VA_ARGS__ \
)
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
