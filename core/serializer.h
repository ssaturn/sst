#pragma once

#include "buffer.h"


namespace sst
{
	class basic_serializer abstract
	{
		
	};
	
	class serializer : public memory::buffer::buffer
	{
	public:
		explicit serializer(const uint32 buf_size)
			: buffer(nullptr, buf_size)
		{
			
		}
		virtual ~serializer() = default;

		template<typename DataTy>
		void serialize(DataTy& val);

		template<typename DataTy, std::size_t N>
		void serialize(DataTy (&val)[N]) requires std::is_array_v<DataTy[N]>;

		template<typename DataTy>
		void serialize(DataTy& val) requires std::is_enum_v<DataTy>;

		template<typename DataTy>
		void serialize(std::vector<DataTy>& vec);

	protected:
		virtual void Serialize(bool& val) = 0;
		virtual void Serialize(int8& val) = 0;
		virtual void Serialize(uint8& val) = 0;
		virtual void Serialize(int16& val) = 0;
		virtual void Serialize(uint16& val) = 0;
		virtual void Serialize(int32& val) = 0;
		virtual void Serialize(uint32& val) = 0;
		virtual void Serialize(int64& val) = 0;
		virtual void Serialize(uint64& val) = 0;
		virtual void Serialize(float& val) = 0;
		virtual void Serialize(double& val) = 0;
		virtual void Serialize(std::string& val) = 0;
		virtual void Serialize(std::wstring& val) = 0;

		bool overran_{ false };
		uint32 offset_{ 0 };
	};

	template <typename DataTy>
	void serializer::serialize(DataTy& val)
	{
		Serialize(val);
	}

	template <typename DataTy, std::size_t N>
	void serializer::serialize(DataTy (& val)[N]) requires std::is_array_v<DataTy[N]>
	{
		for (auto& i : val)
		{
			Serialize(i);
		}
	}

	template <typename DataTy>
	void serializer::serialize(DataTy& val) requires std::is_enum_v<DataTy>
	{
		Serialize(static_cast<std::underlying_type_t<DataTy>>(val));
	}

	template <typename DataTy>
	void serializer::serialize(std::vector<DataTy>& vec)
	{
		constexpr size_t max_vector_count = 1000;

		int32 count = vec.size();
		assert(size > max_vector_count);

		Serialize(count);
		for (auto& val : vec)
		{
			Serialize(val);
		}
	}

	class oserializer final : public serializer
	{
	protected:
		void Serialize(bool& val) override { Read(&val, sizeof(val)); }
		void Serialize(int8& val) override { Read(&val, sizeof(val)); }
		void Serialize(uint8& val) override { Read(&val, sizeof(val)); }
		void Serialize(int16& val) override { Read(&val, sizeof(val)); }
		void Serialize(uint16& val) override { Read(&val, sizeof(val)); }
		void Serialize(int32& val) override { Read(&val, sizeof(val)); }
		void Serialize(uint32& val) override { Read(&val, sizeof(val)); }
		void Serialize(int64& val) override { Read(&val, sizeof(val)); }
		void Serialize(uint64& val) override { Read(&val, sizeof(val)); }
		void Serialize(float& val) override { Read(&val, sizeof(val)); }
		void Serialize(double& val) override { Read(&val, sizeof(val)); }
		void Serialize(std::string& val) override
		{
			uint16 str_size = 0;
			Read(&str_size, sizeof(str_size));
			val.resize(str_size / sizeof(std::string::value_type));
			Read(val.data(), str_size);
		}
		void Serialize(std::wstring& val) override
		{
			uint16 str_size = 0;
			Read(&str_size, sizeof(str_size));
			val.resize(str_size / sizeof(std::wstring::value_type));
			Read(val.data(), str_size);
		}

	private:
		void Read(void* val, const uint32 val_size)
		{
			if (overran_)
			{
				return;
			}

			if (offset_ + val_size <= size)
			{
				memcpy_s(val, val_size, &buf[offset_], size - offset_);
				offset_ += size;
			}
			else
			{
				overran_ = true;
			}
		}
	};

	class iserializer final : public serializer
	{
	protected:
		void Serialize(bool& val) override { Write(&val, sizeof(val)); }
		void Serialize(int8& val) override { Write(&val, sizeof(val)); };
		void Serialize(uint8& val) override { Write(&val, sizeof(val)); };
		void Serialize(int16& val) override { Write(&val, sizeof(val)); };
		void Serialize(uint16& val) override { Write(&val, sizeof(val)); };
		void Serialize(int32& val) override { Write(&val, sizeof(val)); };
		void Serialize(uint32& val) override { Write(&val, sizeof(val)); };
		void Serialize(int64& val) override { Write(&val, sizeof(val)); };
		void Serialize(uint64& val) override { Write(&val, sizeof(val)); };
		void Serialize(float& val) override { Write(&val, sizeof(val)); };
		void Serialize(double& val) override { Write(&val, sizeof(val)); };
		void Serialize(std::string& val) override
		{
			uint16 str_size = static_cast<uint16>(val.length() * sizeof(std::string::value_type));
			Write(&str_size, sizeof(str_size));
			Write(val.data(), str_size);
		}
		void Serialize(std::wstring& val) override
		{
			uint16 str_size = static_cast<uint16>(val.length() * sizeof(std::wstring::value_type));
			Write(&str_size, sizeof(str_size));
			Write(val.data(), str_size);
		}
		

	private:
		void Write(void* val, const uint32 val_size)
		{
			if (overran_)
			{
				return;
			}

			if (offset_ + val_size <= size)
			{
				memcpy_s(&buf[offset_], size - offset_, val, val_size);
				offset_ += size;
			}
			else
			{
				overran_ = true;
			}
		}
		
	};
}
