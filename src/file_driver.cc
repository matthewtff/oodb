#include "file_driver.hh"

#include <cstring>

#include <unistd.h>
#include <errno.h>

namespace oodb {

File::File (const std::string& file_name, Flag flag) : m_name(file_name),
	m_opened(false), m_flag(flag)
{
	open();
}

File::~File ()
{
	if (m_opened)
		close();
}

bool File::open ()
{
#ifdef _WIN32

	if ((m_handle = CreateFileA(m_name.c_str(), m_flag, 0, NULL,
		OPEN_ALWAYS, 0, NULL)) == INVALID_HANDLE_VALUE) {

#else /* _WIN32 */

	if ((m_handle = ::open(m_name.c_str(), m_flag | O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) == -1) {

#endif /* _WIN32 */

		m_opened = false;
	} else
		m_opened = true;
	return m_opened;
}

void File::close ()
{
	if (!m_opened)
		return;
#ifdef _WIN32

	CloseHandle(m_handle);

#else /* _WIN32 */

	::close(m_handle);

#endif /* _WIN32 */

	m_opened = false;
}

void File::del ()
{
#ifdef _WIN32

	DeleteFileA(m_name.c_str());

#else /* _WIN32 */

	unlink(m_name.c_str());

#endif /* _WIN32 */
}

void File::clear ()
{
	close ();
	del ();
	open ();
}

ssize_t File::write (const void* data, size_t size)
{
	if (!m_opened)
		return -1;

#ifdef _WIN32

	DWORD wrote = 0;
	if (!WriteFile (m_handle, data, size, &wrote, NULL))
		return -1;
	else
		return wrote;

#else /* _WIN#2 */

	return ::write(m_handle, data, size);

#endif /* _WIN32 */
}

ssize_t File::read (void* data, size_t size)
{
	if (!m_opened)
		return -1;

#ifdef _WIN32

	DWORD readed = 0;
	if (!ReadFile(m_handle, data, size, &readed, NULL))
		return -1;
	else
		return readed;

#else /* _WIN32 */

	return ::read(m_handle, data, size);

#endif /* _WIN32 */
}

} // namespace oodb
