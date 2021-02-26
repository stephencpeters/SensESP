#include "vedirect_helper.h"

#include "sensesp_app.h"
#include "signalk/signalk_output.h"

#include "sensesp.h"

VEDirectInput::VEDirectInput(Stream* rx_stream)
    : Sensor() {
  rx_stream_ = rx_stream;

  vedirect_parser_.add_sentence_parser(new SentenceParserV(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserVPV(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserPPV(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserI(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserIL(&vedirect_data_));

}

void VEDirectInput::enable() {
  // enable reading the serial port
  app.onAvailable(*rx_stream_, [this]() {
    while (rx_stream_->available()) {
      vedirect_parser_.handle(rx_stream_->read());
    }
  });
}


VEDirectInput* setup_vedirect(Stream* rx_stream) {
  VEDirectInput* victronDevice = new VEDirectInput(rx_stream);
  victronDevice->vedirect_data_.batteryVoltage.connect_to(
      new SKOutputNumber("electrical.batteries.mainBattery.voltage", ""));
  victronDevice->vedirect_data_.panelVoltage.connect_to(
      new SKOutputNumber("electrical.solar.panel.voltage", ""));
  victronDevice->vedirect_data_.panelPower.connect_to(
      new SKOutputNumber("electrical.solar.panel.power", ""));
  victronDevice->vedirect_data_.batteryCurrent.connect_to(
      new SKOutputNumber("electrical.batteries.mainBattery.current", ""));
  victronDevice->vedirect_data_.loadCurrent.connect_to(
      new SKOutputNumber("electrical.batteries.loadCurrent.current", ""));

  return victronDevice;
}

