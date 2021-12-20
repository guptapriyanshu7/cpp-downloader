#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "download.hpp"

#include <httplib.h>

#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <thread>

Download::Download(std::string domain, std::string path, std::string filename)
  : m_domain(domain),
    m_path(path),
    m_filename(filename) {
  std::ofstream m_file(filename);
  m_file.close();
}

size_t Download::bytes_downloaded() {
  return std::filesystem::file_size(m_filename);
}

void Download::start_download() {
  httplib::Client cli(m_domain);

  cli.set_follow_location(true);

  std::ofstream m_file(m_filename);

  cli.Get(
    m_path.data(),
    [&](const httplib::Response& response) {
      m_total = std::stoul(response.get_header_value("Content-Length"));
      return true;
    },
    [&](const char* data, size_t data_length) {
      std::string str(data, data_length);
      m_file << str;
      return true;
    });

  m_file.close();
  m_done = true;
}

void Download::start_process() {
  // std::thread th(&Download::start_download, this);
  // launch on a different thread explicitly
  // benefit: no need to manage thread on our own
  auto fut = std::async(std::launch::async, &Download::start_download, this);
  check_progress();
  // th.join();
}

void Download::check_progress() {
  auto last_time = false;
  while (!m_done || last_time) {
    std::cout << "\r[";
    auto bytes_done = bytes_downloaded();
    auto progress   = 40 * bytes_done / m_total;
    for (size_t i = 0; i < 40; i++) {
      if (i < progress) {
        std::cout << "=";
      } else if (i == progress) {
        std::cout << ">";
      } else {
        std::cout << " ";
      }
    }
    std::cout << "] ";
    std::cout << bytes_done / 1000 << "/" << m_total / 1000 << " kB";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    last_time = m_done && !last_time;
  }
  std::cout << std::endl;
}
