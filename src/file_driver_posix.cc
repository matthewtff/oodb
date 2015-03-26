#include "file_driver.hh"

namespace oodb {

bool File::Open() {
  const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
  handle_ = open(name_.c_str(), flag_ | O_CREAT, mode);
  opened_ = handle_ != -1;
  return opened_;
}

void File::Close() {
  if (!opened_) {
    return;
  }
  close(handle_);
  opened_ = false;
}

void File::Erase() {
  unlink(name_.c_str());
}

File::Size File::Write(const void* data, size_t size) {
  if (!opened_) {
    return -1;
  }
  return write(handle_, data, size);
}

File::Size File::Read(void* data, size_t size) {
  if (!opened_) {
    return -1;
  }
  return read(handle_, data, size);
}

} // namespace oodb
