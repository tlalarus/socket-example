//
// Created by minkyung on 23. 3. 28.
//

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdlib.h>
#include <boost/serialization/serialization.hpp>
#include "serialization_helper.hpp"

// external client -> adapter(server/client) ->  engine(server)
typedef enum {
	REQ_IS_STANDBY = 0,
	REQ_IS_READY,
	REQ_IS_TRIGGERED,
	REQ_RESULT,
	REQ_MAX
} eRequestType;

// external client <- adapter(server/client) <- engine(server)
typedef enum {
	RES_STANDBY = 0,
	RES_READY,
	RES_RESULT,
	RES_ERROR,
	RES_MAX
} eResponseType;

typedef struct _payload {
    uint8_t* val;
    size_t length;
} payload;

struct sRequest{
	int code;
	payload body;
};

struct sResponse{
	int code;       // 4byte
	payload body;   // payload.length
};

struct sResponseTest{
	int32_t code;     // 4byte
	bool val1;        // 1byte
	float val2;       // 4byte
	float val3;       // 4byte

private:
	friend class boost::serialization::access;
	template <typename AR> void serialize(AR& ar, const unsigned int ver){
		SerializeMember(ar, code, val1, val2, val3);
	}
};

struct sRequestTest{
	int32_t code;     // 4byte

private:
	friend class boost::serialization::access;
	template <typename AR> void serialize(AR& ar, const unsigned int ver){
		SerializeMember(ar, code);
	}
};

typedef struct _result {
  bool succeed;           // 1 byte
  int32_t matchingCount;  // 4byte
	float prob;             // 4byte
} result_data;

#ifdef __cplusplus
#include <functional>
//using CallbackFn = std::function<void(eResponseType code, struct _payload payloads)>;
using NotiCallbackFn = std::function<void(sResponseTest)>;
#else
typedef void(*CallbackFn) (eResponseType code, struct _payload payloads);
#endif

#endif