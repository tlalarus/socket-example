//
// Created by minkyung on 23. 3. 31.
//

#ifndef SOCKET_SERIALIZATION_HELPER_HPP
#define SOCKET_SERIALIZATION_HELPER_HPP

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <string>
#include <vector>

typedef boost::iostreams::back_insert_device<std::vector<char>> BOOST_STREAM_DEVICE;
typedef boost::iostreams::basic_array_source<char> BOOST_ARRAY_DEVICE;
typedef boost::iostreams::stream<BOOST_STREAM_DEVICE> OUT_STREAM_TYPE;
typedef boost::iostreams::stream_buffer<BOOST_ARRAY_DEVICE> IN_STREAM_TYPE;

template <typename T> inline void Serialize(T const& obj, std::vector<char>& buf){
	OUT_STREAM_TYPE out_s_(buf);
	boost::archive::binary_oarchive out_ar_(out_s_);
	out_ar_ << obj;
	out_s_.flush();
}

template <typename T> inline bool Deserialize(T& obj, char const* p_raw, size_t size){
	try {
		IN_STREAM_TYPE in_s_(p_raw, size);
		boost::archive::binary_iarchive in_ar_(in_s_);
		in_ar_ >> obj;
	}
	catch(boost::archive::archive_exception const&){
		return false;
	}
}

template <typename AR> class serialize_helper{
		AR & ar_;
	public:
		explicit serialize_helper(AR& ar)
		: ar_(ar) {};

		template <typename T>
		void serialize_data(T& data){
			ar_ & data;
		}

		template <typename T, typename ... args>
		void serialize_data(T& first, args&... rest){
			ar_ & first;
			serialize_data(rest...);
		}
	};

template <typename AR, typename ... ARGS>
inline void SerializeMember(AR& ar, ARGS & ... rest){
	serialize_helper<AR>(ar).serialize_data(rest...);
}

#endif //SOCKET_SERIALIZATION_HELPER_HPP
