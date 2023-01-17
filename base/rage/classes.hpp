#pragma once
#include <cstdint>
#include <utility>

namespace rage
{
    class datBase { public: virtual ~datBase() = default; };
	class pgBase {
	public:
		virtual ~pgBase() = default;
	private:
		void* m_pgunk;
	};
	class scrProgram : public pgBase {
	      public:
	      std::uint8_t** m_code_blocks; // 0x10
		  std::uint32_t m_hash; // 0x18
		  std::uint32_t m_code_size; // 0x1C
		  std::uint32_t m_arg_count; // 0x20
		  std::uint32_t m_local_count; // 0x24
		  std::uint32_t m_global_count; // 0x28
		  std::uint32_t m_native_count; // 0x2C
		  void* m_local_data; // 0x30
		  std::int64_t** m_global_data; // 0x38
		  void** m_native_entrypoints; // 0x40
		  char m_padding6[0x10]; // 0x48
		  std::uint32_t m_name_hash; // 0x58
		  char m_padding7[0x04]; // 0x5C
		  const char* m_name; // 0x60
		  const char** m_strings_data; // 0x68
		  std::uint32_t m_strings_count; // 0x70
		  char m_padding8[0x0C]; // 0x74
		  bool is_valid() const {
			  return m_code_size != 0;
		  }
		  std::uint32_t get_num_code_pages() const {
			  return (m_code_size + 0x3FFF) >> 14;
		  }
		  std::uint32_t get_code_page_size(std::uint32_t page) const {
			  auto num = get_num_code_pages();
			  if (page < num) {
				  if (page == num - 1) return m_code_size & 0x3FFF;
				  return 0x4000;
			  }
			  return 0;
		  }
		  std::uint32_t get_full_code_size() const {
			  auto numPages = get_num_code_pages();
			  if (!numPages) return 0;
			  if (numPages == 1)
				  --numPages;
			  return (numPages * 0x4000) + (m_code_size & 0x3FFF);
		  }
		  std::uint8_t* get_code_page(std::uint32_t page) const {
			  return m_code_blocks[page];
		  }
		  std::uint8_t* get_code_address(std::uint32_t index) const {
			  if (index < m_code_size) return &m_code_blocks[index >> 14][index & 0x3FFF];
			  return nullptr;
		  }
		  const char* get_string(std::uint32_t index) const {
			  if (index < m_strings_count) return &m_strings_data[index >> 14][index & 0x3FFF];
			  return nullptr;
		  }
		  void** get_address_of_native_entrypoint(void* entrypoint) {
			  for (std::uint32_t i = 0; i < m_native_count; ++i) {
				  if (m_native_entrypoints[i] == entrypoint) {
					  return m_native_entrypoints + i;
				  }
			  }
			  return nullptr;
		  }
	 };
	class scrNativeCallContext
	{
	public:
		void reset()
		{
			m_arg_count = 0;
			m_data_count = 0;
		}

		template <typename T>
		void push_arg(T&& value)
		{
			static_assert(sizeof(T) <= sizeof(std::uint64_t));
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(reinterpret_cast<std::uint64_t*>(m_args) + (m_arg_count++)) = std::forward<T>(value);
		}

		template <typename T>
		T& get_arg(std::size_t index)
		{
			static_assert(sizeof(T) <= sizeof(std::uint64_t));
			return *reinterpret_cast<T*>(reinterpret_cast<std::uint64_t*>(m_args) + index);
		}

		template <typename T>
		void set_arg(std::size_t index, T&& value)
		{
			static_assert(sizeof(T) <= sizeof(std::uint64_t));
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(reinterpret_cast<std::uint64_t*>(m_args) + index) = std::forward<T>(value);
		}

		template <typename T>
		T* get_return_value()
		{
			return reinterpret_cast<T*>(m_return_value);
		}

		template <typename T>
		void set_return_value(T&& value)
		{
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(m_return_value) = std::forward<T>(value);
		}
	protected:
		void* m_return_value;
		std::uint32_t m_arg_count;
		void* m_args;
		std::int32_t m_data_count;
		std::uint32_t m_data[48];
	};
	using scrNativeHash = std::uint64_t;
	using scrNativeMapping = std::pair<scrNativeHash, scrNativeHash>;
	using scrNativeHandler = void(*)(scrNativeCallContext*);
	class scrNativeRegistration;
#pragma pack(push, 1)
	class hash_list
	{
	public:
		std::uint64_t* list; //0x0000
		std::int32_t capacity; //0x0008
		char pad_000C[4]; //0x000C
		std::int32_t free; //0x0010
		char pad_0014[4]; //0x0014
		std::uint64_t item_size; //0x0018
		char pad_0020[16]; //0x0020
		std::uint32_t* data; //0x0030
		char pad_0038[16]; //0x0038
		std::int32_t occupied; //0x0048
		const std::uint64_t get(std::int32_t index) const {
			if ((this->data[index >> 5] >> (index & 0x1F)) & 1)
				return this->list[index];
			return NULL;
		}
	}; // Size: 0x004C
#pragma pack(pop)
#pragma pack(push, 1)
	class scrNativeRegistrationTable
	{
		scrNativeRegistration* m_entries[0xFF];
		std::uint32_t m_unk;
		bool m_initialized;
	};
#pragma pack(pop)

	static_assert(sizeof(scrNativeCallContext) == 0xE0);
}
using Void = void;
using Any = int;
using Hash = std::uint32_t;
using Entity = std::int32_t;
using Player = std::int32_t;
using FireId = std::int32_t;
using Interior = std::int32_t;
using Ped = Entity;
using Vehicle = Entity;
using Cam = std::int32_t;
using Object = Entity;
using Pickup = Object;
using Blip = std::int32_t;
using Camera = Entity;
using ScrHandle = Entity;
typedef struct RGB {
	int r, g, b;
} RGB;
typedef struct RGBA {
	int r, g, b, a;
} RGBA;
typedef struct Vector2 {
	float x;
	float y;
} Vector2;
typedef struct
{
	float x;
	DWORD _paddingx;
	float y;
	DWORD _paddingy;
	float z;
	DWORD _paddingz;
} Vector3;
inline std::uint32_t joaat(std::string_view str) {
	std::uint32_t hash = 0;
	for (auto c : str)
		hash += tolower(c), hash += (hash << 10), hash ^= (hash >> 6);
	hash += (hash << 3), hash ^= (hash >> 11), hash += (hash << 15);
	return hash;
}
