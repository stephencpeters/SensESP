#include <map>

#include "Arduino.h"
#include "system/observablevalue.h"

constexpr int INPUT_BUFFER_LENGTH = 250;
constexpr int MAX_TERMS = 25;

struct VEDirectData {
  ObservableValue<float> batteryVoltage;
  ObservableValue<float> panelVoltage;
  ObservableValue<float> panelPower;
  ObservableValue<float> batteryCurrent;
  ObservableValue<float> loadCurrent;
  ObservableValue<float> loadOutputState;
};

class SentenceParser {
 public:
  SentenceParser(VEDirectData* vedirect_data);
  virtual void parse(char* buffer, int term_offsets[], int num_terms) = 0;
  virtual void parse(char* buffer, int term_offsets[], int num_terms,
                     std::map<String, SentenceParser*>& sentence_parsers) {
    parse(buffer, term_offsets, num_terms);
  }
  virtual const char* sentence() = 0;

 protected:
  VEDirectData* vedirect_data;

 private:
};


class SentenceParserV : public SentenceParser {
 public:
  SentenceParserV(VEDirectData* vedirect_data) : SentenceParser{vedirect_data} {}
  void parse(char* buffer, int term_offsets[], int num_terms) override final {}
  void parse(char* buffer, int term_offsets[], int num_terms,
             std::map<String, SentenceParser*>& sentence_parsers);
  const char* sentence() { return "V"; }

 private:
};


class SentenceParserVPV : public SentenceParser {
 public:
  SentenceParserVPV(VEDirectData* vedirect_data) : SentenceParser{vedirect_data} {}
  void parse(char* buffer, int term_offsets[], int num_terms) override final {}
  void parse(char* buffer, int term_offsets[], int num_terms,
             std::map<String, SentenceParser*>& sentence_parsers);
  const char* sentence() { return "VPV"; }

 private:
};


class SentenceParserPPV : public SentenceParser {
 public:
  SentenceParserPPV(VEDirectData* vedirect_data) : SentenceParser{vedirect_data} {}
  void parse(char* buffer, int term_offsets[], int num_terms) override final {}
  void parse(char* buffer, int term_offsets[], int num_terms,
             std::map<String, SentenceParser*>& sentence_parsers);
  const char* sentence() { return "PPV"; }

 private:
};


class SentenceParserI : public SentenceParser {
 public:
  SentenceParserI(VEDirectData* vedirect_data) : SentenceParser{vedirect_data} {}
  void parse(char* buffer, int term_offsets[], int num_terms) override final {}
  void parse(char* buffer, int term_offsets[], int num_terms,
             std::map<String, SentenceParser*>& sentence_parsers);
  const char* sentence() { return "I"; }

 private:
};


class SentenceParserIL : public SentenceParser {
 public:
  SentenceParserIL(VEDirectData* vedirect_data) : SentenceParser{vedirect_data} {}
  void parse(char* buffer, int term_offsets[], int num_terms) override final {}
  void parse(char* buffer, int term_offsets[], int num_terms,
             std::map<String, SentenceParser*>& sentence_parsers);
  const char* sentence() { return "IL"; }

 private:
};


class VEDirectParser {
 public:
  VEDirectParser();
  void handle(char c);
  void add_sentence_parser(SentenceParser* parser);

 private:
  void (VEDirectParser::*current_state)(char);
  void state_start(char c);
  void state_in_term(char c);
  void state_in_checksum(char c);
  // current sentence
  char buffer[INPUT_BUFFER_LENGTH];
  // offset for each sentence term in the buffer
  int term_offsets[MAX_TERMS];
  // pointer for the next character in buffer
  int cur_offset;
  // pointer for the current term in buffer
  int cur_term;
  int parity;
  bool validate_checksum();
  std::map<String, SentenceParser*> sentence_parsers;
};

