//
// Created by minkyung on 23. 3. 28.
//

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdlib.h>
#include <boost/serialization/serialization.hpp>

// external client -> adapter(server/client) ->  engine(server)
typedef enum {
	REQ_IS_READY = 0,
	REQ_RESULT,
	REQ_MAX
} eRequest;

// external client <- adapter(server/client) <- engine(server)
typedef enum {
	RES_READY = 0,
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
	std::string body;
	template<typename Ar> void serialize(Ar& ar, unsigned) {ar& code; ar& body;}
};

typedef struct _result {
  bool succeed;           // 1 byte
  int32_t matchingCount;  // 4byte
	float prob;             // 4byte
} result_data;

#ifdef __cplusplus
#include <functional>
using CallbackFn = std::function<void(eResponseType code, struct _payload payloads)>;
#else
typedef void(*CallbackFn) (eResponseType code, struct _payload payloads);
#endif

#endif