#include <iostream>

#include "download.hpp"

int main(int argc, char* argv[]) {
  std::string url = argv[1];

  size_t i = 0, cnt = 0;
  std::string domain, path;

  while (i < url.size()) {
    if (url[i] == '/') cnt++;
    if (cnt == 3) break;
    domain += url[i];
    i++;
  }

  path = url.substr(i);
  if (path.empty()) path = "/";

  Download download{ domain, path, argv[2] };
  download.start_process();
  return 0;
}
