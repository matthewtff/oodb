#include "file_driver.hh"

namespace oodb {

bool File::Open() {
  handle_ = CreateFileA(name_.c_str(), flag_, 0, NULL, OPEN_ALWAYS, 0, NULL);
  opened_ = handle_ != INVALID_HANDLE_VALUE;
  return opened_;
}

void File::Close() {
  if (!opened_) {
    return;
  }
  CloseHandle(handle_);
  opened_ = false;
}

void File::Erase() {
  DeleteFileA(name_.c_str());
}

File::Size File::Write(const void* data, size_t size) {
  if (!opened_) {
    return -1;
  }
  DWORD written = 0;
  return WriteFile(handle_, data, size, &written, NULL) ? written : -1;
}

File::Size File::Read(void* data, size_t size) {
  if (opened_) {
    return -1;
  }
  DWORD readed = 0;
  return ReadFile(handle_, data, size, &readed, NULL) ? readed : -1;
}

} // namespace oodb
