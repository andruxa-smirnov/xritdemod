/*
 * ChannelWritter.cpp
 *
 *  Created on: 07/11/2016
 *      Author: Lucas Teske
 */

#include "ChannelWriter.h"
#include <sstream>
#include <SatHelper/sathelper.h>

ChannelWriter::ChannelWriter(std::string baseFolder) {
    this->baseFolder = baseFolder;
}

void ChannelWriter::writeChannel(uint8_t *data, int size, uint16_t vcid) {
  std::stringstream ss;
  SatHelper::Tools::makedir(baseFolder);
  ss << baseFolder << "/channel_" << vcid << ".bin";
  FILE *f = fopen(ss.str().c_str(), "a+");
  fwrite(data, static_cast<size_t>(size), 1, f);
  fclose(f);
}

void ChannelWriter::dumpCorruptedPacket(uint8_t *data, int size, int type) {
  std::stringstream ss;
  SatHelper::Tools::makedir(baseFolder);
  ss << baseFolder << "/errors/";
  SatHelper::Tools::makedir(ss.str());
  ss << SatHelper::Tools::getTimestamp() << "-" << size << "-" << type;
  FILE *f = fopen(ss.str().c_str(), "wb");
  // For more visibility, I write 6 times
  for (int i=0; i<6;i++) {
    fwrite(data, static_cast<size_t>(size), 1, f);
  }
  fclose(f);
}

void ChannelWriter::dumpDebugData(uint8_t *data, int size, int type) {
    std::stringstream ss;
    SatHelper::Tools::makedir(baseFolder);
    ss << baseFolder << "/debug/";
    SatHelper::Tools::makedir(ss.str());
    ss << "debug-" << size << "-" << type;
    FILE *f = fopen(ss.str().c_str(), "a+");
    fwrite(data, static_cast<size_t>(size), 1, f);
    fclose(f);
}

void ChannelWriter::dumpCorruptedPacketStatistics(uint16_t viterbiErrors, uint8_t syncCorrelation, int32_t *rsErrors) {
  std::stringstream ss;
  SatHelper::Tools::makedir(baseFolder);
  ss << baseFolder << "/errors/";
  SatHelper::Tools::makedir(ss.str());
  ss << SatHelper::Tools::getTimestamp() << ".txt";
  FILE *f = fopen(ss.str().c_str(), "wb");
  ss.str(std::string());
  ss.clear();
  ss << "Viterbi Errors: " << viterbiErrors << std::endl;
  ss << "Sync Correlation: " << (uint32_t) syncCorrelation << std::endl;
  ss << "Reed Solomon Errors: " << rsErrors[0] << ", " << rsErrors[1] << ", " << rsErrors[2] << ", " << rsErrors[3] << std::endl;
  std::string data = ss.str();
  fwrite(data.c_str(), data.size(), 1, f);
  fclose(f);
}
