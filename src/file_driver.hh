#ifndef oodb_file_driver_hh
#define oodb_file_driver_hh

#include <string>

#ifdef _WIN32

#include <windows.h>

#else /* _WIN32 */

#include <fcntl.h>
#include <unistd.h>

#endif /* _WIN32 */

#include <cerrno>

namespace oodb {

class File {
public:

#ifdef _WIN32

	typedef DWORD Flag;

	static const Flag ReadOnly = GENERIC_READ;
	static const Flag WriteOnly = GENERIC_WRITE;
	static const Flag ReadWrite = (GENERIC_READ | GENERIC_WRITE);

	typedef HANDLE Handle;
	typedef DWORD ssize_t;

#else /* _WIN32 */

	enum Flag {
		ReadOnly = O_RDONLY,
		WriteOnly = O_WRONLY,
		ReadWrite = O_RDWR
	};

	typedef int Handle;
	typedef ssize_t ssize_t;

#endif /* _WIN32 */

public:
	File (const std::string& file_name, Flag flag);
	~File ();
	bool open ();
	void close ();
	void del ();
	void clear ();
	ssize_t write (const void* data, size_t size);
	ssize_t read (void* data, size_t size);
	bool opened () const { return m_opened; }

private:
	Handle m_handle;
	std::string m_name;
	bool m_opened;
	Flag m_flag;
}; // class File

} // namespace oodb

#endif // oodb_file_driver_hh
