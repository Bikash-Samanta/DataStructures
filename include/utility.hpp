#pragma once
#ifndef __UTILITY__
#define __UTILITY__

#include <cstdlib>
#include <cstring>
#include <new>
#include <type_traits>
#include <utility>
#include <initializer_list>
#include <typeinfo>
#include <memory>

namespace alpha {
	
	template<class _Ty>
	struct less {
		using ConstReference = std::conditional_t<std::is_fundamental_v<_Ty> || std::is_pointer_v<_Ty>, const _Ty, const _Ty&>;
		bool operator()(ConstReference _Right, ConstReference _Left) {
			return _Right < _Left;
		}

	};
	template<class _Ty>
	struct greater {
		using ConstReference = std::conditional_t<std::is_fundamental_v<_Ty> || std::is_pointer_v<_Ty>, const _Ty, const _Ty&>;
		bool operator()(ConstReference _Right, ConstReference _Left) {
			return _Right > _Left;
		}
	};

	template<class _Ty>
	inline constexpr _Ty& maximum(_Ty& a, _Ty& b) {
		return a < b ? b : a;
	}
	template<class _Ty, class _Comparator>
	inline constexpr _Ty& maximum(_Ty& a, _Ty& b, _Comparator comp) {
		return comp(a, b) ? b : a;
	}

	template <typename U>
	inline constexpr auto _Unwrappable(int) -> decltype(std::declval<U>()._Unwrapped(), bool{}) {
		return true;
	}
	template <typename U>
	inline constexpr auto _Orderable(int) -> decltype(std::declval<U>().operator>(U()), bool{}) {
		return true;
	}

	template <typename U>
	inline constexpr bool _Unwrappable(...) {
		return false;
	}
	template <typename U>
	inline constexpr bool _Orderable(...) {
		return false;
	}

	template <typename _Ty>
	inline constexpr bool _Unwrappable_v = _Unwrappable<_Ty>(0);
	template <typename _Ty>
	inline constexpr bool _Orderable_v = _Orderable<_Ty>(0);

	template <class _Iter>
	[[nodiscard]] constexpr decltype(auto) _GetUnwrapped(_Iter&& _It) noexcept {
		if constexpr (std::is_pointer_v<std::decay_t<_Iter>>) {
			return _It + 0;
		}
		else if constexpr (_Unwrappable_v<_Iter>) {
			return static_cast<_Iter&&>(_It)._Unwrapped();
		}
		else {
			return static_cast<_Iter&&>(_It);
		}
	}
}


namespace alpha {
	template<class _Ty, class... _Args>
	inline void construct(_Ty* _Ptr, _Args&&... _Val) {
		new(_Ptr) _Ty(std::forward<_Args>(_Val)...);
	}
}
namespace alpha {
	template<class _Ty> using initializer = std::initializer_list<_Ty>;
	template <class _Ty> const char* type_name = typeid(_Ty).name();
}

#endif