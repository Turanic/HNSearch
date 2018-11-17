#include "tsv_parser.hh"
#include <charconv>
#include <ctime>
#include <stdexcept>

namespace tools
{
  TSVParser::TSVParser(const std::string& file,
                       std::string_view minimum_timestamp,
                       std::string_view maximum_timestamp)
      : reader(file)
  {
    if (reader.fail())
      throw std::runtime_error{"TSV Parser can't open file"};

    std::time_t ts;
    std::from_chars(minimum_timestamp.data(),
                    minimum_timestamp.data() + minimum_timestamp.size(),
                    ts);
    min_ts_ = std::chrono::system_clock::from_time_t(ts);
    std::from_chars(maximum_timestamp.data(),
                    maximum_timestamp.data() + maximum_timestamp.size(),
                    ts);
    max_ts_ = std::chrono::system_clock::from_time_t(ts);
  }

  std::tuple<TSVParser::timepoint_t, std::string, bool>
  TSVParser::read_element()
  {
    std::string url;
    timepoint_t time_point;

    do
    {
      std::string timestamp_str;
      reader >> timestamp_str >> url;

      if (reader.eof())
        break;

      std::time_t ts;
      std::from_chars(timestamp_str.data(),
                      timestamp_str.data() + timestamp_str.size(),
                      ts);
      time_point = std::chrono::system_clock::from_time_t(ts);

    } while (time_point < min_ts_ || time_point > max_ts_);

    return std::make_tuple(time_point, url, reader.eof());
  }
} // namespace tools
