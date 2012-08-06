#ifndef oodb_hh
#define oodb_hh

#include <string>
#include <utility>

#include <unordered_map>
#include <unordered_set>
#include <cstdint>

#include "file_driver.hh"

namespace oodb {

typedef std::unordered_map<std::string, std::string> HashMap;
typedef std::unordered_set<std::string> Set;
typedef uint64_t Integer;

typedef std::unordered_map<std::string, HashMap> Hashes;
typedef std::unordered_map<std::string, Set> Sets;
typedef std::unordered_map<std::string, std::string> Strings;
typedef std::unordered_map<std::string, Integer> Integers;

typedef std::pair<std::string, std::string> HashValue;

class Db {
public:
	Db (const std::string& FileName);
	~Db ();
	void load ();
	void save ();
	void setHash (const std::string& key, const HashValue& value);
	std::string& getHash (const std::string& key, const std::string& u_key);
	std::string getHash (const std::string& key, const std::string& u_key) const;

	Set& getSet (const std::string& key) { return m_sets[key]; }
	bool addSet (const std::string& key, const std::string& value);
	bool checkSet (const std::string& key, const std::string& value);
	bool checkSet (const std::string& key, const std::string& value) const;
	bool remSet (const std::string& key, const std::string& value);

	void setString (const std::string& key, const std::string& value);
	std::string& getString (const std::string& key);
	std::string getString (const std::string& key) const;
	void unsetString (const std::string& key);

	void setInteger (const std::string& key, Integer value);
	Integer getInteger (const std::string& key);
	Integer getInteger (const std::string& key) const;
	Integer incrInteger (const std::string& key);
	Integer decrInteger (const std::string& key);
	void unsetInteger (const std::string& key);

	void flush ();

	Hashes& getHashes () { return m_hashes; }
	Sets& getSets () { return m_sets; }
	Strings& getStrings () { return m_strings; }
	Integers& getIntegers () { return m_integers; }

private:

	void loadHashes ();
	void loadSets ();
	void loadStrings ();
	void loadIntegers ();

	void saveHashes ();
	void saveSets ();
	void saveStrings ();
	void saveIntegers ();

private:
	File m_file;

	Hashes m_hashes;
	Sets m_sets;
	Strings m_strings;
	Integers m_integers;
};

} // namespace oodb

#endif // oodb_hh
