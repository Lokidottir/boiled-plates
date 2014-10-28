#ifndef BOP_UTIL_LOGIC_HELPERS
#define BOP_UTIL_LOGIC_HELPERS

namespace bop {
	namespace logic {
		template<class T, class C>
		bool oneOf(T to_test, C container) {
			for (auto& within : container) {
				if (within == to_test) return true;
			}
			return false;
		}
	};
};

#endif
