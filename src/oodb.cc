#include "oodb.hh"

#include <vector>

namespace oodb {

Db::Db (const std::string& FileName) : m_file(FileName, File::ReadWrite) 
{
	load();
}

Db::~Db ()
{
	save();
}

void Db::load ()
{
	loadHashes();
	loadSets();
	loadStrings();
	loadIntegers();
}

void Db::save ()
{
	m_file.clear();
	saveHashes();
	saveSets();
	saveStrings();
	saveIntegers();
}

void Db::setHash (const std::string& key, const HashValue& value)
{
	m_hashes[key][value.first] = value.second;
}

std::string& Db::getHash (const std::string& key, const std::string& u_key)
{
	return m_hashes[key][u_key];
}

std::string Db::getHash (const std::string& key, const std::string& u_key) const
{
	Hashes::const_iterator needed_hash = m_hashes.find(key);
	if (needed_hash != m_hashes.end()) {
		HashMap::const_iterator el = needed_hash->second.find(u_key);
		return el == needed_hash->second.end() ? std::string() : el->second;
	} else
		return std::string();
}


bool Db::addSet (const std::string& key, const std::string& value)
{
	return m_sets[key].insert(value).second;
}

bool Db::checkSet (const std::string& key, const std::string& value)
{
	Set& needed_set = m_sets[key];
	return needed_set.find(value) != needed_set.end();
}

bool Db::checkSet (const std::string& key, const std::string& value) const
{
	Sets::const_iterator needed_set = m_sets.find(key);
	return (needed_set != m_sets.end())
		? needed_set->second.find(value) != needed_set->second.end()
		: false;
}

bool Db::remSet (const std::string& key, const std::string& value)
{
	return m_sets[key].erase(value) > 0;
}


void Db::setString (const std::string& key, const std::string& value)
{
	m_strings[key] = value;
}

std::string& Db::getString (const std::string& key)
{
	return m_strings[key];
}

std::string Db::getString (const std::string& key) const
{
	Strings::const_iterator needed_string = m_strings.find(key);
	return needed_string != m_strings.end()
		? needed_string->second
		: std::string();
}

void Db::unsetString (const std::string& key)
{
	m_strings.erase(key);
}

void Db::setInteger (const std::string& key, Integer value)
{
	m_integers[key] = value;
}

Integer Db::getInteger (const std::string& key)
{
	return m_integers[key];
}

Integer Db::getInteger (const std::string& key) const
{
	Integers::const_iterator needed_integer = m_integers.find(key);
	return needed_integer != m_integers.end()
		? needed_integer->second
		: 0;
}

Integer Db::incrInteger (const std::string& key)
{
	return ++m_integers[key];
}

Integer Db::decrInteger (const std::string& key)
{
	return --m_integers[key];
}

void Db::unsetInteger (const std::string& key)
{
	m_integers.erase(key);
}

void Db::flush ()
{
	m_hashes.clear();
	m_sets.clear();
	m_strings.clear();
	m_integers.clear();
}

// private:

void Db::loadHashes ()
{
	// read number of hashes : [number]
	uint32_t number_of_hashes = 0;
	m_file.read(reinterpret_cast<void*>(&number_of_hashes), sizeof(uint32_t));
	for (uint32_t number = 0; number < number_of_hashes; ++number) {
		// read hash global key : [size(key), key]
		uint32_t hash_key_size = 0;
		m_file.read(reinterpret_cast<void*>(&hash_key_size), sizeof(uint32_t));
		std::vector<char> hash_key(hash_key_size);
		m_file.read(reinterpret_cast<void*>(&hash_key[0]), hash_key_size);

		// read number of key-value pairs : [number]
		uint32_t number_of_pairs = 0;
		m_file.read(reinterpret_cast<void*>(&number_of_pairs), sizeof(uint32_t));

		// read key-value pairs in format [size(key), key, size(value), value]
		for (uint32_t key_value_pair = 0; key_value_pair < number_of_pairs; ++key_value_pair) {
			// read key : [size(key), key]
			uint32_t key_size = 0;
			m_file.read(reinterpret_cast<void*>(&key_size), sizeof(uint32_t));
			std::vector<char> key_value(key_size);
			m_file.read(reinterpret_cast<void*>(&key_value[0]), key_size);

			// read value : [size(value), value]
			uint32_t value_size = 0;
			m_file.read(reinterpret_cast<void*>(&value_size), sizeof(uint32_t));
			std::vector<char> value_value(value_size);
			m_file.read(reinterpret_cast<void*>(&value_value[0]), value_size);

			m_hashes[std::string(&hash_key[0])][std::string(&key_value[0])] = std::string(&value_value[0]);
		}
	}
}

void Db::loadSets ()
{
	// read number of sets : [number]
	uint32_t number_of_sets = 0;
	m_file.read(reinterpret_cast<void*>(&number_of_sets), sizeof(uint32_t));
	for (uint32_t set = 0; set < number_of_sets; ++set) {
		// read set global key : [size(key), key]
		uint32_t set_key_size = 0;
		m_file.read(reinterpret_cast<void*>(&set_key_size), sizeof(uint32_t));
		std::vector<char> set_key(set_key_size);
		m_file.read(reinterpret_cast<void*>(&set_key[0]), set_key_size);

		// read number of set members : [number]
		uint32_t number_of_members = 0;
		m_file.read(reinterpret_cast<void*>(&number_of_members), sizeof(uint32_t));

		// read members of set in format : [size(member), member]
		for (uint32_t member = 0; member < number_of_members; ++member) {
			// read member : [size(member), member]
			uint32_t size_of_member = 0;
			m_file.read(reinterpret_cast<void*>(&size_of_member), sizeof(uint32_t));
			std::vector<char> member_value(size_of_member);
			m_file.read(reinterpret_cast<void*>(&member_value[0]), size_of_member);

			addSet(std::string(&set_key[0]), std::string(&member_value[0]));
		}
	}
}

void Db::loadStrings ()
{
	// red number of strings : [number]
	uint32_t number_of_strings = 0;
	m_file.read(reinterpret_cast<void*>(&number_of_strings), sizeof(uint32_t));
	for (uint32_t number = 0; number < number_of_strings; ++number) {
		// read string global key : [size(key), key]
		uint32_t string_key_size = 0;
		m_file.read(reinterpret_cast<void*>(&string_key_size), sizeof(uint32_t));
		std::vector<char> string_key(string_key_size);
		m_file.read(reinterpret_cast<void*>(&string_key[0]), string_key_size);

		// read string : [size(string), string]
		uint32_t string_size = 0;
		m_file.read(reinterpret_cast<void*>(&string_size), sizeof(uint32_t));
		std::vector<char> string_value(string_size);
		m_file.read(reinterpret_cast<void*>(&string_value[0]), string_size);

		setString(std::string(&string_key[0]), std::string(&string_value[0]));
	}
}

void Db::loadIntegers ()
{
	// read number of integers : [number]
	uint32_t number_of_integers = 0;
	m_file.read(reinterpret_cast<void*>(&number_of_integers), sizeof(uint32_t));
	for (uint32_t number = 0; number < number_of_integers; ++number) {
		// read integer global key : [size(key), key]
		uint32_t integer_key_size = 0;
		m_file.read(reinterpret_cast<void*>(&integer_key_size), sizeof(uint32_t));
		std::vector<char> integer_key(integer_key_size);
		m_file.read(reinterpret_cast<void*>(&integer_key[0]), integer_key_size);

		// read integer : [integer]
		Integer integer;
		m_file.read(reinterpret_cast<void*>(&integer), sizeof(Integer));

		setInteger(std::string(&integer_key[0]), integer);
	}
}

void Db::saveHashes ()
{
	// write number of hashes : [number]
	uint32_t number_of_hashes = m_hashes.size();
	m_file.write(reinterpret_cast<const void*>(&number_of_hashes), sizeof(uint32_t));
	for (Hashes::iterator hash = m_hashes.begin(); hash != m_hashes.end(); ++hash) {
		// write hash global key : [size(key), key]
		uint32_t hash_key_size = hash->first.size() + 1;
		m_file.write(reinterpret_cast<const void*>(&hash_key_size), sizeof(uint32_t));
		m_file.write(reinterpret_cast<const void*>(hash->first.c_str()), hash_key_size);

		// write number of key-value pairs : [number]
		uint32_t number_of_pairs = hash->second.size();
		m_file.write(reinterpret_cast<const void*>(&number_of_pairs), sizeof(uint32_t));
		
		// write key-value pairs in format [size(key), key, size(value), value]
		for (HashMap::iterator key_value = hash->second.begin();
			key_value != hash->second.end(); ++key_value)
		{
			// writing key : [size(key), key]
			uint32_t key_size = key_value->first.size() + 1;
			m_file.write(reinterpret_cast<const void*>(&key_size), sizeof(uint32_t));
			m_file.write(reinterpret_cast<const void*>(key_value->first.c_str()), key_size);

			// writing value : [size(value], value]
			uint32_t value_size = key_value->second.size() + 1;
			m_file.write(reinterpret_cast<const void*>(&value_size), sizeof(uint32_t));
			m_file.write(reinterpret_cast<const void*>(key_value->second.c_str()), value_size);
		}
	}
}

void Db::saveSets ()
{
	// write number of sets : [number]
	uint32_t number_of_sets = m_sets.size();
	m_file.write(reinterpret_cast<const void*>(&number_of_sets), sizeof(uint32_t));
	for (Sets::iterator set = m_sets.begin(); set != m_sets.end(); ++set) {
		// write set global key : [size(key), key]
		uint32_t set_key_size = set->first.size() + 1;
		m_file.write(reinterpret_cast<const void*>(&set_key_size), sizeof(uint32_t));
		m_file.write(reinterpret_cast<const void*>(set->first.c_str()), set_key_size);

		// write number of set members : [number]
		uint32_t number_of_members = set->second.size();
		m_file.write(reinterpret_cast<const void*>(&number_of_members), sizeof(uint32_t));
		
		// write members of set in format [size(member), member]
		for (Set::iterator member = set->second.begin(); member != set->second.end(); ++member)
		{
			// writing member : [size(member), member]
			uint32_t member_size = member->size() + 1;
			m_file.write(reinterpret_cast<const void*>(&member_size), sizeof(uint32_t));
			m_file.write(reinterpret_cast<const void*>(member->c_str()), member_size);
		}
	}
}

void Db::saveStrings ()
{
	// write number of strings : [number]
	uint32_t number_of_strings = m_strings.size();
	m_file.write(reinterpret_cast<const void*>(&number_of_strings), sizeof(uint32_t));
	for (Strings::iterator str = m_strings.begin(); str != m_strings.end(); ++str) {
		// write string global key : [size(key), key]
		uint32_t string_key_size = str->first.size() + 1;
		m_file.write(reinterpret_cast<const void*>(&string_key_size), sizeof(uint32_t));
		m_file.write(reinterpret_cast<const void*>(str->first.c_str()), string_key_size);

		// write string : [size(key), key]
		uint32_t string_size = str->second.size() + 1;
		m_file.write(reinterpret_cast<const void*>(&string_size), sizeof(uint32_t));
		m_file.write(reinterpret_cast<const void*>(str->second.c_str()), string_size);
	}
}

void Db::saveIntegers ()
{
	// write number of integers : [number]
	uint32_t number_of_integers = m_integers.size();
	m_file.write(reinterpret_cast<const void*>(&number_of_integers), sizeof(uint32_t));
	for (Integers::iterator integer = m_integers.begin(); integer != m_integers.end(); ++integer) {
		// write integer global key : [size(key), key]
		uint32_t integer_key_size = integer->first.size() + 1;
		m_file.write(reinterpret_cast<const void*>(&integer_key_size), sizeof(uint32_t));
		m_file.write(reinterpret_cast<const void*>(integer->first.c_str()), integer_key_size);

		// write integer : [integer]
		m_file.write(reinterpret_cast<const void*>(&(integer->second)), sizeof(Integer));
	}
}

} // namespace oodb
