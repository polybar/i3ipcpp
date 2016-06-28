#pragma once

#include <sstream>

namespace i3ipc
{
  class AutoStringStream
  {
    private:
      std::stringstream m_stream;

    public:
      AutoStringStream() : m_stream() {}
      virtual ~AutoStringStream() {}

      template<typename T>
      AutoStringStream& operator<<(const T&  arg)
      {
        m_stream << arg;
        return *this;
      }

      AutoStringStream& operator<<(const bool arg)
      {
        m_stream << (arg ? "true" : "false");
        return *this;
      }

      operator std::string() const {
        return m_stream.str();
      }

      const std::string to_string() const {
        return m_stream.str();
      }
  };

  typedef AutoStringStream auss_t;
}
