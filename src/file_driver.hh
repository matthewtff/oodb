#ifndef oodb_file_driver_hh
#define oodb_file_driver_hh

#include <cerrno>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else /* _WIN32 */
#include <fcntl.h>
#include <unistd.h>
#endif /* _WIN32 */

namespace oodb {

class File {
 public:
#ifdef _WIN32
  typedef DWORD Flag;
  typedef HANDLE Handle;
  typedef DWORD Size;

  static const Flag ReadOnly = GENERIC_READ;
  static const Flag WriteOnly = GENERIC_WRITE;
  static const Flag ReadWrite = (GENERIC_READ | GENERIC_WRITE);
#else /* _WIN32 */
  enum Flag {
    ReadOnly = O_RDONLY,
    WriteOnly = O_WRONLY,
    ReadWrite = O_RDWR
  };

  using Handle = int;
  using Size = ssize_t;
#endif /* _WIN32 */

  File(const std::string& file_name, const Flag flag) : name_(file_name),
                                                        opened_(false),
                                                        flag_(flag) {
    Open();
  }

  // Allow move files.
  File(File&& file) : handle_(file.handle_),
                      name_(file.name_),
                      opened_(file.opened_),
                      flag_(file.flag_) {}

  ~File() { Close(); }

  void Clear() {
    Close();
    Erase();
    Open();
  }

  bool Open();
  void Close();
  void Erase();
  Size Write(const void* data, size_t size);
  Size Read(void* data, size_t size);
  bool IsOpened() const { return opened_; }

 private:
  Handle handle_;
  std::string name_;
  bool opened_;
  Flag flag_;
}; // class File

} // namespace oodb

#endif // oodb_file_driver_hh
