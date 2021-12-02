#ifndef DOWNLOAD_HPP_
#define DOWNLOAD_HPP_

#include <string>

class Download {
  private:
    const std::string m_domain;
    const std::string m_path;
    const std::string m_filename;
    size_t m_total = 1;
    bool m_done    = false;

  public:
    Download(std::string domain, std::string path, std::string filename);
    void start_download();
    void start_process();
    void check_progress();
    size_t bytes_downloaded();
};

#endif  // DOWNLOAD_HPP_
