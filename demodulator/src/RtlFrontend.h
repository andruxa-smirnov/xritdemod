/*
 * RtlFrontend.h
 *
 *  Created on: 25/02/2017
 *      Author: Lucas Teske
 */

#ifndef SRC_RTLFRONTEND_H_
#define SRC_RTLFRONTEND_H_

#include <thread>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <functional>
#include <SatHelper/exceptions.h>
#include "FrontendDevice.h"

extern "C" {
#include <rtl-sdr.h>
}

class RtlFrontend: public FrontendDevice {
private:
	const static std::vector<uint32_t> supportedSampleRates;

	uint32_t sampleRate;
	uint32_t centerFrequency;
	int deviceId;
	rtlsdr_dev_t *device;
	std::string deviceName;
	float lut[256];
	float alpha;
	float iavg;
	float qavg;
	std::thread *mainThread;
	uint8_t lnaGain;
	uint8_t vgaGain;
	uint8_t mixerGain;
	bool agc;
	std::function<void(void *data, int length, int type)> cb;

	void threadWork();
	void refreshGains();
	void internalCallback(unsigned char *data, unsigned int length);
protected:
	static void rtlCallback(unsigned char *data, unsigned int length, void *ctx);
public:
	RtlFrontend(int deviceId);
	virtual ~RtlFrontend();

	uint32_t SetSampleRate(uint32_t sampleRate) override;
	uint32_t SetCenterFrequency(uint32_t centerFrequency) override;
	const std::vector<uint32_t>& GetAvailableSampleRates() override;
	void Start() override;
	void Stop() override;
	void SetAGC(bool agc) override;

	void SetLNAGain(uint8_t value) override;
	void SetVGAGain(uint8_t value) override;
	void SetMixerGain(uint8_t value) override;
	void SetBiasT(uint8_t value) override;
	uint32_t GetCenterFrequency() override;

	const std::string &GetName() override;

	uint32_t GetSampleRate() override;

	void SetSamplesAvailableCallback(std::function<void(void*data, int length, int type)> cb) override;
};

#endif /* SRC_RTLFRONTEND_H_ */
