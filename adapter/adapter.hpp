//
// Created by minkyung on 23. 3. 28.
//

#ifndef SOCKET_ADAPTER_HPP
#define SOCKET_ADAPTER_HPP

#include <iostream>

class Adapter {
public:
	Adapter() = default;
	~Adapter() = default;

	bool create();
	bool start();
	bool stop();
	bool destroy();
	bool send();

};


#endif //SOCKET_ADAPTER_HPP
