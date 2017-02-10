/*
 * DiagManager.cpp
 *
 *  Created on: 07/02/2017
 *      Author: Lucas Teske
 */

#include "DiagManager.h"

DiagManager::DiagManager(float interval) : buffer(DM_BUFFER_SIZE), interval(interval) {
	running = true;
	mainThread = new std::thread(&DiagManager::threadLoop, this);
}

DiagManager::~DiagManager() {
	if (this->running) {
		this->running = false;
		this->mainThread->join();
	}
}

void DiagManager::threadLoop() {
	std::vector<char> data(1024);

	std::chrono::milliseconds period((uint32_t) round(interval * 1000));
	t0 = std::chrono::high_resolution_clock::now() - period;

	socket.Bind(9001);

	while (running) {
		if ((std::chrono::high_resolution_clock::now() - t0) >= period) {
			int size = buffer.size();
			if (size >= 1024) {
				buffer.unsafe_lockMutex();
				for (int i=0; i<1024; i++) {
					float val = buffer.unsafe_takeSample();
					val *= 128.f;
					val = val > 127 ? 127 : val;
					val = val < -128 ? -128 : val;
					data[i] = static_cast<char>(val);
				}
				buffer.unsafe_unlockMutex();

				// TODO: Send to 2+ clients
				try {
					socket.SendTo(&data[0], 1024, SatHelper::IPAddress::localhost, 9000);
				} catch (SatHelper::SocketException &e) {
					std::cerr << "Error sending DiagSamples" << std::endl;
				}
				t0 = std::chrono::high_resolution_clock::now();
			}
		}
		usleep(100);
	}
}

void DiagManager::addSamples(const float *data, int length) {
	if(!buffer.isOverflow()) {
		buffer.addSamples(data, length);
	}
}
